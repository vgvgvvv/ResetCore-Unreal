---
--- Generated by EmmyLua(https://github.com/EmmyLua)
--- Created by dieseldong.
--- DateTime: 2021/6/23 0023 10:41
---

require("define")

local CheckFunctions = require("MapCheck.check_functions")


---@type MapConfig[]
local MapCheckConfig = {}

--绿岛规则
MapCheckConfig["forest"] = {
    Desc = "雨林地图",
    MapFolders = {
        "Maps/PUBG_Baltic/Baltic_Building",
        "Maps/PUBG_Baltic/Baltic_Object",
        "Maps/PUBG_Baltic/Baltic_Trees",
        "Maps/PUBG_Baltic/Baltic_Landscape"
    },
    Mods = {

    },
    Rules = {
        {
            Desc = "检查建筑物以及物体地块",
            Regex = {"Building", "Object"},
            CheckFuncs = {
                {Desc = "CheckBounds", Func = CheckFunctions.CheckBounds, Args = {size = 110000}}
            }
        },
        {
            Desc = "检查植被地块",
            Regex = {"Trees"},
            CheckFuncs ={
                {Desc = "CheckBounds", Func = CheckFunctions.CheckBounds, Args = {size = 210000}}
            }
        },
    }
}

return MapCheckConfig