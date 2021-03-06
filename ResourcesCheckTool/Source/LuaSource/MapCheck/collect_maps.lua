---
--- Generated by EmmyLua(https://github.com/EmmyLua)
--- Created by dieseldong.
--- DateTime: 2021/6/23 0023 11:44
---
require("define")

local CollectMap = {}

---@param mapNames string[]
---@param ModNames string[]
---@return AssetData[]
function CollectMap.CollectCheckMaps(mapNames, ModNames)

    local result = {}

    ---@type MapCheckCommandlet
    local MapCheckCommandlet = import("MapCheckCommandlet")

    for _,v in pairs(mapNames) do
        local mapFolder = "/Game/"..v
        ---@type LuaArray
        local maps = MapCheckCommandlet.GetAllMapAssets(mapFolder)
        local num = maps:Num()
        for i = 0, num-1 do
            result[#result + 1] = maps:Get(i)
        end
    end

    for _, modName in pairs(ModNames) do
        for _,v in pairs(mapNames) do
            local mapFolder = "/Game/"..modName.."/"..v
            ---@type LuaArray
            local maps = MapCheckCommandlet.GetAllMapAssets(mapFolder)
            local num = maps:Num()
            for i = 0, num-1 do
                result[#result + 1] = maps:Get(i)
            end
        end
    end

    return result
end

return CollectMap