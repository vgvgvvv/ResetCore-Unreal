﻿#pragma once
#include "CoreMinimal.h"

#include "NetPackage.h"
#include "Containers/Queue.h"
#include "NetPackageHandler/NetPackageHandler.h"

class FSocket;
class FInternetAddr;
DECLARE_MULTICAST_DELEGATE(FOnRegistRouteClient);
DECLARE_MULTICAST_DELEGATE(FOnCloseRouteClient);
DECLARE_MULTICAST_DELEGATE(FOnHeartBeatRouteClient);

class COMMONLIB_API FSocketClient
{
public:
	//Client属性相关

	FSocketClient* SetBuffer(int32 bufferSize);
	FSocketClient* SetConnectTarget(FString host, int32 port);

	FOnRegistRouteClient OnRegist;
	FOnCloseRouteClient OnClose;
	FOnHeartBeatRouteClient OnHeartBeat;

private:

	TSharedPtr<FInternetAddr> InternetAddress;
	int32 BufferSize = 2 * 1024 * 1024;	
	FString Name;

	int32 LastHeartBeatTime;
	int32 HeartBeatSecond = 3;
public:
	const FString& GetName() const;
	const int32 GetBufferSize() const;


public:
	//生命周期相关
	FSocketClient(const FString& name, TSharedPtr<INetPackageHandler> PackageHandler);
	~FSocketClient();

	void Run();
	void SendMessage(const FNetPackage& NetPackage);
	void Stop();
private:

	bool Connect();

	void WriteMessageBuffer();
	void ReadSocketBuffer();
	void DoHeartBeat();

private:
	
	FSocket* ClientSocket = nullptr;
	TQueue<FNetPackage> SendMessageQueue;
	TQueue<FNetPackage> ReceivedMessageQueue;

	bool ShouldStop = false;
	bool NeedReconnect = false;

	TSharedPtr<INetPackageHandler> PackageHandler;
};
