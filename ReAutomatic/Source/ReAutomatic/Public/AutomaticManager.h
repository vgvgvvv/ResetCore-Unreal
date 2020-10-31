#pragma once

#include "CoreMinimal.h"
#include "Runnable.h"
#include "AutomaticRunnable.h"
#include "CommandConsumer.h"

class FAutomaticManager
{
public:

	FAutomaticManager();

	static FAutomaticManager& Get();

	void StartThread(int32 port);
	void PushCommand(FString ReceivedString);
	void StopThread();

private:
	static FAutomaticManager* Instance;
	TSharedPtr<FAutomaticRunnable> Automatic;
	FCommandConsumer CommandConsumer;
};
