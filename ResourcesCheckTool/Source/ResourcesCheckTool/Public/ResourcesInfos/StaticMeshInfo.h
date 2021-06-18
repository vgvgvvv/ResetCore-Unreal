#pragma once
#include "Engine/StaticMesh.h"
#include "StaticMeshInfo.generated.h"

USTRUCT()
struct RESOURCESCHECKTOOL_API FStaticMeshLODRuleItem
{
	GENERATED_USTRUCT_BODY()

    UPROPERTY(EditAnywhere, Category = CheckerParam)
	int VertexNumber;

	UPROPERTY(EditAnywhere, Category = CheckerParam)
	float ScreenSize;

	UPROPERTY(EditAnywhere, Category = CheckerParam)
	int TriangleNumber;

	UPROPERTY(EditAnywhere, Category = CheckerParam)
	int UVChannelNum;

};

USTRUCT()
struct RESOURCESCHECKTOOL_API FStaticMeshInfo
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = CheckerParam)
	int LODNumber;

	UPROPERTY(EditAnywhere, Category = CheckerParam)
	float CullingDistance;

	UPROPERTY(EditAnywhere, Category = CheckerParam)
	TArray<FStaticMeshLODRuleItem> LODRules;

	void Init(UStaticMesh* StaticMesh);
	
};
