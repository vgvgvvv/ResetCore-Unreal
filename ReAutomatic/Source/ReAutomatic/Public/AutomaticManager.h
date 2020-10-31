#pragma once

#include "CoreMinimal.h"
#include "Runnable.h"
#include "AutomaticRunnable.h"

class FAutomaticManager
{
public:

	FAutomaticManager();
	
	static FAutomaticManager& Get();
	
	void StartThread(int32 port);
	void StopThread();

private:
	static FAutomaticManager* Instance;
	TSharedPtr<FAutomaticRunnable> Automatic;

};
