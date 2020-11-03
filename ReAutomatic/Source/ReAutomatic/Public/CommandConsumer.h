#pragma once
#include "JsonObject.h"
#include "Event/Listener.h"
#include "Queue.h"
#include "Tickable.h"

struct REAUTOMATIC_API FJsonArg : CommonLib::IArg
{
	FJsonArg()
		: FJsonArg(0, TSharedPtr<FJsonObject>())
	{}

	FJsonArg(int CmdId, TSharedPtr<FJsonObject> json)
		: CmdId(CmdId)
		, JsonObject(json)
	{}


	FJsonArg(const FJsonArg& Other)
		: CommonLib::IArg(Other),
		  CmdId(Other.CmdId),
		  JsonObject(Other.JsonObject)
	{
	}

	FJsonArg(FJsonArg&& Other) noexcept
		: CommonLib::IArg(std::move(Other)),
		  CmdId(Other.CmdId),
		  JsonObject(Other.JsonObject)
	{
	}

	FJsonArg& operator=(const FJsonArg& Other)
	{
		if (this == &Other)
			return *this;
		CommonLib::IArg::operator =(Other);
		CmdId = Other.CmdId;
		JsonObject = Other.JsonObject;
		return *this;
	}

	FJsonArg& operator=(FJsonArg&& Other) noexcept
	{
		if (this == &Other)
			return *this;
		CommonLib::IArg::operator =(std::move(Other));
		CmdId = Other.CmdId;
		JsonObject = Other.JsonObject;
		return *this;
	}

	int CmdId;
	TSharedPtr<FJsonObject> JsonObject;
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
	bool ParseCommandString(const FString& CommandStr, FJsonArg& Command ) const;
	static void SendCommandToClient(CommonLib::IArg* Command);
	static FString GetSendString(const FJsonArg& Arg);
private:

	bool IsEnable = false;
	TQueue<FJsonArg> JsonQueue;

};
