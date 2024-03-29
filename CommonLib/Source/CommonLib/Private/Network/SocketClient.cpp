﻿#include "Network/SocketClient.h"

#include "CommonLib.h"
#include "IPAddress.h"
#include "Interfaces/IPv4/IPv4Address.h"
#include "Sockets.h"
#include "SocketSubsystem.h"


FSocketClient::FSocketClient(const FString& name, TSharedPtr<INetPackageHandler> handler)
{
	Name = name;
	ShouldStop = false;
	LastHeartBeatTime = FPlatformTime::Cycles();
	PackageHandler = handler;
}


FSocketClient::~FSocketClient()
{
	ShouldStop = true;
	delete ClientSocket;
	ClientSocket = nullptr;

}

FSocketClient* FSocketClient::SetBuffer(int32 bufferSize)
{
	BufferSize = bufferSize;
	return this;
}

FSocketClient* FSocketClient::SetConnectTarget(FString host, int32 port)
{
	FIPv4Address ip;
	FIPv4Address::Parse(host, ip);

	InternetAddress = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->CreateInternetAddr();
	InternetAddress->SetIp(ip.Value);
	InternetAddress->SetPort(port);
	return this;
}


const FString& FSocketClient::GetName() const
{
	return Name;
}

const int32 FSocketClient::GetBufferSize() const
{
	return BufferSize;
}

void FSocketClient::Run()
{
	ShouldStop = false;
	while (!ShouldStop)
	{
		UE_LOG(LogTemp, Warning, TEXT("Connecting To %s ..."), *InternetAddress->ToString(true))
		if(!Connect())
		{
			UE_LOG(LogTemp, Warning, TEXT("Connect To %s Error..."), *InternetAddress->ToString(true))
		}
		else
		{
			if(OnRegist.IsBound())
			{
				OnRegist.Broadcast();
			}
			
			UE_LOG(LogTemp, Warning, TEXT("Connect To %s Success!!"), *InternetAddress->ToString(true))
			NeedReconnect = false;
			while(ClientSocket != nullptr && ClientSocket->GetConnectionState() == SCS_Connected)
			{
				if(ShouldStop || NeedReconnect)
				{
					break;
				}

				WriteMessageBuffer();
				ReadSocketBuffer();
				DoHeartBeat();
			
				FPlatformProcess::Sleep(0.3);
			}

			if(OnClose.IsBound())
			{
				OnClose.Broadcast();
			}
		}
		FPlatformProcess::Sleep(1);
	}

	ClientSocket->Close();
	ClientSocket = nullptr;
}

void FSocketClient::SendMessage(const FNetPackage& NetPackage)
{
	SendMessageQueue.Enqueue(NetPackage);
}


void FSocketClient::Stop()
{
	ShouldStop = true;
}

bool FSocketClient::Connect()
{

	if(ClientSocket)
	{
		ClientSocket->Close();
	}

	ClientSocket = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->CreateSocket(NAME_Stream, InternetAddress->ToString(true), false);
	if(ClientSocket->Connect(*InternetAddress) == false)
	{
		return false;
	}
	return true;
}

void FSocketClient::WriteMessageBuffer()
{
	if (!ClientSocket)
	{
		return;
	}
	
	FNetPackage sendContent;
	SendMessageQueue.Dequeue(sendContent);

	if(!sendContent.IsEmpty())
	{
		int32 bytesSend = 0;
		
		int32 len = sendContent.Length;
		ClientSocket->Send((uint8*)&len, 4, bytesSend);
		ClientSocket->Send(sendContent.Data.GetData(), sendContent.Length, bytesSend);

		if(bytesSend < 0)
		{
			NeedReconnect = true;
		}
	}
}

void FSocketClient::ReadSocketBuffer()
{
	if (!ClientSocket)
	{
		return;
	}

	TArray<uint8> ReceivedData;
	uint32 Size;

	while(ClientSocket->HasPendingData(Size))
	{
		UE_LOG(ResetCore_CommonLib, Log, TEXT("Receive Data"))

		int32 Read = 0;
		uint8 datalength[4] = { 0 };
		FString ReceivedUE4String = "";

		//接受信息
		{
			ClientSocket->Recv((uint8*)datalength, 4, Read);
			int Len = *((uint32*)(datalength));

			UE_LOG(ResetCore_CommonLib, Log, TEXT("GetRecData Len = %d"), Len);

			Read = 0;
			ReceivedData.Empty();
			ReceivedData.Init(0, FMath::Max(Size, 3000u));
		
			bool result = ClientSocket->Wait(ESocketWaitConditions::WaitForRead, FTimespan(0,0,120));
			
			//超时没收到包，关闭socket，走重连逻辑
			if (!result) {
				ClientSocket->Close();
				ClientSocket = nullptr;
				NeedReconnect = true;
				UE_LOG(ResetCore_CommonLib, Error, TEXT("Recv timeout"));
				return;
			}
			ClientSocket->Recv(ReceivedData.GetData(), Len, Read);

			ReceivedData.Add(0);
			ReceivedMessageQueue.Enqueue(FNetPackage(Len, ReceivedData));
		}

	}

	while(!ReceivedMessageQueue.IsEmpty())
	{
		FNetPackage Package;
		ReceivedMessageQueue.Dequeue(Package);

		PackageHandler->HandlePackage(*this, Package);
	}
	

}

void FSocketClient::DoHeartBeat()
{
	if(FPlatformTime::ToSeconds(FPlatformTime::Cycles() - LastHeartBeatTime)  >= HeartBeatSecond)
	{
		if(OnHeartBeat.IsBound())
		{
			OnHeartBeat.Broadcast();
		}
		LastHeartBeatTime = FPlatformTime::Cycles();
	}
}

