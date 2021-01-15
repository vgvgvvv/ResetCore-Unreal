#include "Network/RouteClient.h"



#include "IPAddress.h"
#include "IPv4Address.h"
#include "RunnableThread.h"
#include "Sockets.h"
#include "SocketSubsystem.h"
#include "TcpSocketBuilder.h"


FRouteClient::FRouteClient(FString name)
{
	Name = name;
	ShouldStop = false;
}


FRouteClient::~FRouteClient()
{
	delete ClientSocket;
	ClientSocket = nullptr;

	delete RunnableThread;
	RunnableThread = nullptr;
}

FRouteClient FRouteClient::Create(FString& name)
{
	return FRouteClient(name);
}

FRouteClient& FRouteClient::SetBuffer(int32 bufferSize)
{
	BufferSize = bufferSize;
	return *this;
}

FRouteClient& FRouteClient::SetConnectTarget(FString& host, int32 port)
{
	FIPv4Address ip;
	FIPv4Address::Parse(host, ip);

	InternetAddress = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->CreateInternetAddr();
	InternetAddress->SetIp(ip.Value);
	InternetAddress->SetPort(port);
	return *this;
}

void FRouteClient::Run()
{
	ShouldStop = false;
	while (!ShouldStop)
	{
		UE_LOG(LogTemp, Warning, TEXT("Connecting To %s ..."), *InternetAddress->ToString(true))
		if(!Connect())
		{
			UE_LOG(LogTemp, Warning, TEXT("Connect To %s Error..."), *InternetAddress->ToString(true))
		}else
		{
			if(OnRegist.IsBound())
			{
				OnRegist.Broadcast();
			}
			
			UE_LOG(LogTemp, Warning, TEXT("Connect To %s Error Success!!"), *InternetAddress->ToString(true))
			while(ClientSocket != nullptr && ClientSocket->GetConnectionState() == SCS_Connected)
			{

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
}

void FRouteClient::SendMessage()
{
}


void FRouteClient::Stop()
{
	ShouldStop = true;
}

bool FRouteClient::Connect()
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

void FRouteClient::WriteMessageBuffer()
{
}

void FRouteClient::ReadSocketBuffer()
{
}

void FRouteClient::DoHeartBeat()
{
	if(FPlatformTime::ToSeconds(FPlatformTime::Cycles() - LastUpdateTime)  >= HeartBeatSecond)
	{
		if(OnHeartBeat.IsBound())
		{
			OnHeartBeat.Broadcast();
		}
	}
}

bool FRouteClientRunner::Init()
{
	return true;
}

uint32 FRouteClientRunner::Run()
{
	Client.Run();
	return true;
}

void FRouteClientRunner::Stop()
{
	return;
}
