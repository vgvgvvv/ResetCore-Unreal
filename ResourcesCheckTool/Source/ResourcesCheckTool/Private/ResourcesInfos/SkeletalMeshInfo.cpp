#include "ResourcesInfos/SkeletalMeshInfo.h"

#include "JsonObjectConverter.h"


void FSkeletalMeshCheckInfo::Init(const UObject* AssetObj)
{
	
}

TSharedPtr<FJsonObject> FSkeletalMeshInfoCreator::GetAssetInfo(UObject* AssetObject)
{
	FSkeletalMeshCheckInfo Result;
	Result.Init(AssetObject);
	return FJsonObjectConverter::UStructToJsonObject(Result);
}
