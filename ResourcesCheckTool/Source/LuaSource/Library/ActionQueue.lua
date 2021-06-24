
---@class ActionQueue
ActionQueue = class("ActionQueue")

---@return ActionQueue
function ActionQueue.New()
    return ActionQueue.New()
end

function ActionQueue:ctor()

    self.running = false
    self.actionQueue = queue.LinkedListQueue.create()
    self.currentQuitHandler = nil
    self.callback = nil
end

--添加异步行为（需要有回调
---@field action function
---@field onClear function
function ActionQueue:AddAciton(action, onClear)
    -- logGreen("AddAction:"..self.actionQueue:size())
    if self.running then
        self.actionQueue:enqueue(
            {
                Action = function()
                    self:WaitAndDo(action)
                end,
                OnClear = onClear
            }
        )
    else
        self.running = true
        self.currentQuitHandler = onClear
        self:WaitAndDo(action)
    end
    return self
end

--添加同步行为
---@field action function
---@field onClear function
function ActionQueue:AddAcitonWithoutCallback(action, onClear)
    if self.running then
        self.actionQueue:enqueue(
            {
                Action = function()
                    self:WaitAndDoWithoutCallback(action)
                end,
                OnClear = onClear
            }
        )
    else
        self.running = true
        self.currentQuitHandler = onClear
        self:WaitAndDoWithoutCallback(action)
    end
    return self
end

--清除行为
function ActionQueue:Clear()
    -- logGreen("Clear Action currentsize:"..self.actionQueue:size())
    while self.actionQueue:size() > 0 do
        self.actionQueue:dequeue()
    end
    if self.currentQuitHandler ~= nil then
        -- logGreen("中断处理")
        self.currentQuitHandler()
        self.currentQuitHandler = nil
    else
        -- logGreen("无需中断处理")
    end
    if self.callback ~= nil then
        self.callback(false)
    end
    self.running = false
end

function ActionQueue:WaitAndDo(action)
    action(function()
        self:TryDequeue()
    end)
end

function ActionQueue:WaitAndDoWithoutCallback(action)
    action()
    self:TryDequeue()
end

function ActionQueue:TryDequeue()
    -- logGreen("DequeueAction:"..self.actionQueue:size())
    --如果队列里面有丶东西
    if self.actionQueue:size() > 0 then
        --出队列调用
        local l_actionTable = self.actionQueue:dequeue()
        self.currentQuitHandler = l_actionTable.OnClear
        l_actionTable.Action()
    else
        if self.callback ~= nil then
            self.callback(true)
        end
        self.running = false
    end

end

--无论是否中断，只要运行结束都会执行回调,第一个参数会传回是否为做完
function ActionQueue:SetCallback(callback)
    self.callback = callback
end

function ActionQueue:IsEmpty()
    return self.actionQueue:size() == 0
end

return ActionQueue