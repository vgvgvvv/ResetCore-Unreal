---
--- Generated by EmmyLua(https://github.com/EmmyLua)
--- Created by dieseldong.
--- DateTime: 2021/6/23 0023 12:06
---

require("define")

local CheckFunctions = {}

---检查bounds大小
---@param map AssetData
---@param args table
---@return string[]
function CheckFunctions.CheckBounds(map, args)

    local size = args.size

    local result = {}

    ---@type MapCheckCommandlet
    local MapCheckCommandlet = import("MapCheckCommandlet")

    ---@type Vector
    local extend = MapCheckCommandlet.GetLevelBoundsExtend()

    local scale = extend * 2

    if scale.X > size or scale.Y > size or scale.Z > size then
        local msg = map.AssetName.." 地图尺寸过大!! 当前尺寸为 ("..scale.X..", "..scale.Y..", "..scale.Z..")"
        print(msg)
        result[#result + 1] = msg
    else
        print(map.AssetName.."x:"..scale.X..", y:"..scale.Y..", z:"..scale.Z.." is no problem~")
    end

    return result;
end

return CheckFunctions