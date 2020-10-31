#pragma once
#include "JsonObject.h"
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

struct FJsonArg : CommonLib::IArg
{
	TSharedRef<FJsonObject> JsonObject;
};

class FCommandConsumer : public FTickableGameObject
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
	bool ParseCommandString(const FString& CommandStr, FCommand& Command ) const;
	void SendCommandToClient(const FJsonArg& Command) const;
	FString GetSendString(const FJsonArg& Arg) const;
private:

	bool IsEnable = false;
	TQueue<FCommand> CommandQueue;

};
