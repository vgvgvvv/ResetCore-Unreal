#pragma once
#include "BaseAssetInfo.h"
#include "SkeletalMeshInfo.generated.h"

USTRUCT()
struct RESOURCESCHECKTOOL_API FSkeletalMeshLODRuleItem
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, Category = CheckerParam)
	FString VertexNumber;

	UPROPERTY(EditAnywhere, Category = CheckerParam)
	FString ScreenSize;

};

USTRUCT()
struct RESOURCESCHECKTOOL_API FSkeletalMeshInfo : public FBaseAssetInfo
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, Category = CheckerParam)
	FString LODNumber;

	UPROPERTY(EditAnywhere, Category = CheckerParam)
	TArray<FSkeletalMeshLODRuleItem> LODRules;

	UPROPERTY(EditAnywhere, Category = CheckerParam)
	FString TriangleNumber;

	UPROPERTY(EditAnywhere, Category = CheckerParam)
	FString UVChannelNum;
};
