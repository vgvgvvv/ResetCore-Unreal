#pragma once
#include "BaseAssetInfo.h"
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
struct RESOURCESCHECKTOOL_API FStaticMeshInfo : public FBaseAssetInfo
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = CheckerParam)
	int LODNumber;

	UPROPERTY(EditAnywhere, Category = CheckerParam)
	float CullingDistance;

	UPROPERTY(EditAnywhere, Category = CheckerParam)
	TArray<FStaticMeshLODRuleItem> LODRules;

	virtual void Init(const UObject* AssetObj) override;
};

class RESOURCESCHECKTOOL_API FStaticMeshInfoCreator : public FBaseAssetInfoCreator
{
	public:
	virtual TSharedPtr<FJsonObject> GetAssetInfo(UObject* AssetObject) override;
};