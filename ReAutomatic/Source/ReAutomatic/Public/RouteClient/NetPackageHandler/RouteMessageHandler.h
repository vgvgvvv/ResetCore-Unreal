#pragma once
#include "Network/NetPackageHandler/JsonPackageHandler.h"


class FRouteMessageHandler : public FJsonPackageHandler
{
public:

	virtual void HandleJsonInfo(FSocketClient& from, TSharedPtr<FJsonObject> jsonObject) override;

};
