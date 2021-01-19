#include "Network/NetPackageHandler/JsonPackageHandler.h"


#include "NetPackage.h"
#include "Serializer/FNetJsonSerializer.h"

void FJsonPackageHandler::HandlePackage(FSocketClient& from, FNetPackage& package)
{
	auto result = package.GetValue<TSharedPtr<FJsonObject>, FNetJsonSerializer>();
}
