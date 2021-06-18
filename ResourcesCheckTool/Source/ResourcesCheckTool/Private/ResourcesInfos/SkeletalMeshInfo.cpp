#include "ResourcesInfos/SkeletalMeshInfo.h"

#include "JsonObjectConverter.h"


void FSkeletalMeshInfo::Init(const UObject* AssetObj)
{
	
}

TSharedPtr<FJsonObject> FSkeletalMeshInfoCreator::GetAssetInfo(UObject* AssetObject)
{
	FSkeletalMeshInfo Result;
	Result.Init(AssetObject);
	return FJsonObjectConverter::UStructToJsonObject(Result);
}
