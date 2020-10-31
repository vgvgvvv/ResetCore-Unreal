#pragma once
#include "IPv4/IPv4Endpoint.h"
#include "Sockets.h"


class FAutomaticServer
{
public:

	enum class EStatus
	{
		Stop,
		WaitingForConnect,
		Running
	};

	FAutomaticServer();

public:
	EStatus GetCurrentStatus();
	
	FSocket* TCPSocket;
	FSocket* ConnectSocket;

	void Init(FString host, int port, FString Name);
	void WaitForConnect();
	void Update();
	void Stop();

private:

	EStatus CurrentStatus;
	FIPv4Endpoint RemoteAddressForConnection;
};