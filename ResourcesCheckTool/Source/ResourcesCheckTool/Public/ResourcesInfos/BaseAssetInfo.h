#pragma once
#include "JsonObject.h"
#include "BaseAssetInfo.generated.h"


USTRUCT()
struct FBaseAssetInfo
{
public:
	GENERATED_BODY()

	virtual ~FBaseAssetInfo() = default;
	
	virtual void Init(const UObject* AssetObj) {};
};

class RESOURCESCHECKTOOL_API FBaseAssetInfoCreator
{
	public:
	virtual ~FBaseAssetInfoCreator() = default;
	virtual TSharedPtr<FJsonObject> GetAssetInfo(UObject* AssetObject);
};
