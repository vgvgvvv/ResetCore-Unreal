#pragma once
#include "Event/EventDispatcher.h"
#include "Event/Listener.h"

class FJsonObject;
struct FRemoteControllerInfo;
struct FLuaScriptMessage;
class FJsonValue;

DECLARE_MULTICAST_DELEGATE_TwoParams(FUE4ControlRunLuaDelegate, const FLuaScriptMessage&, const struct FRemoteControllerInfo&);
DECLARE_MULTICAST_DELEGATE_TwoParams(FUE4ControlUE4MsgTriggerDelegate, const FName&, IArg*);

class REAUTOMATIC_API FUE4ControlCenter
{
public:

	// 注册多个控制本端的控制器
	void RegisterRouteClient(class IRouteClient& RouteClient);
	void RemoveRouteClient(class IRouteClient& RouteClient);

	// UE4 Message
	FUE4ControlUE4MsgTriggerDelegate OnUE4Msg;
	const IListener* RegisterCmdEvent(const FName& CmdName, TFunction<void(IArg*)> Handler);
	void RemoveCmdEventLister(const IListener* Listener);
	void Trigger(const FName& EventName, IArg& Arg);
	void ClearAllCmd();
	void SendBackUE4MsgResult(int32 Index, const TSharedPtr<FJsonObject> Result, const FRemoteControllerInfo& ControllerInfo);

	// Run Lua
	FUE4ControlRunLuaDelegate OnRunLua;
	void SendBackRunLuaResult(int32 Index, const TSharedPtr<FJsonObject> Result, const FRemoteControllerInfo& ControllerInfo);
	
private:
	FEventDispatcher UE4MsgEventDispatcher;
public:
	static FUE4ControlCenter& Get();
private:
	static FUE4ControlCenter* Instance;
	TArray<IRouteClient*> RouteClients;
};
