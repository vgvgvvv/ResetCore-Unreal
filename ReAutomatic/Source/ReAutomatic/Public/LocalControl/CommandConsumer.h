#pragma once
#include "EventDispatcher.h"
#include "Event/Listener.h"
#include "Queue.h"
#include "Tickable.h"


struct FUE4CmdArg;

class REAUTOMATIC_API FCommandConsumer : public FTickableGameObject
{
public:

	FCommandConsumer();
	~FCommandConsumer();

	void SetEnable(bool Enable);
	void PushCommand(FString ReceivedStr);


	virtual void Tick(float DeltaTime) override;
	virtual bool IsTickable() const override;
	virtual TStatId GetStatId() const override;

private:
	bool ParseCommandString(const FString& CommandStr, FUE4CmdArg& Command ) const;
	static void SendCommandToClient(CommonLib::IArg* Command);
	static FString GetSendString(const FUE4CmdArg& Arg);
private:

	bool IsEnable = false;
	TQueue<FUE4CmdArg> JsonQueue;

	CommonLib::FEventDispatcher EventDispatcher;

};
