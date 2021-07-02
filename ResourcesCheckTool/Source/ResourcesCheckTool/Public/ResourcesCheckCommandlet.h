#pragma once

#include "Commandlets/Commandlet.h"
#include "ResourcesInfos/TextureInfo.h"


#include "ResourcesCheckCommandlet.generated.h"

UCLASS()
class UResourcesCheckCommandlet : public UCommandlet
{
	GENERATED_BODY()
public:

	UResourcesCheckCommandlet();

	UFUNCTION()
	virtual int32 Main(const FString& Params) override;

private:
	UFUNCTION()
	void DumpAllResourcesInfo();
	
	void DumpSingleAssetData(const FAssetData& AssetData);

	TSharedPtr<FJsonObject> GetAssetInfo(const FAssetData& AssetData);

	TMap<FString, FTextureInfoCreator> AssetInfoCreators;
};
