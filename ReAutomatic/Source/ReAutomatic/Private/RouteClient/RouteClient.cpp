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

void FRouteClient::Start(const FString Name, const FString Host, const int32 Port)
{
	LocalClient = new FSocketClient(Name, MakeShared<FJsonPackageHandler>());
	LocalClient->SetConnectTarget(Host, Port);

	LocalClient->OnRegist.AddLambda([this]()
	{
		FRouteRegistMessage Message;
		Message.RegistName = LocalClient->GetName();
		Message.RegistType = ERouteType::UnrealEngine;

		const auto JSONObj = FJsonObjectConverter::UStructToJsonObject(Message);
		auto Package = FNetPackageUtil::MakePack<TSharedPtr<FJsonObject>, FNetJsonSerializer>(JSONObj);
		
		LocalClient->SendMessage(Package);
	});

	LocalClient->OnHeartBeat.AddLambda([]()
	{
		
	});
	
	Runner = FLambdaRunnable::RunLambdaOnBackGroundThread([this]()
    {
        LocalClient->Run();
    });
}

void FRouteClient::Stop() const
{
	LocalClient->Stop();
}


