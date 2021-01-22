// #pragma once
//
// #include "HAL/Runnable.h"
// #include "AutomaticServer.h"
// #include "CommandConsumer.h"
//
//
// class FAutomaticRunnable : public FRunnable
// {
// public:
// 	FAutomaticRunnable();
//
//
// 	bool Init() override;
// 	uint32 Run() override;
// 	void Stop() override;
// 	void Exit() override;
//
// public:
//
// 	void RequestStop();
// 	TSharedPtr<FAutomaticServer> GetServer() const;
//
// private:
//
// 	TSharedPtr<FAutomaticServer> Server;
// 	bool ShouldStop;
//
// };
