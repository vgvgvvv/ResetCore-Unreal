#pragma once
#include "Network/NetPackageHandler/JsonPackageHandler.h"


class FRouteMessageHandler : public INetPackageHandler
{
public:
	virtual void HandlePackage(FSocketClient& from, FNetPackage& package) override;

	void HandleJsonInfo(FSocketClient& from, TSharedPtr<FJsonObject> jsonObject);

};
