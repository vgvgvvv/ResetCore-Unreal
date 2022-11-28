#pragma once
#include "ResourcesInfos/StaticMeshInfo.h"

#include "JsonObjectConverter.h"

void FStaticMeshCheckInfo::Init(const UObject* AssetObj)
{
	auto StaticMesh = Cast<UStaticMesh>(AssetObj);
	LODNumber = StaticMesh->GetNumLODs();

}

TSharedPtr<FJsonObject> FStaticMeshInfoCreator::GetAssetInfo(UObject* AssetObject)
{
	FStaticMeshCheckInfo Result;
	Result.Init(AssetObject);
	return FJsonObjectConverter::UStructToJsonObject(Result);
}
