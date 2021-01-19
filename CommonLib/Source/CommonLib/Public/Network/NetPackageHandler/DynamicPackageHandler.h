#pragma once
#include "Function.h"
#include "NetPackageHandler.h"

class COMMONLIB_API FDynamicPackageHandler : public INetPackageHandler
{
public:

	explicit FDynamicPackageHandler(TFunction<void(FSocketClient&, FNetPackage&)> handler);
	
	virtual void HandlePackage(FSocketClient& from, FNetPackage& package) override;

private:
	TFunction<void(FSocketClient&, FNetPackage&)> HandlerFunc;
};
