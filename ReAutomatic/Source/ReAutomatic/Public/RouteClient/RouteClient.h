#pragma once
#include "UnrealString.h"

class FRouteClient
{
public:
	void Start(const FString Name, const FString Host, const int32 Port);

	void Stop() const;
	
	~FRouteClient();

private:
	class FSocketClient* LocalClient = nullptr;
	class FLambdaRunnable* Runner = nullptr;
};
