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

	UFUNCTION()
	static TArray<struct FAssetData> GetAllMapAssets(const FString& MapRoot);

	UFUNCTION()
	static bool LoadLevel(const FString& LevelToLoad);

	UFUNCTION()
	static FVector GetLevelBoundsExtend();
};
