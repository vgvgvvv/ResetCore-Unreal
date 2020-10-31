#pragma once
#include "Listener.h"
#include "Queue.h"
#include "Tickable.h"

struct FCommand : CommonLib::IArg
{
	FCommand()
		: FCommand(0, "")
	{}

	FCommand(int CmdId, FString CmdStr)
		: CmdId(CmdId)
		, CmdStr(CmdStr)
	{}

	int CmdId;
	FString CmdStr;
};

class FCommandConsumer : public FTickableGameObject
{
public:

	void SetEnable(bool Enable);
	void PushCommand(FString ReceivedStr);


	virtual void Tick(float DeltaTime) override;
	virtual bool IsTickable() const override;
	virtual TStatId GetStatId() const override;

private:
	bool ParseCommandString(FString CommandStr, FCommand& Command ) const;

private:

	bool IsEnable = false;
	TQueue<FCommand> CommandQueue;

};
