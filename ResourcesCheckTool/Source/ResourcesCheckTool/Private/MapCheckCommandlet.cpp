#include "MapCheckCommandlet.h"



#include "Editor.h"
#include "FileHelpers.h"
#include "Engine/Engine.h"
#include "Engine/ObjectLibrary.h"
#include "Kismet/GameplayStatics.h"

UMapCheckCommandlet::UMapCheckCommandlet()
{
	LogToConsole = true;
}

int32 UMapCheckCommandlet::Main(const FString& Params)
{

	auto result = LoadLevel("/Game/Maps/TestUnrealResetCore.umap");

	if(!result)
	{
		UE_LOG(LogTemp, Error, TEXT("Cannot load map"));
		return 0;
	}
	
	TArray<AActor*> Result;
	UGameplayStatics::GetAllActorsOfClass(GWorld, AActor::StaticClass(), Result);

	
	for(auto actor : Result)
	{
		auto Name = actor->GetFullName();
		UE_LOG(LogTemp, Display, TEXT("Name:%s"), *Name);
	}
	
	return 0;
}

TArray<FString> UMapCheckCommandlet::GetAllMapNames(){
	auto ObjectLibrary = UObjectLibrary::CreateLibrary(UWorld::StaticClass(), false, true);
	ObjectLibrary->LoadAssetDataFromPath(TEXT("/Game/Maps"));
	TArray<FAssetData> AssetDatas;
	ObjectLibrary->GetAssetDataList(AssetDatas);
	UE_LOG(LogTemp, Warning, TEXT("Found maps: %d"), AssetDatas.Num());
 
	TArray<FString> Names = TArray<FString>();
 
	for (int32 i = 0; i < AssetDatas.Num(); ++i)
	{
		FAssetData& AssetData = AssetDatas[i];
 
		auto name = AssetData.AssetName.ToString();
		Names.Add(name);
	}
	return Names;
}


bool UMapCheckCommandlet::LoadLevel(const FString& LevelToLoad)
{
	bool bResult = false;
    
    	if (!LevelToLoad.IsEmpty())
    	{
    		UE_LOG(LogTemp, Log, TEXT("Loading Map %s"), *LevelToLoad);
    
    		FString Filename;
    		if (FPackageName::TryConvertLongPackageNameToFilename(LevelToLoad, Filename))
    		{
    			UPackage* Package = LoadPackage(NULL, *Filename, 0);
    
    			UWorld* World = UWorld::FindWorldInPackage(Package);
    			if (World)
    			{
    				// Clean up any previous world.  The world should have already been saved
    				UWorld* ExistingWorld = GEditor->GetEditorWorldContext().World();
    
    				GEngine->DestroyWorldContext(ExistingWorld);
    				ExistingWorld->DestroyWorld(true, World);
    
    				GWorld = World;
    
    				World->WorldType = EWorldType::Editor;
    
    				FWorldContext& WorldContext = GEngine->CreateNewWorldContext(World->WorldType);
    				WorldContext.SetCurrentWorld(World);
    
    				// add the world to the root set so that the garbage collection to delete replaced actors doesn't garbage collect the whole world
    				World->AddToRoot();
    
    				// initialize the levels in the world
    				World->InitWorld(UWorld::InitializationValues().AllowAudioPlayback(false));
    				World->GetWorldSettings()->PostEditChange();
    				World->UpdateWorldComponents(true, false);
    
    				bResult = true;
    			}
    		}
    	}
    	else
    	{
    		// a map was not specified, ignore
    		bResult = true;
    	}
    
    	if (!bResult)
    	{
    		UE_LOG(LogTemp, Error, TEXT("Could not find or load level %s"), *LevelToLoad);
    	}
    
    	return bResult;
}
