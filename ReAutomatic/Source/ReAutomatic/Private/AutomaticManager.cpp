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

void FAutomaticManager::StartThread()
{
	UE_LOG(LogAutomatic, Log, TEXT("Start Automatic Thread"));

	Automatic = MakeShared<FAutomaticRunnable>();
	AutomaticThread = FRunnableThread::Create(Automatic.Get(), TEXT("Automatic Thread"));
	CommandConsumer.SetEnable(true);

}


void FAutomaticManager::PushCommandFromClient(const FString& ReceivedString)
{
	CommandConsumer.PushCommand(ReceivedString);
}

void FAutomaticManager::SendCommandToClient(const FString& SendContent) const
{
	if(Automatic.IsValid() && Automatic->GetServer().IsValid())
	{
		Automatic->GetServer()->SendMessage(SendContent);
	}
}

void FAutomaticManager::SendCommandToClient(FString&& SendContent) const
{
	if(Automatic.IsValid() && Automatic->GetServer().IsValid())
	{
		Automatic->GetServer()->SendMessage(MoveTemp(SendContent));
	}
}

void FAutomaticManager::StopThread()
{
	UE_LOG(LogAutomatic, Log, TEXT("Stop Automatic Thread"));

	if (Automatic.IsValid() && AutomaticThread)
	{
		Automatic->RequestStop();
		AutomaticThread->WaitForCompletion();
	}

	CommandConsumer.SetEnable(false);
}
