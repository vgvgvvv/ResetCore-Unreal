
---@class LuaArray
local LuaArray
---@return number
function LuaArray:Num()  end
---@return number
function LuaArray:Get()  end
---@param value any
function LuaArray:Set(value) end
---@param value any
function LuaArray:Add(value) end
---@param index number
---@param value any
function LuaArray:Insert(index, value) end
---@param index number
function LuaArray:Remove(index) end
function LuaArray:Clear() end
function LuaArray:CreateValueTypeObject() end

---@class LuaMapEnumerator

---@class LuaMap
local LuaMap
---@return number
function LuaMap:Num()  end
function LuaMap:Get(key)  end
function LuaMap:Add(key, value)  end
function LuaMap:Remove(key)  end
function LuaMap:Clear()  end
function LuaMap:CreateValueTypeObject() end

---@class LuaSet
local LuaSet
---@return number
function LuaSet:Num()  end
