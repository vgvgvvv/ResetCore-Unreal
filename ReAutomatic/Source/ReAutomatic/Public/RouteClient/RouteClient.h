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

	bool InitWithFile();

	void Init(const FString& Name, const FString& Host, const int32 Port);

	FLambdaRunnable* RunWithBackground();

	void RunWithTick(float DeltaTime);
	
	void Stop() const;
	
	~FRouteClient();

public:

	static FNetPackage NetPackageFromJsonObject(TSharedPtr<FJsonObject> JsonObject);
	static FNetPackage NetPackageFromProtoIDMessage(ERouteProtoID ProtoID, TSharedPtr<FJsonObject> Content);
	static FNetPackage NetPackageFromRouteMessage(const FRawCommandMessage& CommandMessage, const FString& TargetName, const ERouteType TargetType);

	static FNetPackage NetPackageFromLogMessage(const FString& Log, const FString& TargetName, const ERouteType TargetType);
	
private:
	class FSocketClient* LocalClient = nullptr;
	class FLambdaRunnable* Runner = nullptr;
};
