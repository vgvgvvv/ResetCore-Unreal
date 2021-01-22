#include "RouteClient/NetPackageHandler/RouteMessageHandler.h"


#include "EnumUtil.h"
#include "JsonUtil.h"
#include "ReAutomatic.h"
#include "JsonUtilities.h"
#include "RouteProto.h"
#include "Async/Async.h"
#include "UE4ControlCenter/UE4CmdArg.h"
#include "UE4ControlCenter/UE4ControlCenter.h"


void FRouteMessageHandler::HandleJsonInfo(FSocketClient& from, TSharedPtr<FJsonObject> jsonObject)
{
	FRouteSendMessage msg;
	FJsonObjectConverter::JsonObjectToUStruct<FRouteSendMessage>(jsonObject.ToSharedRef(), &msg);

	UE_LOG(LogAutomatic, Log, TEXT("recv from %s (%s) %s"), *msg.FromRouteName,
		*FEnumUtil::EnumToString(TEXT("ERouteType"), msg.FromRouteType),
		*FJsonUtil::JsonObjectToString(jsonObject))

	FRemoteControllerInfo controllerInfo;
	controllerInfo.ControllerName = msg.FromRouteName;
	controllerInfo.ControllerType = msg.FromRouteType;
	
	auto ContentObject = jsonObject->GetObjectField("Content");
	// RawCommand
	auto CmdId = ContentObject->GetStringField("CmdId");
	auto RawCommandContent = ContentObject->GetField<EJson::None>("Content");

	if(CmdId == FCmdTypes::Log)
	{
		auto LogStr = RawCommandContent->AsString();
		UE_LOG(LogAutomatic, Log, TEXT("Log From Route Server : %s"), *LogStr)
	}
	else if (CmdId == FCmdTypes::UE4Msg)
	{
		auto UE4MessageObject = RawCommandContent->AsObject();
		
		auto CmdId = UE4MessageObject->GetStringField("CmdId");
		auto Index = UE4MessageObject->GetIntegerField("Index");
		auto CmdContent = UE4MessageObject->GetField<EJson::None>("Content");

		FUE4CmdArg Arg;
		Arg.CmdId = CmdId;
		Arg.Content = RawCommandContent;
		Arg.ControllerInfo = controllerInfo;


		auto Task = [CmdId, Arg]()
		{
			FUE4CmdArg Temp = Arg;
			FName Name = FName(*CmdId);
			FUE4ControlCenter::Get().Trigger(Name, Temp);
		};
		AsyncTask(ENamedThreads::GameThread, Task);
	}
	else if (CmdId == FCmdTypes::RunLua)
	{
		auto RunLuaMessageObject = RawCommandContent->AsObject();
		auto Index = RunLuaMessageObject->GetIntegerField("Index");
		auto LuaStr = RunLuaMessageObject->GetStringField("LuaScript");

		auto Task = [LuaStr, controllerInfo]()
		{
			FUE4ControlCenter::Get().OnRunLua.Broadcast(LuaStr, controllerInfo);
		};
		
		AsyncTask(ENamedThreads::GameThread, Task);
	}else
	{
		UE_LOG(LogAutomatic, Error, TEXT("Not Support Cmdid : %s"), *CmdId)
	}
}
