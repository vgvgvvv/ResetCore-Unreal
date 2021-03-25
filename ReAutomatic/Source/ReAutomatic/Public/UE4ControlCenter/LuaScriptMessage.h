#pragma once

#include "Containers/UnrealString.h"
#include "LuaScriptMessage.generated.h"

USTRUCT()
struct FLuaScriptMessage
{
	GENERATED_BODY()

	UPROPERTY()
	int32 Index;

	UPROPERTY()
	FString LuaScript;
};
