#pragma once
#include "EventDispatcher.h"
#include "Listener.h"

class FJsonValue;

struct REAUTOMATIC_API FUE4CmdArg : CommonLib::IArg
{
	FUE4CmdArg();

	FUE4CmdArg(const FString& CmdId, TSharedPtr<FJsonValue> JSON);

	FString CmdId;
	TSharedPtr<FJsonValue> Content;
};

class REAUTOMATIC_API FUE4ControlCenter
{
public:
	
	const CommonLib::IListener* RegisterCmdEvent(const FName& CmdName, TFunction<void(CommonLib::IArg*)> Handler);
	void RemoveCmdEventLister(const CommonLib::IListener* Listener);

	void Trigger(const FName& EventName, CommonLib::IArg& Arg);

private:
	CommonLib::FEventDispatcher EventDispatcher;
public:
	static FUE4ControlCenter& Get();
private:
	static FUE4ControlCenter* Instance;
};
