#include "MapCheckCommandlet.h"


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

	UWorld* NewWorld = UWorld::CreateWorld(EWorldType::None, false);

	// UGameplayStatics::OpenLevel( GWorld, FName( "TestMap" ), true );
	// GEngine->LoadMap( *GEngine->GetWorldContextFromWorld( GWorld ), URL, nullptr, Error );
	GEngine->SetClientTravel( GWorld, TEXT( "TestMap" ), TRAVEL_Absolute );
	// UPackage* Package = LoadPackage( nullptr, TEXT( "/Game/Maps/SunTemple.umap" ), LOAD_EditorOnly );
	// GEditor->HandleMapCommand( TEXT( "LOAD FILE=" ), Ar, GWorld );
	// FEditorFileUtils::LoadMap( TEXT( "/Game/TestMap.umap" ) );
	
	// UGameplayStatics::OpenLevel(NewWorld, FName(TEXT("TestMap")));
	TArray<AActor*> Result;
	UGameplayStatics::GetAllActorsOfClass(NewWorld, AActor::StaticClass(), Result);

	
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
 