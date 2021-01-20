#pragma once
#include "JsonObject.h"
#include "LambdaRunnable.h"
#include "NetPackage.h"
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

	static FNetPackage MakeFromJsonObject(TSharedPtr<FJsonObject> JsonObject);
	static FNetPackage MakeFromProtoIDMessage(ERouteProtoID ProtoID, TSharedPtr<FJsonObject> Content);
	static FNetPackage MakeFromRouteMessage(const FRawCommandMessage& CommandMessage, const FString& TargetName, const ERouteType TargetType);
	
private:
	class FSocketClient* LocalClient = nullptr;
	class FLambdaRunnable* Runner = nullptr;
};
