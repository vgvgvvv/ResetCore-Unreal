#pragma once

#include "Kismet/BlueprintFunctionLibrary.h"
#include "AutomaticPlatformHelper.generated.h"

UCLASS()
class REAUTOMATIC_API UAutomaticPlatformHelper : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable)
    static FString GetDeviceName();
};
