#pragma once
#include "RouteProto.h"

// 用于抽象控制器，包括Route控制器以及本地直连控制器
class REAUTOMATIC_API IRouteClient
{
public:
	virtual ~IRouteClient() = default;
	virtual const FString& GetName() const = 0;
	virtual void SendMessage(const struct FNetPackage& Package) = 0;

	friend bool operator==(const IRouteClient& Lhs, const IRouteClient& RHS)
	{
		return Lhs.GetName() == RHS.GetName();
	}

	friend bool operator!=(const IRouteClient& Lhs, const IRouteClient& RHS)
	{
		return !(Lhs == RHS);
	}

public:
	static FNetPackage NetPackageFromJsonObject(TSharedPtr<FJsonObject> JsonObject);
	static FNetPackage NetPackageFromProtoIDMessage(ERouteProtoID ProtoID, TSharedPtr<FJsonObject> Content);
	static FNetPackage NetPackageFromRouteMessage(const FRawCommandMessage& CommandMessage, const FString& TargetName, const ERouteType TargetType);

	static FNetPackage NetPackageFromLogMessage(const FString& Log, const FString& TargetName, const ERouteType TargetType);

	static FNetPackage NetPackageFromLuaResult(int32 Index, const TSharedPtr<FJsonValue> Result, const FString& TargetName, const ERouteType TargetType);
	static FNetPackage NetPackageFromUE4MsgResult(int32 Index, const TSharedPtr<FJsonValue> Result, const FString& TargetName, const ERouteType TargetType);
	static FNetPackage NetPackageFromLuaResult(int32 Index, const TSharedPtr<FJsonValue> Result, const FRemoteControllerInfo& ControllerInfo);
	static FNetPackage NetPackageFromUE4MsgResult(int32 Index, const TSharedPtr<FJsonValue> Result, const FRemoteControllerInfo& ControllerInfo);
	
};
