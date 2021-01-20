#include "RouteClient/RouteClient.h"



#include "FNetJsonSerializer.h"
#include "RouteMessagePackage.h"
#include "RouteClient/RouteProto.h"
#include "Network/SocketClient.h"
#include "Network/NetPackageHandler/JsonPackageHandler.h"
#include "Runtime/JsonUtilities/Public/JsonObjectConverter.h"
#include "Utility/LambdaRunnable.h"

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

		auto Package = FRouteMessagePackage::MakeFromProtoIDMessage(ERouteProtoID::Regist, JSONObj);
		LocalClient->SendMessage(Package);
	});

	LocalClient->OnHeartBeat.AddLambda([this]()
	{
		auto Package = FRouteMessagePackage::MakeFromProtoIDMessage(ERouteProtoID::Regist, MakeShared<FJsonObject>());
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

