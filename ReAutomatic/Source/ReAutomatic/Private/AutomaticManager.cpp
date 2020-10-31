#include "AutomaticManager.h"
#include "RunnableThread.h"
#include "ReAutomatic.h"

FAutomaticManager* FAutomaticManager::Instance = nullptr;
FRunnableThread* AutomaticThread = nullptr;

FAutomaticManager::FAutomaticManager()
{
	Instance = this;
}


FAutomaticManager& FAutomaticManager::Get()
{
	if(Instance == nullptr)
	{
		Instance = new FAutomaticManager;
	}
	return *Instance;
}

void FAutomaticManager::StartThread(int32 port)
{
	UE_LOG(LogAutomatic, Log, TEXT("Start Automatic Thread"));
	Automatic = MakeShared<FAutomaticRunnable>();
	AutomaticThread = FRunnableThread::Create(Automatic.Get(), TEXT("Automatic Thread"));
}

void FAutomaticManager::StopThread()
{
	UE_LOG(LogAutomatic, Log, TEXT("Stop Automatic Thread"));

	if (AutomaticThread)
	{
		Automatic->RequestStop();
		AutomaticThread->WaitForCompletion();
	}
}