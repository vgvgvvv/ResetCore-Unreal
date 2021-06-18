#pragma once
#include "BaseAssetInfo.h"

#include "MaterialInfo.generated.h"

USTRUCT()
struct RESOURCESCHECKTOOL_API FMaterialInfo : public FBaseAssetInfo
{
	GENERATED_BODY()

	virtual void Init(const UObject* AssetObj) override;	
};

class RESOURCESCHECKTOOL_API FMaterialInfoCreator : public FBaseAssetInfoCreator
{
	public:
	virtual TSharedPtr<FJsonObject> GetAssetInfo(UObject* AssetObject) override;
};