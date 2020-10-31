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
	void PushCommandFromClient(const FString& ReceivedString);
	void SendCommandToClient(const FString& SendContent) const;
	void SendCommandToClient(FString&& SendContent) const;
	void StopThread();

private:
	static FAutomaticManager* Instance;
	TSharedPtr<FAutomaticRunnable> Automatic;
	FCommandConsumer CommandConsumer;
};
