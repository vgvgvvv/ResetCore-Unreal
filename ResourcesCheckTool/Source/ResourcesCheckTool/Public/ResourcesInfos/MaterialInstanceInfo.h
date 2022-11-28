#pragma once
#include "BaseAssetInfo.h"
#include "MaterialInstanceInfo.generated.h"


USTRUCT()
struct RESOURCESCHECKTOOL_API FMaterialInstanceBoolValue
{
	GENERATED_BODY()

    UPROPERTY(EditAnywhere, Category = CheckerParam)
	FString PropertyName;

	UPROPERTY(EditAnywhere, Category = CheckerParam)
	bool BoolValue;

};

USTRUCT()
struct RESOURCESCHECKTOOL_API FMaterialInstanceNumberValue
{
	GENERATED_BODY()

    UPROPERTY(EditAnywhere, Category = CheckerParam)
	FString PropertyName;

	UPROPERTY(EditAnywhere, Category = CheckerParam)
	float MaxValue;

	UPROPERTY(EditAnywhere, Category = CheckerParam)
	float MinValue;

};

USTRUCT()
struct RESOURCESCHECKTOOL_API FMaterialInstanceAssetCheckInfo : public FBaseAssetInfo
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, Category = CheckerParam)
	FString ParentMaterialRegex;

	UPROPERTY(EditAnywhere, Category = CheckerParam)
	TArray<FString> MustHaveTextureArray;

	UPROPERTY(EditAnywhere, Category = CheckerParam)
	TArray<FMaterialInstanceNumberValue> LimitNumPropertyArray;

	UPROPERTY(EditAnywhere, Category = CheckerParam)
	TArray<FMaterialInstanceBoolValue> LimitBoolPropertyArray;
	
	virtual void Init(const UObject* AssetObj) override;
};

class RESOURCESCHECKTOOL_API FMaterialInstanceInfoCreator : public FBaseAssetInfoCreator
{
public:
	virtual TSharedPtr<FJsonObject> GetAssetInfo(UObject* AssetObject) override;
};


