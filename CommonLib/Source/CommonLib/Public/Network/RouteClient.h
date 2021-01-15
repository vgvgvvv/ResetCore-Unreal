#pragma once
#include "IPv4Endpoint.h"
#include "Runnable.h"
#include "CoreMinimal.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnRegistRouteClient);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnCloseRouteClient);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnHeartBeatRouteClient);

class FRouteClient
{
public:
	//Client属性相关

	FRouteClient& SetBuffer(int32 bufferSize);
	FRouteClient& SetConnectTarget(FString& host, int32 port);

	FOnRegistRouteClient OnRegist;
	FOnCloseRouteClient OnClose;
	FOnHeartBeatRouteClient OnHeartBeat;

private:

	TSharedRef<FInternetAddr> InternetAddress;
	int32 BufferSize = 2 * 1024 * 1024;	
	FString Name;


public:
	//生命周期相关
	
	static FRouteClient Create(FString& name);
	void Run();
	void SendMessage();
	void Stop();
private:

	bool Connect();

	void WriteMessageBuffer();
	void ReadSocketBuffer();
	void DoHeartBeat();

private:

	FRouteClient(FString name);
	~FRouteClient();
	
	FSocket* ClientSocket = nullptr;
	FRunnableThread* RunnableThread = nullptr;
	

	bool ShouldStop = false;
};

class FRouteClientRunner : public FRunnable
{
public:

	FRouteClientRunner(FRouteClient& client) : Client(client){} 
	
	virtual bool Init() override;
	virtual uint32 Run() override;
	virtual void Stop() override;
private:
	FRouteClient& Client;
};