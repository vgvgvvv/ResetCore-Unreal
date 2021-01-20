#include "RouteClient/RouteClient.h"



#include "FNetJsonSerializer.h"
#include "RouteClient/RouteProto.h"
#include "Network/SocketClient.h"
#include "Network/NetPackageHandler/JsonPackageHandler.h"
#include "Runtime/JsonUtilities/Public/JsonObjectConverter.h"
#include "Utility/LambdaRunnable.h"

FRouteClient::~FRouteClient()
{
	if(Runner)
	{
		Runner->Stop();
		Runner->EnsureCompletion();

		delete Runner;
		Runner = nullptr;
	}
	
	if(LocalClient)
	{
		LocalClient->Stop();

		delete LocalClient;
		LocalClient = nullptr;
	}
}

void FRouteClient::Init(const FString Name, const FString Host, const int32 Port)
{
	LocalClient = new FSocketClient(Name, MakeShared<FJsonPackageHandler>());
	LocalClient->SetConnectTarget(Host, Port);

	LocalClient->OnRegist.AddLambda([this]()
	{
		FRouteRegistMessage Message;
		Message.RegistName = LocalClient->GetName();
		Message.RegistType = ERouteType::UnrealEngine;

		const auto JSONObj = FJsonObjectConverter::UStructToJsonObject(Message);
		SendProtoIDMessage(ERouteProtoID::Regist, JSONObj);
	});

	LocalClient->OnHeartBeat.AddLambda([this]()
	{
		SendProtoIDMessage(ERouteProtoID::HeartBeat, MakeShared<FJsonObject>());
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


void FRouteClient::SendMessage(TSharedPtr<FJsonObject> JsonObject) const
{
	FNetPackage Package = FNetPackageUtil::MakePack<TSharedPtr<FJsonObject>, FNetJsonSerializer>(JsonObject);
	LocalClient->SendMessage(Package);
}

void FRouteClient::SendProtoIDMessage(ERouteProtoID ProtoID,TSharedPtr<FJsonObject> Content) const
{
	TSharedPtr<FJsonObject> message = MakeShared<FJsonObject>();
	message->SetNumberField("ProtoID", static_cast<int32>(ProtoID));
	message->SetObjectField("Content", Content);

	SendMessage(message);
}

void FRouteClient::SendRouteMessage(const FRawCommandMessage& CommandMessage, const FString& TargetName, const ERouteType TargetType) const
{
	FRouteSendMessage RouteSendMessage;
	RouteSendMessage.TargetRouteName = TargetName;
	RouteSendMessage.TargetRouteType = TargetType;
	auto RouteSendMessageObj = FJsonObjectConverter::UStructToJsonObject(RouteSendMessage);

	TSharedPtr<FJsonObject> RawCommandMessageObj = MakeShared<FJsonObject>();
	RawCommandMessageObj->SetStringField("CmdId", CommandMessage.CmdId);
	RawCommandMessageObj->SetObjectField("Content", CommandMessage.Content);
	
	RouteSendMessageObj->SetObjectField("Content", RawCommandMessageObj);
	
	SendProtoIDMessage(ERouteProtoID::NormalMessage, RouteSendMessageObj);
}
