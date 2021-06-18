#include "ResourcesInfos/TextureInfo.h"


void FTextureInfo::Init(UTexture2D* Texture)
{
	TextureGroup = Texture->LODGroup;
	MaxSize = Texture->MaxTextureSize;
	CompressNoAlpha = Texture->CompressionNoAlpha;
	PowerOfTwo = FMath::IsPowerOfTwo((int)Texture->GetSurfaceWidth()) &&
		FMath::IsPowerOfTwo((int)Texture->GetSurfaceHeight());
}
