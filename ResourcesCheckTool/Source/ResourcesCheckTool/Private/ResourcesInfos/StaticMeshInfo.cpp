#pragma once
#include "ResourcesInfos/StaticMeshInfo.h"

void FStaticMeshInfo::Init(const UObject* AssetObj)
{
	auto StaticMesh = Cast<UStaticMesh>(AssetObj);
	LODNumber = StaticMesh->GetNumLODs();

}
