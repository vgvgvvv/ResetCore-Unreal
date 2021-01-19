#pragma once

#include "NetPackageHandler.h"

class COMMONLIB_API FJsonPackageHandler : public INetPackageHandler
{
public:
	virtual void HandlePackage(FSocketClient& from, FNetPackage& package) override;
	
};
