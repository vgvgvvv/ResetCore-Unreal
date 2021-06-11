#include "UE4ControlCenter/IRouteClient.h"


#include "JsonObjectConverter.h"
#include "Network/NetPackage.h"
#include "RouteClient/Serializer/AESEncryptionSerializer.h"
#include "Utility/JsonUtil.h"
#include "Utility/LinqCore.h"


class FNetJsonSerializer;

FNetPackage IRouteClient::NetPackageFromJsonObject(TSharedPtr<FJsonObject> JsonObject)
{
	auto Result = FNetPackageUtil::MakePack<TSharedPtr<FJsonObject>, FAESEncryptionSerializer<>>(JsonObject);

#if UE_BUILD_DEVELOPMENT
	FString BytesStr = "";
	for(int i = 0; i < Result.Data.Num(); i ++)
	{
		BytesStr = BytesStr + FString::FromInt(Result.Data[i]) + ",";
	}
	UE_LOG(LogTemp, Log, TEXT("Make Net Package : %s Length : %d Content:%s"), *FJsonUtil::JsonObjectToString(JsonObject), Result.Data.Num(), *BytesStr)
#endif
	
	return Result;
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
	RouteSendMessage.FromRouteType = ERouteType::UnrealEngine;
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


FNetPackage IRouteClient::NetPackageFromLuaResult(int32 Index, const TSharedPtr<FJsonObject> Result, const FString& TargetName,
	const ERouteType TargetType)
{
	FRawCommandMessage ResultMessage;
	ResultMessage.CmdId = FCmdTypes::RunLuaFinish;

	TSharedPtr<FJsonObject> ResultObj = MakeShared<FJsonObject>();
	ResultObj->SetNumberField("Index", Index);
	ResultObj->SetObjectField("Content", Result);
	ResultMessage.Content = MakeShared<FJsonValueObject>(ResultObj);

	return NetPackageFromRouteMessage(ResultMessage, TargetName, TargetType);
}

FNetPackage IRouteClient::NetPackageFromUE4MsgResult(int32 Index, const TSharedPtr<FJsonObject> Result, const FString& TargetName,
	const ERouteType TargetType)
{
	FRawCommandMessage ResultMessage;
    ResultMessage.CmdId = FCmdTypes::UE4MsgResult;

	TSharedPtr<FJsonObject> ResultObj = MakeShared<FJsonObject>();
	ResultObj->SetNumberField("Index", Index);
	ResultObj->SetObjectField("Content", Result);
    ResultMessage.Content = MakeShared<FJsonValueObject>(ResultObj);

    return NetPackageFromRouteMessage(ResultMessage, TargetName, TargetType);
}

FNetPackage IRouteClient::NetPackageFromLuaResult(int32 Index, const TSharedPtr<FJsonObject> Result,
	const FRemoteControllerInfo& ControllerInfo)
{
	return NetPackageFromLuaResult(Index, Result, ControllerInfo.ControllerName, ControllerInfo.ControllerType);
}

FNetPackage IRouteClient::NetPackageFromUE4MsgResult(int32 Index, const TSharedPtr<FJsonObject> Result,
	const FRemoteControllerInfo& ControllerInfo)
{
	return NetPackageFromUE4MsgResult(Index, Result, ControllerInfo.ControllerName, ControllerInfo.ControllerType);
}
