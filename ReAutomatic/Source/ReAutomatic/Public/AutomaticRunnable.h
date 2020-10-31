#pragma once

#include "HAL/Runnable.h"
#include "AutomaticServer.h"


class FAutomaticRunnable : public FRunnable
{
public:
	FAutomaticRunnable();


	bool Init() override;
	uint32 Run() override;
	void Stop() override;
	void Exit() override;

public:

	void RequestStop();
	
private:

	TSharedPtr<FAutomaticServer> Server;
	bool ShouldStop;
	
};