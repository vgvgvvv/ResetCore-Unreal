---
--- Generated by EmmyLua(https://github.com/EmmyLua)
--- Created by dieseldong.
--- DateTime: 2021/6/22 0022 16:43
---

Common = Common or {}


function Common.Wait(nTime)
    coroutine.yield("wait", nTime)
end

---任意类型object转字符串, 对于table会自动格式化输出
---@param object any
function _G.dump(object)
    if type(object) == 'table' then
        local s = '{ '
        local first = true

        for k,v in pairs(object) do
            if k ~= "__index" then
                local nextStr = k ..':' .. dump(v)

                if not first then
                    nextStr = ', ' .. nextStr
                end

                s = s .. nextStr
                first = false
            end
        end

        return s .. ' }'
    end

    return tostring(object)
end

---@param array LuaArray
function _G.ArrayToTable(array)
    local table = {}
    local num = array:Num()
    for i=0,num-1 do
        table[i+1] = array:Get(i)
    end
    return table
end

