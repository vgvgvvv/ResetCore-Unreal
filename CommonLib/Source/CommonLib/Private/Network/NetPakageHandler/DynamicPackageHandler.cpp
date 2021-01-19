#include "Network/NetPackageHandler/DynamicPackageHandler.h"

FDynamicPackageHandler::FDynamicPackageHandler(TFunction<void(FSocketClient&, FNetPackage&)> handler)
{
	HandlerFunc = handler;
}

void FDynamicPackageHandler::HandlePackage(FSocketClient& from, FNetPackage& package)
{
	if(HandlerFunc)
	{
		HandlerFunc(from, package);
	}
}
