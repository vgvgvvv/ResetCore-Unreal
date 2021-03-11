#pragma once
#include "UE4ControlCenter/IRouteClient.h"

enum class ERouteProtoID : uint8;

class REAUTOMATIC_API FRouteClient : public IRouteClient
{
public:

	bool InitWithFile();

	bool Init(const FString& Name, const FString& Host, const int32 Port);

	class FLambdaRunnable* RunWithBackground();

	void RunWithTick(float DeltaTime);
	
	void Stop() const;
	
	~FRouteClient();
public:

	virtual const FString& GetName() const override;
	
public:

	virtual void SendMessage(const FNetPackage& Package) override;


	
private:

	void OnRegister() const;
	void OnHeartBeat() const;
	
private:
	class FSocketClient* LocalClient = nullptr;
	class FLambdaRunnable* Runner = nullptr;
};
