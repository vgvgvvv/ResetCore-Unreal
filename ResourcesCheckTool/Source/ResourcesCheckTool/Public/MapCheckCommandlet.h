#pragma once
#include "Commandlets/Commandlet.h"
#include "MapCheckCommandlet.generated.h"

UCLASS()
class RESOURCESCHECKTOOL_API UMapCheckCommandlet : public UCommandlet
{
	GENERATED_BODY()
public:

	UMapCheckCommandlet();
	virtual int32 Main(const FString& Params) override;

	TArray<FString> GetAllMapNames();
};
