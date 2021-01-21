#include "UE4ControlCenter/UE4ControlCenter.h"

FUE4CmdArg::FUE4CmdArg(): FUE4CmdArg(TEXT(""), TSharedPtr<FJsonValue>())
{
}

FUE4CmdArg::FUE4CmdArg(const FString& CmdId, TSharedPtr<FJsonValue> JSON)
	: CmdId(CmdId)
    , Content(JSON)
{
}

const CommonLib::IListener* FUE4ControlCenter::RegisterCmdEvent(const FName& CmdName, TFunction<void(CommonLib::IArg*)> Handler)
{
	return CommonLib::FEventDispatcher::CreateListener(CmdName)
	       .AddHandler(Handler)
	       .RegisterListener(EventDispatcher);
}

void FUE4ControlCenter::RemoveCmdEventLister(const CommonLib::IListener* Listener)
{
	EventDispatcher.RemoveListener(*Listener);
}

void FUE4ControlCenter::Trigger(const FName& EventName, CommonLib::IArg& Arg)
{
	EventDispatcher.TriggerEvent(EventName, Arg);
}

FUE4ControlCenter* FUE4ControlCenter::Instance = nullptr;

FUE4ControlCenter& FUE4ControlCenter::Get()
{
	if(Instance == nullptr)
	{
		Instance = new FUE4ControlCenter;
	}
	return *Instance;
}