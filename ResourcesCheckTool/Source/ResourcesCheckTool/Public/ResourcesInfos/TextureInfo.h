#pragma once
#include "BaseAssetInfo.h"
#include "Engine/Texture2D.h"
#include "TextureInfo.generated.h"

USTRUCT()
struct RESOURCESCHECKTOOL_API FTextureCheckInfo : public FBaseAssetInfo
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

	virtual void Init(const UObject* AssetObj) override;
};

class RESOURCESCHECKTOOL_API FTextureInfoCreator : public FBaseAssetInfoCreator
{
public:
	virtual TSharedPtr<FJsonObject> GetAssetInfo(UObject* AssetObject) override;
};

