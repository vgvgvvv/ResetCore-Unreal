#pragma once
#include "Listener.h"
#include "RouteProto.h"

class FJsonValue;

struct REAUTOMATIC_API FUE4CmdArg : public IArg
{
	FString CmdId;
	int32 Index;
	TSharedPtr<FJsonValue> Content;
	FRemoteControllerInfo ControllerInfo;
};
