#pragma once
#include "IPv4/IPv4Endpoint.h"
#include "Runnable.h"
#include "CoreMinimal.h"


#include "NetPackage.h"
#include "Queue.h"
#include "NetPackageHandler/NetPackageHandler.h"
#include "Serializer/FNetJsonSerializer.h"

DECLARE_MULTICAST_DELEGATE(FOnRegistRouteClient);
DECLARE_MULTICAST_DELEGATE(FOnCloseRouteClient);
DECLARE_MULTICAST_DELEGATE(FOnHeartBeatRouteClient);

class FSocketClient
{
public:
	//Client属性相关

	FSocketClient& SetBuffer(int32 bufferSize);
	FSocketClient& SetConnectTarget(FString host, int32 port);

	FOnRegistRouteClient OnRegist;
	FOnCloseRouteClient OnClose;
	FOnHeartBeatRouteClient OnHeartBeat;

private:

	TSharedPtr<FInternetAddr> InternetAddress;
	int32 BufferSize = 2 * 1024 * 1024;	
	FString Name;

	int32 LastUpdateTime;
	int32 HeartBeatSecond = 3;


public:
	//生命周期相关
	FSocketClient(const FString& name);
	~FSocketClient();

	void Run();
	void SendMessage(FNetPackage& NetPackage);
	void Stop();
private:

	bool Connect();

	void WriteMessageBuffer();
	void ReadSocketBuffer();
	void DoHeartBeat();

private:
	
	FSocket* ClientSocket = nullptr;
	FRunnableThread* RunnableThread = nullptr;
	TQueue<FNetPackage> SendMessageQueue;
	TQueue<FNetPackage> ReceivedMessageQueue;

	bool ShouldStop = false;

	TSharedPtr<INetPackageHandler> PackageHandler;
};
