#pragma once
#include "JsonObject.h"
#include "Event/Listener.h"
#include "Queue.h"
#include "Tickable.h"

struct REAUTOMATIC_API FCommand : CommonLib::IArg
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

struct REAUTOMATIC_API FJsonArg : CommonLib::IArg
{
	TSharedRef<FJsonObject> JsonObject;
};

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
	bool ParseCommandString(const FString& CommandStr, FCommand& Command ) const;
	static void SendCommandToClient(CommonLib::IArg* Command);
	static FString GetSendString(const FJsonArg& Arg);
private:

	bool IsEnable = false;
	TQueue<FCommand> CommandQueue;

};
