package.path = "?.lua;"..
        "Library/?.lua;"

require("Library.class")
require("Library.DataStructure.DataStructure")
require("Library.common_api")

print = slua.display
json = require("Library.json")

linq = require("Library.linq")

WeCom = require("Library.wecom")

---@type CommonLuaLibrary
LuaCommon = import("CommonLuaLibrary")

