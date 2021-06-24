---
--- Generated by EmmyLua(https://github.com/EmmyLua)
--- Created by dieseldong.
--- DateTime: 2021/6/21 0021 13:48
---

require("define")

local Main = class("MapCheck")

function Main:ctor()

end

function Main:main()

    local allConfigs = require("MapCheck.config.forest_config")
    local collectMaps = require("MapCheck.collect_maps")
    local checkMaps = require("MapCheck.check_maps")
    local showMsgs = require("MapCheck.show_msg")

    ---@type CheckSingleConfigResult[]
    local msgTable = {}

    for _, config in pairs(allConfigs) do
        -- 收集需要检查的地图
        local maps = collectMaps.CollectCheckMaps(config.MapFolders, config.Mods)

        -- 具体进行检查流程
        ---@type CheckSingleConfigResult
        local msgs = checkMaps.CheckSingleConfig(maps, config)

        msgTable[config.Desc] = msgs
    end

    -- 进行告警与归档
    showMsgs.Show(msgTable)

end

return Main.New()
