#include "RouteClient/RouteClient.h"

#include "SocketClient.h"

void FRouteClient::Start()
{
	auto& Client = FSocketClient(TEXT("UE4Client"))
		.SetConnectTarget(TEXT("121.196.216.106"), 8000);

	Client.OnHeartBeat.AddLambda([]()
	{
		
	});
		
}
