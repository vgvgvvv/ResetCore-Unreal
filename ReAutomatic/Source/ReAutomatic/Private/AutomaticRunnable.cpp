#include "AutomaticRunnable.h"
#include "CoreMinimal.h"
#include "ReAutomatic.h"
#include "RunnableThread.h"


FAutomaticRunnable::FAutomaticRunnable()
	: ShouldStop(false)
{
}


bool FAutomaticRunnable::Init()
{
	ShouldStop = false;
	Server = MakeShared<FAutomaticServer>();
	Server->Init(TEXT("127.0.0.1"), 44444, TEXT("AutomaticRunnable"));
	return true;
}

uint32 FAutomaticRunnable::Run()
{
	while(ShouldStop || !Server.IsValid())
	{
		Server->WaitForConnect();
		Server->Update();
		FPlatformProcess::Sleep(0.1f);

		if(Server->GetCurrentStatus() == FAutomaticServer::EStatus::Stop)
		{
			break;
		}
	}
	return 0;
}

void FAutomaticRunnable::Stop()
{
	
}

void FAutomaticRunnable::Exit()
{
	Server->Stop();
}


void FAutomaticRunnable::RequestStop()
{
	ShouldStop = true;
}
