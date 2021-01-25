#include "UE4ControlCenter/UE4ControlCenter.h"

#include "NetPackage.h"
#include "RouteClient.h"


FUE4ControlCenter* FUE4ControlCenter::Instance = nullptr;

FUE4ControlCenter& FUE4ControlCenter::Get()
{
	if(Instance == nullptr)
	{
		Instance = new FUE4ControlCenter;
	}
	return *Instance;
}

void FUE4ControlCenter::RegisterRouteClient(IRouteClient& RouteClient)
{
	RouteClients.AddUnique(&RouteClient);
}

void FUE4ControlCenter::RemoveRouteClient(IRouteClient& RouteClient)
{
	RouteClients.Remove(&RouteClient);
}

const IListener* FUE4ControlCenter::RegisterCmdEvent(const FName& CmdName, TFunction<void(IArg*)> Handler)
{
	return FEventDispatcher::CreateListener(CmdName)
	       .AddHandler(Handler)
	       .RegisterListener(UE4MsgEventDispatcher);
}

void FUE4ControlCenter::RemoveCmdEventLister(const IListener* Listener)
{
	UE4MsgEventDispatcher.RemoveListener(*Listener);
}

void FUE4ControlCenter::Trigger(const FName& EventName, IArg& Arg)
{
	UE4MsgEventDispatcher.TriggerEvent(EventName, Arg);
}

void FUE4ControlCenter::ClearAllCmd()
{
	UE4MsgEventDispatcher.Clear();
}


void FUE4ControlCenter::SendBackUE4MsgResult(const TSharedPtr<FJsonValue> Result,
	const FRemoteControllerInfo& ControllerInfo)
{
	for(auto& Client : RouteClients)
	{
		Client->SendMessage(FRouteClient::NetPackageFromUE4MsgResult(Result, ControllerInfo.ControllerName, ControllerInfo.ControllerType));
	}
}

void FUE4ControlCenter::SendBackRunLuaResult(const TSharedPtr<FJsonValue> Result,
	const FRemoteControllerInfo& ControllerInfo)
{
	for(auto& Client : RouteClients)
	{
		Client->SendMessage(FRouteClient::NetPackageFromLuaResult(Result, ControllerInfo.ControllerName, ControllerInfo.ControllerType));
	}
}
