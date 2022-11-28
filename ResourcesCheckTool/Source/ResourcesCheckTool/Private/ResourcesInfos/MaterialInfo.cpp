#include "ResourcesInfos/MaterialInfo.h"

#include "JsonObjectConverter.h"


void FMaterialCheckInfo::Init(const UObject* AssetObj)
{
}

TSharedPtr<FJsonObject> FMaterialInfoCreator::GetAssetInfo(UObject* AssetObject)
{
	FMaterialCheckInfo Result;
	Result.Init(AssetObject);
	return FJsonObjectConverter::UStructToJsonObject(Result);
}
