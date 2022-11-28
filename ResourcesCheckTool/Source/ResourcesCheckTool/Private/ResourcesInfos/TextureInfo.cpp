#include "ResourcesInfos/TextureInfo.h"

#include "JsonObjectConverter.h"


void FTextureCheckInfo::Init(const UObject* AssetObj)
{
	auto Texture = Cast<UTexture2D>(AssetObj);
	TextureGroup = Texture->LODGroup;
    MaxSize = Texture->MaxTextureSize;
    CompressNoAlpha = Texture->CompressionNoAlpha;
    PowerOfTwo = FMath::IsPowerOfTwo((int)Texture->GetSurfaceWidth()) &&
    	FMath::IsPowerOfTwo((int)Texture->GetSurfaceHeight());
}

TSharedPtr<FJsonObject> FTextureInfoCreator::GetAssetInfo(UObject* AssetObject)
{
	FTextureCheckInfo Result;
	Result.Init(AssetObject);
	return FJsonObjectConverter::UStructToJsonObject(Result);
}
