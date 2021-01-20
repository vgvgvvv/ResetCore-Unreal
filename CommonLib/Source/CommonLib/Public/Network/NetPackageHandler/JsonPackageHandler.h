#pragma once

#include "JsonObject.h"
#include "NetPackageHandler.h"
#include "SharedPointer.h"

class COMMONLIB_API FJsonPackageHandler : public INetPackageHandler
{
public:
	virtual void HandlePackage(FSocketClient& from, FNetPackage& package) override;

	virtual void HandleJsonInfo(FSocketClient& from, TSharedPtr<FJsonObject> jsonObject);
	
};
