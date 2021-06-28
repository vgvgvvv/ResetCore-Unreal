#include "ResourcesInfos/MaterialInfo.h"

#include "JsonObjectConverter.h"


void FMaterialInfo::Init(const UObject* AssetObj)
{
}

TSharedPtr<FJsonObject> FMaterialInfoCreator::GetAssetInfo(UObject* AssetObject)
{
	FMaterialInfo Result;
	Result.Init(AssetObject);
	return FJsonObjectConverter::UStructToJsonObject(Result);
}
