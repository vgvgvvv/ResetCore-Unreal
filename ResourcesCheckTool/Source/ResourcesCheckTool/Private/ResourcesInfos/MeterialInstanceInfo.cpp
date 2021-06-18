#include "JsonObjectConverter.h"
#include "ResourcesInfos/MaterialInstanceInfo.h"

void FMaterialInstanceInfo::Init(const UObject* AssetObj)
{
	
}


TSharedPtr<FJsonObject> FMaterialInstanceInfoCreator::GetAssetInfo(UObject* AssetObject)
{
	FMaterialInstanceInfo Result;
	Result.Init(AssetObject);
	return FJsonObjectConverter::UStructToJsonObject(Result);
}