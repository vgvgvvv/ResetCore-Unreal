#include "RouteClient/RouteClient.h"

#include "ReAutomatic.h"
#include "FileHelper.h"
#include "Network/Serializer/FNetJsonSerializer.h"
#include "JsonObjectConverter.h"
#include "Paths.h"
#include "RouteClient/NetPackageHandler/RouteMessageHandler.h"
#include "Network/SocketClient.h"
#include "Utility/JsonUtil.h"


FRouteClient::~FRouteClient()
{
	if(LocalClient)
	{
		LocalClient->Stop();
	}
	
	if(Runner)
	{
		delete Runner;
		Runner = nullptr;
	}
	
	if(LocalClient)
	{
		delete LocalClient;
		LocalClient = nullptr;
	}
}


bool FRouteClient::InitWithFile()
{
	const FString AutomaticConfigFilePath = FPaths::ProjectSavedDir() + TEXT("run_automatic.json");
	if (!FPaths::FileExists(*AutomaticConfigFilePath))
	{
		UE_LOG(LogAutomatic, Log, TEXT("Remote Contorl not enabled"))
		return false;
	}

	FString ConfigJson;
	FFileHelper::LoadFileToString(ConfigJson, *AutomaticConfigFilePath);

	if(ConfigJson.IsEmpty())
	{
		UE_LOG(LogAutomatic, Log, TEXT("File is empty : %s"), *AutomaticConfigFilePath)
		return false;
	}

	auto JsonObject = FJsonUtil::StringToJsonObject(ConfigJson);

	auto Name = JsonObject->GetStringField("Name");
	auto Host = JsonObject->GetStringField("Host");
	auto Port = JsonObject->GetIntegerField("Port");
	
	Init(Name, Host, Port);
	return true;
}

void FRouteClient::Init(const FString& Name, const FString& Host, const int32 Port)
{
	LocalClient = new FSocketClient(Name, MakeShared<FRouteMessageHandler>());
	LocalClient->SetConnectTarget(Host, Port);

	LocalClient->OnRegist.AddLambda([this]()
	{
		FRouteRegistMessage Message;
		Message.RegistName = LocalClient->GetName();
		Message.RegistType = ERouteType::UnrealEngine;
	
		const auto JSONObj = FJsonObjectConverter::UStructToJsonObject(Message);
	
		auto Package = NetPackageFromProtoIDMessage(ERouteProtoID::Regist, JSONObj);
		LocalClient->SendMessage(Package);
	});
	
	LocalClient->OnHeartBeat.AddLambda([this]()
	{
		const TSharedPtr<FJsonObject> JSONObj = MakeShared<FJsonObject>();
		auto Package = NetPackageFromProtoIDMessage(ERouteProtoID::HeartBeat, JSONObj);
		LocalClient->SendMessage(Package);
	});
}


FLambdaRunnable* FRouteClient::RunWithBackground()
{
	Runner = FLambdaRunnable::RunLambdaOnBackGroundThread([this]()
    {
        LocalClient->Run();
    });

	return Runner;
}


void FRouteClient::RunWithTick(float DeltaTime)
{
	//TODO
}

void FRouteClient::Stop() const
{
	LocalClient->Stop();
}

FNetPackage FRouteClient::NetPackageFromJsonObject(TSharedPtr<FJsonObject> JsonObject)
{
	return FNetPackageUtil::MakePack<TSharedPtr<FJsonObject>, FNetJsonSerializer>(JsonObject);
}

FNetPackage FRouteClient::NetPackageFromProtoIDMessage(ERouteProtoID ProtoID, TSharedPtr<FJsonObject> Content)
{
	TSharedPtr<FJsonObject> message = MakeShared<FJsonObject>();
	message->SetNumberField("ProtoID", static_cast<int32>(ProtoID));
	message->SetObjectField("Content", Content);
	
	return NetPackageFromJsonObject(message);
}

FNetPackage FRouteClient::NetPackageFromRouteMessage(const FRawCommandMessage& CommandMessage,
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

FNetPackage FRouteClient::NetPackageFromLogMessage(const FString& Log, const FString& TargetName,
	const ERouteType TargetType)
{
	FRawCommandMessage LogMessage;
	LogMessage.CmdId = FCmdTypes::Log;
	LogMessage.Content = MakeShared<FJsonValueString>(Log);

	return NetPackageFromRouteMessage(LogMessage, TargetName, TargetType);
}
