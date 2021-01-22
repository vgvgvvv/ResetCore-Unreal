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
	       .RegisterListener(EventDispatcher);
}

void FUE4ControlCenter::RemoveCmdEventLister(const IListener* Listener)
{
	EventDispatcher.RemoveListener(*Listener);
}

void FUE4ControlCenter::Trigger(const FName& EventName, IArg& Arg)
{
	EventDispatcher.TriggerEvent(EventName, Arg);
}

void FUE4ControlCenter::ClearAllCmd()
{
	EventDispatcher.Clear();
}


void FUE4ControlCenter::SendBackUE4MsgResult(int32 Index, const TSharedPtr<FJsonObject> Result,
	const FRemoteControllerInfo& ControllerInfo)
{
	for(auto& Client : RouteClients)
	{
		Client->SendMessage(FRouteClient::NetPackageFromUE4MsgResult(Index, Result, ControllerInfo.ControllerName, ControllerInfo.ControllerType));
	}
}

void FUE4ControlCenter::SendBackRunLuaResult(int32 Index, const TSharedPtr<FJsonObject> Result,
	const FRemoteControllerInfo& ControllerInfo)
{
	for(auto& Client : RouteClients)
	{
		Client->SendMessage(FRouteClient::NetPackageFromLuaResult(Index, Result, ControllerInfo.ControllerName, ControllerInfo.ControllerType));
	}
}
