#include "AutomaticServer.h"
#include "ReAutomatic.h"
#include "IPv4Address.h"
#include "IPv4Endpoint.h"
#include "TcpSocketBuilder.h"
#include "Sockets.h"
#include "AutomaticManager.h"


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


void FAutomaticServer::TryReceiveMessage() const
{
	if(!ConnectSocket)
	{
		return;
	}

	ISocketSubsystem* SocketSubsystem = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM);
	uint32 PendingDataSize = 0;

	//Binary Array!
	TArray<uint8> ReceivedData;

	TArray<uint8> ReceivedDataEn;

	uint32 Size;
	while (this->ConnectSocket->HasPendingData(Size))
	{
		UE_LOG(LogAutomatic, Log, TEXT("ReciveData"));
		ReceivedData.Init(0, FMath::Min(Size, 65507u));
		ReceivedDataEn.Init(0, FMath::Min(Size, 65507u));

		int32 Read = 0;
		this->ConnectSocket->Recv(ReceivedData.GetData(), ReceivedData.Num(), Read);

		ReceivedDataEn.Add(0);
		FString ReceivedUE4String = FString(ANSI_TO_TCHAR(reinterpret_cast<const char*>(ReceivedDataEn.GetData())));
		UE_LOG(LogAutomatic, Log, TEXT("GetRecData %s"), *ReceivedUE4String);
		{
			// if (ReceivedUE4String.Contains("NewSocket="))
			// {
			// 	ReceivedUE4String = ReceivedUE4String.Replace(TEXT("NewSocket="), TEXT(""));
			// 	int32 newPort = FCString::Atoi(*ReceivedUE4String);
			// 	CreateTCPConnection(newPort);
			// 	break;
			// }

			// Get Message String
			FAutomaticManager::Get().PushCommandFromClient(ReceivedUE4String);
		}

	}

}


void FAutomaticServer::SendMessage(const FString& SendContent) const
{
	if (!ConnectSocket)
	{
		return;
	}
	if (!SendContent.IsEmpty())
	{
		TArray<TCHAR> contentList = SendContent.GetCharArray();
		int32 bytesSend = 0;
		ConnectSocket->Send((uint8*)TCHAR_TO_ANSI(*SendContent), sizeof(ANSICHAR)*contentList.Num(), bytesSend);
		UE_LOG(LogAutomatic, Log, TEXT("SendData %s"), *SendContent);
	}
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
