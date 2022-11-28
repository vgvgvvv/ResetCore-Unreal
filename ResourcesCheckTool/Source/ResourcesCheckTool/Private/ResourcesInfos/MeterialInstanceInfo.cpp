#include "JsonObjectConverter.h"
#include "ResourcesInfos/MaterialInstanceInfo.h"

void FMaterialInstanceAssetCheckInfo::Init(const UObject* AssetObj)
{
	
}


TSharedPtr<FJsonObject> FMaterialInstanceInfoCreator::GetAssetInfo(UObject* AssetObject)
{
	FMaterialInstanceAssetCheckInfo Result;
	Result.Init(AssetObject);
	return FJsonObjectConverter::UStructToJsonObject(Result);
}