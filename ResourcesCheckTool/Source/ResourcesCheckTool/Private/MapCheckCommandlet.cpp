#include "MapCheckCommandlet.h"



#include "Editor.h"
#include "Misc/FileHelper.h"
#include "FileHelpers.h"
#include "Engine/LevelBounds.h"
#include "Services/LuaService.h"
#include "Utility/ServiceManager.h"
#include "Engine/Engine.h"
#include "Engine/ObjectLibrary.h"
#include "GameFramework/WorldSettings.h"
#include "Kismet/GameplayStatics.h"

UMapCheckCommandlet::UMapCheckCommandlet()
{
	LogToConsole = true;
}

int32 UMapCheckCommandlet::Main(const FString& Params)
{

	auto luaService = Cast<ILuaService>(
		UServiceManager::Get(nullptr)->GetService(
			ULuaService::StaticClass(),
			"EditorLua"));

	FString LuaRoot = FPaths::Combine(FPaths::ProjectPluginsDir(), TEXT("resetcore-unreal/ResourcesCheckTool/Source/LuaSource"));
	luaService->InitLuaStateWithLuaRoot(LuaRoot);
	
	UE_LOG(LogTemp, Display, TEXT("Begin RunLua : MapCheck.entry"));
	luaService->RunLua(TEXT("MapCheck.entry"), TEXT("main"));
	UE_LOG(LogTemp, Display, TEXT("End RunLua : MapCheck.entry"));
	
	luaService->UnInitLuaState();
	
	return 0;
}

TArray<FAssetData> UMapCheckCommandlet::GetAllMapAssets(const FString& MapRoot){
	auto ObjectLibrary = UObjectLibrary::CreateLibrary(UWorld::StaticClass(), false, true);
	ObjectLibrary->LoadAssetDataFromPath(MapRoot);
	TArray<FAssetData> AssetDatas;
	ObjectLibrary->GetAssetDataList(AssetDatas);
	return AssetDatas;
}


bool UMapCheckCommandlet::LoadLevel(const FString& LevelToLoad)
{
	bool bResult = false;
    
    	if (!LevelToLoad.IsEmpty())
    	{
    		UE_LOG(LogTemp, Display, TEXT("Loading Map %s"), *LevelToLoad);
    
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
    				World->InitWorld(UWorld::InitializationValues()
    					.AllowAudioPlayback(false)
    					.CreatePhysicsScene(false)
    					.CreateAISystem(false)
    					.ShouldSimulatePhysics(false));
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


FVector UMapCheckCommandlet::GetLevelBoundsExtend()
{
	auto bounds = ALevelBounds::CalculateLevelBounds(GWorld->GetCurrentLevel());
	return bounds.GetExtent();
}
