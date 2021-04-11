#include "Network/NetPackageHandler/JsonPackageHandler.h"

#include "Network/NetPackage.h"
#include "Network/Serializer/FNetJsonSerializer.h"
#include "Utility/JsonUtil.h"
#include "CommonLib.h"


void FJsonPackageHandler::HandlePackage(FSocketClient& from, FNetPackage& package)
{
	const auto result = package.GetValue<TSharedPtr<FJsonObject>, FNetJsonSerializer>();
	HandleJsonInfo(from, result);	
}

void FJsonPackageHandler::HandleJsonInfo(FSocketClient& from, TSharedPtr<FJsonObject> jsonObject)
{
	UE_LOG(ResetCore_CommonLib, Log, TEXT("recv json %s"), *FJsonUtil::JsonObjectToString(jsonObject));
}
