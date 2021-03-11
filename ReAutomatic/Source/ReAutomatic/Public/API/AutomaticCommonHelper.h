#pragma once
#include "Kismet/BlueprintFunctionLibrary.h"

#include "AutomaticCommonHelper.generated.h"

UCLASS()
class UAutomaticCommonHelper : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable)
	static bool IsClassOf(const UObject* Object, const UClass* Class);
};
