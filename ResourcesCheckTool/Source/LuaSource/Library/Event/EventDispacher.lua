local package = package
local setmetatable = setmetatable
local pairs = pairs

--[[
    数据层次
    ["EventName1"] =
    {
        ["_StaticFunc"] = { Func1, Func2 },

        [Object1] = { Func1, Func2 },
        [Object2] = { Func1, Func2 },
    },
    ...
]]
---@class EventDispatcher
EventDispatcher = class(EventDispatcher)


-- 默认调用函数
local function PreInvoke(func, object, ...)
    if object then
        func(object, ...)
    else
        func(...)
    end
end

---@return EventDispatcher
function EventDispatcher.New()
    local obj = setmetatable( {}, { __index = package.loaded["EventDispatcher"] } )
    obj.mPreInvokeFunc = PreInvoke
    obj.mEventTable = {}
    return obj
end

--[[
    @Description: 根据事件和对象注册回调
    @Date: 2018/7/13
    @Param: [eventDispatch: 事件器 eventName:事件名称 func: 事件回调 object: 事件关联对象 userData:TODO]
    @Return
--]]
---@param eventName string @事件名
---@param func fun() @回调函数
---@param object any @绑定对象
function EventDispatcher:Add(eventName, func, object, userData)
    if not self.mEventTable[eventName] then self.mEventTable[eventName] = {} end
    local eventDatas = self.mEventTable[eventName]

    if not object then object = "_StaticFunc" end
    if not eventDatas[object] then eventDatas[object] = {} end
    eventDatas[object][func] = userData or true
end

-- 设置调用前回调
---@param HookFunc fun() @调用前回调
function EventDispatcher:SetDispatchHook(HookFunc)
    self.mPreInvokeFunc = HookFunc
end

--[[
    @Description: 派发事件
    @Date: 2018/7/13
    @Param: [args]
    @Return
--]]
---@param eventName string
function EventDispatcher:Dispatch(eventName, ...)
    local eventDatas = self.mEventTable[eventName]
    if not Exist(self, eventName) then return end

    for object, objectFuncs in pairs(eventDatas) do
        if object == "_StaticFunc" then object = nil end
        for func, userData in pairs(objectFuncs) do
            self.mPreInvokeFunc(func, object, ...)
        end
    end

end

--[[
    @Description: 是否存在回调
    @Date: 2018/7/13
    @Param: [eventDispatch 事件派发器 eventName: 事件名称]
    @Return
--]]
---@param eventName string
function EventDispatcher:Exist(eventName)
    local eventDatas = self.mEventTable[eventName]
    if not eventDatas then return false end

    local ret = false
    for _, funcs in pairs(eventDatas) do
        if table.ro_size(funcs) > 0 then
            ret = true
            break
        end
    end
    return ret
end

--[[
    @Description: 根据object&func清除事件回调
    @Date: 2018/7/13
    @Param: [args]
    @Return
--]]
---@param eventName string @事件名
---@param func fun() @回调函数
---@param object any @绑定对象
function EventDispatcher:Remove(eventName, func, object)
    local eventDatas = self.mEventTable[eventName]
    if not eventDatas then return end
    if not object then object = "_StaticFunc" end
    if eventDatas[object] and func then eventDatas[object][func] = nil end
end

--[[
    @Description: 根据object清除所有回调
    @Date: 2018/7/13
    @Param: [args]
    @Return
--]]
-- 清除对象的所有回调
function EventDispatcher:RemoveObjectAllFunc(eventName, object)
    local eventDatas = self.mEventTable[eventName]
    if eventDatas and eventDatas[object] then eventDatas[object] = nil end
end