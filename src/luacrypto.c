#include <string.h>
#include <stdarg.h>
#include <stdlib.h>
#include <lauxlib.h>
#include "crc32.h"
#include "rc4.h"

// Lua extra function, define a class with metatable, keep stack balance
static void luaextra_defineclass(lua_State *L, const char *name, const luaL_Reg *func) {
    luaL_newmetatable(L, name);
    lua_pushstring(L, "__index");
    lua_newtable(L);
    lua_pushstring(L, "name");
    lua_pushstring(L, name);
    lua_rawset(L, -3);
    for (; func->name; func++) {
        lua_pushstring(L, func->name);
        lua_pushcfunction(L, func->func);
        lua_rawset(L, func->name[0] == '_' ? -5 : -3);
    }
    lua_rawset(L, -3);
    lua_pop(L, 1);
}

// Lua extra function, add a new class to the top of stack
static void *luaextra_newclass(lua_State *L, const char *name, size_t size) {
    luaL_newmetatable(L, name);
    lua_pop(L, 1);

    void *pointer = NULL;
    pointer = lua_newuserdata(L, size);
    luaL_setmetatable(L, name);
    return pointer;
}

// Lua extra function, check the args on stack simply
static void luaextra_checkopt(lua_State *L, int argc, ...) {
    if (argc < 0) argc = 0;
    int argv = 0, index = 1;

    va_list ap;
    va_start(ap, argc);
    while (argc--) {
        luaL_checkany(L, index);
        argv = va_arg(ap, int);
        luaL_checktype(L, index, argv);
        index++;
    }
    va_end(ap);
}

// Lua CRC32 Module
static int lua_crc32_init(lua_State *L);
static int lua_crc32_update(lua_State *L);
static int lua_crc32_fetch(lua_State *L);

static const luaL_Reg CRC32[] = {
    {"init", lua_crc32_init},
    {"update", lua_crc32_update},
    {"fetch", lua_crc32_fetch},
    {NULL, NULL}
};

static int lua_crc32_open(lua_State *L) {
    luaextra_defineclass(L, "CRC32", CRC32);
    luaextra_newclass(L, "CRC32", sizeof(CRC32_Handle));

    return 1;
}

static int lua_crc32_init(lua_State *L) {
    luaextra_checkopt(L, 1, LUA_TUSERDATA);

    CRC32_Handle *handle = NULL;
    uint32_t init = 0;

    handle = (CRC32_Handle *)luaL_checkudata(L, 1, "CRC32");
    init = (uint32_t)luaL_optinteger(L, 2, 0);
    crc32_init(handle, init);

    return 0;
}

static int lua_crc32_update(lua_State *L) {
    luaextra_checkopt(L, 2, LUA_TUSERDATA, LUA_TSTRING);

    CRC32_Handle *handle = NULL;
    const char *buf = NULL;
    size_t len = 0;

    handle = (CRC32_Handle *)luaL_checkudata(L, 1, "CRC32");
    buf = luaL_checkstring(L, 2);
    len = (size_t)luaL_optinteger(L, 3, strlen(buf));
    crc32_update(handle, (const uint8_t *)buf, len);

    return 0;
}

static int lua_crc32_fetch(lua_State *L) {
    luaextra_checkopt(L, 1, LUA_TUSERDATA);

    CRC32_Handle *handle = NULL;
    uint32_t crc32 = 0;

    handle = (CRC32_Handle *)luaL_checkudata(L, 1, "CRC32");
    crc32_fetch(handle, &crc32);
    lua_pushinteger(L, crc32);

    return 1;
}

// Lua RC4 Module
static int lua_rc4_reset(lua_State *L);
static int lua_rc4_crypt(lua_State *L);

static const luaL_Reg RC4[] = {
    {"reset", lua_rc4_reset},
    {"crypt", lua_rc4_crypt},
    {NULL, NULL}
};

static int lua_rc4_open(lua_State *L) {
    luaextra_defineclass(L, "RC4", RC4);
    luaextra_newclass(L, "RC4", sizeof(RC4_State));

    return 1;
}

static int lua_rc4_reset(lua_State *L) {
    luaextra_checkopt(L, 2, LUA_TUSERDATA, LUA_TSTRING);

    RC4_State *state = NULL;
    const char *key = NULL;
    size_t len = 0;

    state = (RC4_State *)luaL_checkudata(L, 1, "RC4");
    key = luaL_checkstring(L, 2);
    len = (size_t)luaL_optinteger(L, 3, strlen(key));
    rc4_reset(state, (const uint8_t *)key, len);

    return 0;
}

static int lua_rc4_crypt(lua_State *L) {
    luaextra_checkopt(L, 2, LUA_TUSERDATA, LUA_TSTRING);

    RC4_State *state = NULL;
    const char *in = NULL;
    char *out = NULL;
    size_t len = 0;

    state = (RC4_State *)luaL_checkudata(L, 1, "RC4");
    in = luaL_checkstring(L, 2);
    len = (size_t)luaL_optinteger(L, 3, strlen(in));
    out = (char *)malloc(sizeof(char) * len);
    rc4_crypt(state, (const uint8_t *)in, (uint8_t *)out, len);
    lua_pushlstring(L, out, len);
    free(out);

    return 1;
}

// Lua Crypto Library
static const luaL_Reg Crypto[] = {
    {"rc4", lua_rc4_open},
    {"crc32", lua_crc32_open},
    {NULL, NULL}
};

int luaopen_crypto(lua_State *L) {
    luaL_newlib(L, Crypto);
    return 1;
}
