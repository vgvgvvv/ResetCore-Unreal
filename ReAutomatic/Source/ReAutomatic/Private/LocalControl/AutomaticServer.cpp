#include "LocalControl/AutomaticServer.h"
#include "ReAutomatic.h"
#include "IPv4Address.h"
#include "IPv4Endpoint.h"
#include "TcpSocketBuilder.h"
#include "Sockets.h"
#include "LocalControl/AutomaticManager.h"


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


void FAutomaticServer::TryReceiveMessage()
{
	if(!ConnectSocket)
	{
		return;
	}

	ISocketSubsystem* SocketSubsystem = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM);
	uint32 PendingDataSize = 0;

	//Binary Array!
	TArray<uint8> ReceivedData;

	uint32 Size;
	while (this->ConnectSocket->HasPendingData(Size))
	{
		UE_LOG(LogAutomatic, Log, TEXT("ReciveData"));

		int32 Read = 0;
		uint8 datalength[4] = { 0 };
		FString ReceivedUE4String = "";

		this->ConnectSocket->Recv((uint8*)datalength, 4, Read);
		int Len = *((uint32*)(datalength));
		UE_LOG(LogAutomatic, Log, TEXT("GetRecData Len = %d"), Len);

		Read = 0;
		ReceivedData.Empty();
		ReceivedData.Init(0, FMath::Max(Size, 3000u));

		bool result = ConnectSocket->Wait(ESocketWaitConditions::WaitForRead, FTimespan(0, 0, 120));

		//超时没收到包，关闭socket，走重连逻辑
		if (!result) 
		{
			this->ConnectSocket->Close();
			this->ConnectSocket = nullptr;
			ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->DestroySocket(ConnectSocket);
			UE_LOG(LogAutomatic, Error, TEXT("Recv timeout"));
			return;
		}
		
		this->ConnectSocket->Recv(ReceivedData.GetData(), Len, Read);

		ReceivedData.Add(0);
		ReceivedUE4String = FString(ANSI_TO_TCHAR(reinterpret_cast<const char*>(ReceivedData.GetData())));
		
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
		ConnectSocket->Send((uint8*)UTF8_TO_TCHAR(*SendContent), sizeof(ANSICHAR)*contentList.Num(), bytesSend);
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
