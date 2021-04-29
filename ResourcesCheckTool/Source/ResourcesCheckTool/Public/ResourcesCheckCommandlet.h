#pragma once

#include "Commandlets/Commandlet.h"

#include "ResourcesCheckCommandlet.generated.h"

UCLASS()
class UResourcesCheckCommandlet : public UCommandlet
{
	GENERATED_BODY()
public:

	UResourcesCheckCommandlet();
	virtual int32 Main(const FString& Params) override;
};
