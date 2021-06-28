#pragma once
#include "ResourcesInfos/StaticMeshInfo.h"

#include "JsonObjectConverter.h"

void FStaticMeshInfo::Init(const UObject* AssetObj)
{
	auto StaticMesh = Cast<UStaticMesh>(AssetObj);
	LODNumber = StaticMesh->GetNumLODs();

}

TSharedPtr<FJsonObject> FStaticMeshInfoCreator::GetAssetInfo(UObject* AssetObject)
{
	FStaticMeshInfo Result;
	Result.Init(AssetObject);
	return FJsonObjectConverter::UStructToJsonObject(Result);
}
