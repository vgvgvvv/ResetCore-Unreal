#include "RouteClient/NetPackageHandler/RouteMessageHandler.h"


#include "EnumUtil.h"
#include "JsonUtil.h"
#include "ReAutomatic.h"
#include "RouteProto.h"
#include "JsonUtilities.h"
#include "SocketClient.h"


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
}
