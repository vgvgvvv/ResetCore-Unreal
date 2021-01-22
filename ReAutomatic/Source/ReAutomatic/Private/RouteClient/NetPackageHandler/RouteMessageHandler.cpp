#include "RouteClient/NetPackageHandler/RouteMessageHandler.h"


#include "EnumUtil.h"
#include "JsonUtil.h"
#include "ReAutomatic.h"
#include "RouteProto.h"
#include "JsonUtilities.h"
#include "SocketClient.h"
#include "UE4ControlCenter/UE4ControlCenter.h"
#include "Async/Async.h"


void FRouteMessageHandler::HandleJsonInfo(FSocketClient& from, TSharedPtr<FJsonObject> jsonObject)
{
	FRouteSendMessage msg;
	FJsonObjectConverter::JsonObjectToUStruct<FRouteSendMessage>(jsonObject.ToSharedRef(), &msg);

	UE_LOG(LogAutomatic, Log, TEXT("recv from %s (%s) %s"), *msg.FromRouteName,
		*FEnumUtil::EnumToString(TEXT("ERouteType"), msg.FromRouteType),
		*FJsonUtil::JsonObjectToString(jsonObject))

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

		auto Arg = FUE4CmdArg(CmdId, RawCommandContent);

		AsyncTask(ENamedThreads::GameThread, [CmdId, Arg]()
		{
			auto Temp = Arg;
			FUE4ControlCenter::Get().Trigger(FName(*CmdId), Temp);
		});
	}
	else if (CmdId == FCmdTypes::RunLua)
	{
		auto RunLuaMessageObject = RawCommandContent->AsObject();
		auto Index = RunLuaMessageObject->GetIntegerField("Index");
		auto LuaStr = RunLuaMessageObject->GetStringField("LuaScript");

		AsyncTask(ENamedThreads::GameThread, [LuaStr]()
		{
			FUE4ControlCenter::Get().OnRunLua.Broadcast(LuaStr);
		});
	}else
	{
		UE_LOG(LogAutomatic, Error, TEXT("Not Support Cmdid : %s"), *CmdId)
	}
}
