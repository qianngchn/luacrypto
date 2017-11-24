#!/usr/bin/env lua

if #arg ~= 1 then
    print("Usage: " .. arg[0] .. " <file>")
    os.exit(false)
end

local crc32 = assert(require("crypto").crc32())
local file = assert(io.open(arg[1], "rb"))
local buf = nil
local size = 0x100000
local value = 0

crc32:init()
buf = file:read(size)
while buf do
    crc32:update(buf, #buf)
    buf = file:read(size)
end
file:close()
value = crc32:fetch()
print(string.format("%08x", value))

os.exit(true)
