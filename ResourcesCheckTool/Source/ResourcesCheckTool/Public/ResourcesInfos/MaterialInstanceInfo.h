#pragma once
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
struct RESOURCESCHECKTOOL_API FMaterialInstanceInfo
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
};