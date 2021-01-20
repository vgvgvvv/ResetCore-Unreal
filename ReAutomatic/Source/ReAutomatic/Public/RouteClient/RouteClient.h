#pragma once
#include "JsonObject.h"
#include "LambdaRunnable.h"
#include "RouteProto.h"
#include "SharedPointer.h"
#include "UnrealString.h"

enum class ERouteProtoID : uint8;

class REAUTOMATIC_API FRouteClient
{
public:
	void Init(const FString Name, const FString Host, const int32 Port);

	FLambdaRunnable* RunWithBackground();

	void RunWithTick(float DeltaTime);
	
	void Stop() const;
	
	~FRouteClient();

public:

	void SendMessage(TSharedPtr<FJsonObject> JsonObject) const;
	void SendProtoIDMessage(ERouteProtoID ProtoID,TSharedPtr<FJsonObject> JsonObject) const;
	void SendRouteMessage(const FRawCommandMessage& CommandMessage, const FString& TargetName, const ERouteType TargetType) const;

private:
	class FSocketClient* LocalClient = nullptr;
	class FLambdaRunnable* Runner = nullptr;
};
