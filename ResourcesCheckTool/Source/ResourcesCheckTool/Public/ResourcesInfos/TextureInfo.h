#pragma once
#include "Engine/Texture2D.h"
#include "TextureInfo.generated.h"

USTRUCT()
struct RESOURCESCHECKTOOL_API FTextureInfo
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = CheckerParam)
	TEnumAsByte<enum TextureGroup> TextureGroup;

	UPROPERTY(EditAnywhere, Category = CheckerParam)
	int MaxSize;

	UPROPERTY(EditAnywhere, Category = CheckerParam)
	bool CompressNoAlpha;

	UPROPERTY(EditAnywhere, Category = CheckerParam)
	bool PowerOfTwo;


public:
	void Init(UTexture2D* Texture);
	
};

