#include "ResourcesInfos/BaseAssetInfo.h"

#include "JsonObjectConverter.h"

TSharedPtr<FJsonObject> FBaseAssetInfoCreator::GetAssetInfo(UObject* AssetObject)
{
	return MakeShared<FJsonObject>();
}
