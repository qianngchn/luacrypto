#!/usr/bin/env lua

if #arg ~= 2 then
    print("Usage: " .. arg[0] .. " <key> <message>")
    os.exit(false)
end

local rc4 = assert(require("crypto").rc4())
local key = arg[1]
local message = arg[2]

print("message: " .. message)
rc4:reset(key)
message = rc4:crypt(message)
print("encrypt: " .. message)
rc4:reset(key)
message = rc4:crypt(message)
print("decrypt: " .. message)

os.exit(true)
