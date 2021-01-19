#pragma once

#include "NetPackageHandler.h"

class FJsonPackageHandler : public INetPackageHandler
{
public:
	virtual void HandlePackage(FSocketClient& from, FNetPackage& package) override;
};
