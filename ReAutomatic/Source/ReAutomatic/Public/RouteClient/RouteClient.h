#pragma once
#include "JsonObject.h"
#include "LambdaRunnable.h"
#include "NetPackage.h"
#include "RouteProto.h"
#include "SharedPointer.h"
#include "UE4ControlCenter/IRouteClient.h"

enum class ERouteProtoID : uint8;

class REAUTOMATIC_API FRouteClient : public IRouteClient
{
public:

	bool InitWithFile();

	void Init(const FString& Name, const FString& Host, const int32 Port);

	FLambdaRunnable* RunWithBackground();

	void RunWithTick(float DeltaTime);
	
	void Stop() const;
	
	~FRouteClient();
public:

	virtual const FString& GetName() const override;
	
public:

	virtual void SendMessage(const FNetPackage& Package) override;


	
private:

	void OnRegister();
	void OnHeartBeat();
	
private:
	class FSocketClient* LocalClient = nullptr;
	class FLambdaRunnable* Runner = nullptr;
};
