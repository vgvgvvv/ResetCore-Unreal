#include "UE4ControlCenter/IRouteClient.h"


#include "JsonObjectConverter.h"
#include "NetPackage.h"


class FNetJsonSerializer;

FNetPackage IRouteClient::NetPackageFromJsonObject(TSharedPtr<FJsonObject> JsonObject)
{
	return FNetPackageUtil::MakePack<TSharedPtr<FJsonObject>, FNetJsonSerializer>(JsonObject);
}

FNetPackage IRouteClient::NetPackageFromProtoIDMessage(ERouteProtoID ProtoID, TSharedPtr<FJsonObject> Content)
{
	TSharedPtr<FJsonObject> message = MakeShared<FJsonObject>();
	message->SetNumberField("ProtoID", static_cast<int32>(ProtoID));
	message->SetObjectField("Content", Content);
	
	return NetPackageFromJsonObject(message);
}

FNetPackage IRouteClient::NetPackageFromRouteMessage(const FRawCommandMessage& CommandMessage,
    const FString& TargetName, const ERouteType TargetType)
{
	FRouteSendMessage RouteSendMessage;
	RouteSendMessage.TargetRouteName = TargetName;
	RouteSendMessage.TargetRouteType = TargetType;
	auto RouteSendMessageObj = FJsonObjectConverter::UStructToJsonObject(RouteSendMessage);

	TSharedPtr<FJsonObject> RawCommandMessageObj = MakeShared<FJsonObject>();
	RawCommandMessageObj->SetStringField("CmdId", CommandMessage.CmdId);
	RawCommandMessageObj->SetField("Content", CommandMessage.Content);
	
	RouteSendMessageObj->SetObjectField("Content", RawCommandMessageObj);
	
	return NetPackageFromProtoIDMessage(ERouteProtoID::NormalMessage, RouteSendMessageObj);
}

FNetPackage IRouteClient::NetPackageFromLogMessage(const FString& Log, const FString& TargetName,
	const ERouteType TargetType)
{
	FRawCommandMessage LogMessage;
	LogMessage.CmdId = FCmdTypes::Log;
	LogMessage.Content = MakeShared<FJsonValueString>(Log);

	return NetPackageFromRouteMessage(LogMessage, TargetName, TargetType);
}


FNetPackage IRouteClient::NetPackageFromLuaResult(const TSharedPtr<FJsonValue> Result, const FString& TargetName,
	const ERouteType TargetType)
{
	FRawCommandMessage ResultMessage;
	ResultMessage.CmdId = FCmdTypes::RunLuaFinish;
	ResultMessage.Content = Result;

	return NetPackageFromRouteMessage(ResultMessage, TargetName, TargetType);
}

FNetPackage IRouteClient::NetPackageFromUE4MsgResult(const TSharedPtr<FJsonValue> Result, const FString& TargetName,
	const ERouteType TargetType)
{
	FRawCommandMessage ResultMessage;
    ResultMessage.CmdId = FCmdTypes::UE4MsgResult;
    ResultMessage.Content = Result;

    return NetPackageFromRouteMessage(ResultMessage, TargetName, TargetType);
}

FNetPackage IRouteClient::NetPackageFromLuaResult(const TSharedPtr<FJsonValue> Result,
	const FRemoteControllerInfo& ControllerInfo)
{
	return NetPackageFromLuaResult(Result, ControllerInfo.ControllerName, ControllerInfo.ControllerType);
}

FNetPackage IRouteClient::NetPackageFromUE4MsgResult(const TSharedPtr<FJsonValue> Result,
	const FRemoteControllerInfo& ControllerInfo)
{
	return NetPackageFromUE4MsgResult(Result, ControllerInfo.ControllerName, ControllerInfo.ControllerType);
}
