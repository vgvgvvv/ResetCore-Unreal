#include "Network/NetPackageHandler/JsonPackageHandler.h"

#include "NetPackage.h"
#include "Serializer/FNetJsonSerializer.h"
#include "Utility/JsonUtil.h"
#include "CommonLib.h"



void FJsonPackageHandler::HandlePackage(FSocketClient& from, FNetPackage& package)
{
	auto result = package.GetValue<TSharedPtr<FJsonObject>, FNetJsonSerializer>();

	auto str = FJsonUtil::JsonObjectToString(result);

	UE_LOG(ResetCore_CommonLib, Log, TEXT("recv json %s"), *str);
	
}
