#include "RouteClient/RouteClient.h"

#include "ReAutomatic.h"
#include "Misc/FileHelper.h"
#include "Network/Serializer/FNetJsonSerializer.h"
#include "JsonObjectConverter.h"
#include "Misc/Paths.h"
#include "RouteClient/NetPackageHandler/RouteMessageHandler.h"
#include "Network/SocketClient.h"
#include "Utility/JsonUtil.h"
#include "UE4ControlCenter/IRouteClient.h"
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

	FString Name = FPlatformMisc::GetDeviceMakeAndModel() + "(" + FPlatformMisc::GetDeviceId() + ")";
	if(JsonObject->HasField("Name"))
	{
		Name = JsonObject->GetStringField("Name");
	}
	auto Host = JsonObject->GetStringField("Host");
	auto Port = JsonObject->GetIntegerField("Port");
	
	Init(Name, Host, Port);
	return true;
}

/// 初始化RouteClient
bool FRouteClient::Init(const FString& Name, const FString& Host, const int32 Port)
{
	TSharedPtr<FRouteMessageHandler> handler = MakeShared<FRouteMessageHandler>();
	LocalClient = new FSocketClient(Name, handler);
	LocalClient->SetConnectTarget(Host, Port);

	LocalClient->OnRegist.AddRaw(this, &FRouteClient::OnRegister);
	LocalClient->OnHeartBeat.AddRaw(this, &FRouteClient::OnHeartBeat);
	return true;
}

/// 后台运行RouteClient
FLambdaRunnable* FRouteClient::RunWithBackground()
{
	if(Runner != nullptr)
	{
		return Runner;
	}
	Runner = FLambdaRunnable::RunLambdaOnBackGroundThread([this]()
    {
        LocalClient->Run();
    });

	return Runner;
}

/// 在Tick中运行
void FRouteClient::RunWithTick(float DeltaTime)
{
	//TODO
}

void FRouteClient::Stop() const
{
	if(LocalClient)
	{
		LocalClient->Stop();
	}
}


const FString& FRouteClient::GetName() const
{
	return LocalClient->GetName();
}

void FRouteClient::SendMessage(const FNetPackage& Package)
{
	if(LocalClient)
	{
		LocalClient->SendMessage(Package);
	}
}


void FRouteClient::OnRegister() const
{
	FRouteRegistMessage Message;
    Message.RegistName = LocalClient->GetName();
    Message.RegistType = ERouteType::UnrealEngine;

    const TSharedPtr<FJsonObject> JSONObj = FJsonObjectConverter::UStructToJsonObject(Message);

    auto Package = NetPackageFromProtoIDMessage(ERouteProtoID::Regist, JSONObj);
    LocalClient->SendMessage(Package);
}

void FRouteClient::OnHeartBeat() const
{
	const TSharedPtr<FJsonObject> JSONObj = MakeShared<FJsonObject>();
    auto Package = NetPackageFromProtoIDMessage(ERouteProtoID::HeartBeat, JSONObj);
    LocalClient->SendMessage(Package);
}

