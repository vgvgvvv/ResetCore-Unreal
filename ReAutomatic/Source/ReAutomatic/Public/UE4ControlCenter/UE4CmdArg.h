#pragma once
#include "Listener.h"
#include "RouteProto.h"

class FJsonValue;

struct REAUTOMATIC_API FUE4CmdArg : public IArg
{
	FString CmdId;
	TSharedPtr<FJsonValue> Content;
	FRemoteControllerInfo ControllerInfo;
};
