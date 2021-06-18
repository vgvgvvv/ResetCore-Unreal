#pragma once
#include "ResourcesInfos/StaticMeshInfo.h"

void FStaticMeshInfo::Init(UStaticMesh* StaticMesh)
{
	this->LODNumber = StaticMesh->GetNumLODs();
}
