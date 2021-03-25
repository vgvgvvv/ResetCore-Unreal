#pragma once

#include "Dom/JsonObject.h"
#include "NetPackageHandler.h"
#include "Templates/SharedPointer.h"

class COMMONLIB_API FJsonPackageHandler : public INetPackageHandler
{
public:
	virtual void HandlePackage(FSocketClient& from, FNetPackage& package) override;

	virtual void HandleJsonInfo(FSocketClient& from, TSharedPtr<FJsonObject> jsonObject);
	
};
