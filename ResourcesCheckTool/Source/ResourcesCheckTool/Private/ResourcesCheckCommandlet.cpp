#include "ResourcesCheckCommandlet.h"



#include "FileHelper.h"
#include "JsonObject.h"
#include "Utility/JsonUtil.h"
#include "Paths.h"
#include "Engine/ObjectLibrary.h"

UResourcesCheckCommandlet::UResourcesCheckCommandlet()
{
	LogToConsole = true;
}

int32 UResourcesCheckCommandlet::Main(const FString& Params)
{
	UE_LOG(LogTemp, Display, TEXT("Hello !"));
	DumpAllResourcesInfo();
	return 0;
}

void UResourcesCheckCommandlet::DumpAllResourcesInfo()
{
	UObjectLibrary* ObjectLibrary = UObjectLibrary::CreateLibrary(UObject::StaticClass(), false, GIsEditor);
	
	int32 NumOfAssetDatas = ObjectLibrary->LoadAssetDataFromPath(TEXT("/Game"));

	UE_LOG(LogTemp, Display, TEXT("NumOfAssetDatas ! %d"), NumOfAssetDatas);
	
	TArray<FAssetData> AssetDatas;
	ObjectLibrary->GetAssetDataList(AssetDatas);
	for(auto& assetData : AssetDatas)
	{
		UE_LOG(LogTemp, Display, TEXT("%s"), *assetData.ObjectPath.ToString());
		DumpSingleAssetData(assetData);
	}
}

void UResourcesCheckCommandlet::DumpSingleAssetData(const FAssetData& AssetData)
{
	TSharedPtr<FJsonObject> DataInfoJson = MakeShared<FJsonObject>();
	DataInfoJson->SetStringField("Class", AssetData.AssetClass.ToString());
	DataInfoJson->SetStringField("Name", AssetData.AssetName.ToString());
	DataInfoJson->SetStringField("FullName", AssetData.GetFullName());

	auto DataInfoString = FJsonUtil::JsonObjectToString(DataInfoJson);
	
	auto SavedAssetInfo = FPaths::Combine(FPaths::ProjectSavedDir(),
            TEXT("AssetInfo"), AssetData.ObjectPath.ToString() + TEXT(".json"));
	FFileHelper::SaveStringToFile(DataInfoString, *SavedAssetInfo, FFileHelper::EEncodingOptions::ForceUTF8);
}
