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
	auto CmdId = ContentObject->GetStringField("CmdId");

	if(CmdId == FCmdTypes::Log)
	{
		auto Value = ContentObject->GetField<EJson::String>("Content");
		auto LogStr = Value->AsString();
		UE_LOG(LogAutomatic, Log, TEXT("Log From Route Server : %s"), *LogStr)
	}
	else if (CmdId == FCmdTypes::UE4Msg)
	{
		auto Value = ContentObject->GetField<EJson::Object>("Content");
		auto UE4JsonObj = Value->AsObject();

		
		auto CmdId = UE4JsonObj->GetStringField("CmdId");
		auto CmdContent = UE4JsonObj->GetField<EJson::None>("Content");

		auto Arg = FUE4CmdArg(CmdId, CmdContent);

		AsyncTask(ENamedThreads::GameThread, [CmdId, Arg]()
		{
			auto Temp = Arg;
			FUE4ControlCenter::Get().Trigger(FName(*CmdId), Temp);
		});
	}
	else if (CmdId == FCmdTypes::RunLua)
	{
		auto Value = ContentObject->GetField<EJson::String>("Content");
		auto LuaStr = Value->AsString();

		AsyncTask(ENamedThreads::GameThread, [LuaStr]()
		{
			FUE4ControlCenter::Get().OnRunLua.Broadcast(LuaStr);
		});
	}else
	{
		UE_LOG(LogAutomatic, Error, TEXT("Not Support Cmdid : %s"), *CmdId)
	}
}
