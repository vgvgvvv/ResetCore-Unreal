#include "AutomaticServer.h"
#include "ReAutomatic.h"
#include "IPv4Address.h"
#include "IPv4Endpoint.h"
#include "TcpSocketBuilder.h"
#include "Sockets.h"


static int32 BufferSize = 2 * 1024 * 1024;

FAutomaticServer::FAutomaticServer()
	: TCPSocket(nullptr)
	, ConnectSocket(nullptr)
	, CurrentStatus(EStatus::Stop)
{
}


FAutomaticServer::EStatus FAutomaticServer::GetCurrentStatus()
{
	return CurrentStatus;
}

void FAutomaticServer::Init(FString host, int port, FString Name)
{
	FIPv4Address Ipv4Address;
	FIPv4Address::Parse(host, Ipv4Address);
	FIPv4Endpoint Endpoint(Ipv4Address, port);

	if (TCPSocket)
	{
		TCPSocket->Close();
		TCPSocket = nullptr;
	}

	TCPSocket = FTcpSocketBuilder(Name)
		.AsReusable()
		.BoundToEndpoint(Endpoint)
		.Listening(8);

	if(!TCPSocket)
	{
		UE_LOG(LogAutomatic, Error, TEXT("Init TCP Socket Failed"));
		return;
	}else
	{
		UE_LOG(LogAutomatic, Error, TEXT("Init TCP Socket success"));
	}
	
	TCPSocket->SetSendBufferSize(BufferSize, BufferSize);
	TCPSocket->SetReceiveBufferSize(BufferSize, BufferSize);

	CurrentStatus = EStatus::WaitingForConnect;
}


void FAutomaticServer::WaitForConnect()
{
	if (!TCPSocket || ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM) == nullptr)
	{
		return;
	}

	TSharedPtr<FInternetAddr> RemoteAddress = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->CreateInternetAddr();
	if (!RemoteAddress.IsValid())
	{
		
		return;
	}
	
	bool Pending = false;
	if (TCPSocket->HasPendingConnection(Pending) && Pending)
	{
		//UE_LOG(AutoRobotThread, Log, TEXT("XUP Connection"));
		if (ConnectSocket != nullptr)
		{
			ConnectSocket->Close();
			ConnectSocket = nullptr;
			ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->DestroySocket(ConnectSocket);
		}

		ConnectSocket = TCPSocket->Accept(*RemoteAddress, TEXT("TCP Received Socket Connection"));

		if (ConnectSocket != nullptr)
		{
			RemoteAddressForConnection = FIPv4Endpoint(RemoteAddress);
			CurrentStatus = EStatus::Running;

			UE_LOG(LogAutomatic, Log,
				TEXT("Connected %s : %d"), *RemoteAddressForConnection.Address.ToString(), RemoteAddressForConnection.Port);
		}else
		{
			CurrentStatus = EStatus::WaitingForConnect;
		}
	}
	
}


void FAutomaticServer::Update()
{
}

void FAutomaticServer::Stop()
{
	if(TCPSocket)
	{
		TCPSocket->Close();
		TCPSocket = nullptr;
	}
	CurrentStatus = EStatus::Stop;
}
