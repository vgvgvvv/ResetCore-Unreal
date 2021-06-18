#pragma once
#include "Commandlets/Commandlet.h"
#include "TestCommandlet.generated.h"

UCLASS()
class UTestCommandlet : public UCommandlet
{
	GENERATED_BODY()
public:
	UTestCommandlet();

	virtual int32 Main(const FString& Params) override;
};
