#include "Services/LuaService.h"


#include "FileHelper.h"
#include "Paths.h"
#include "Regex.h"
#include "Engine/World.h"

TArray<FString> ILuaService::SearchPaths;

void ILuaService::InitLuaStateWithLuaRoot(const FString& LuaRoot)
{
	SearchPaths.Add(LuaRoot);
	LoadFileDelegate delegate = [](const char* fn, FString& filepath)->TArray<uint8>
	{
		FString FileName(UTF8_TO_TCHAR(fn));
		FileName.ReplaceInline(TEXT("."), TEXT("/"), ESearchCase::CaseSensitive);
		FileName += ".lua";

		filepath = FileName;

		TArray<uint8> Content;
		for(auto& rootPath : SearchPaths)
		{
			FString FileFullPath = FPaths::Combine(rootPath, FileName);

			if(!FPaths::FileExists(FileFullPath))
			{
				continue;
			}
			
			UE_LOG(LogTemp, Log, TEXT("Load File from %s"), *FileFullPath);
	
			FFileHelper::LoadFileToArray(Content, *FileFullPath);

			int32 Size = Content.Num();
			if (Size >= 3 && Content[0] == 0xef && Content[1] == 0xbb && Content[2] == 0xbf)
			{
				Content.RemoveAt(0, 3, false);
			}

			return Content;
		}
		return Content;
	};
	
	InitLuaState(delegate);
}

UWorld* UCommonLuaLibrary::GetGWorld()
{
	return GWorld;
}


FString UCommonLuaLibrary::GetName(UObject* obj)
{
	return obj->GetName();
}

FString UCommonLuaLibrary::GetFullName(UObject* obj)
{
	return obj->GetFullName();
}

TArray<FString> UCommonLuaLibrary::GetModNames()
{
	TArray<FString> Result;
	const FString ModPath = FPaths::Combine(FPaths::ProjectContentDir(), "Mod");
	IFileManager& FileManager = IFileManager::Get();
	FileManager.FindFiles(Result, *ModPath, false, true);
	return Result;
}


bool UCommonLuaLibrary::IsMatch(const FString& source, const FString& regex)
{
	FRegexPattern pattern(regex);
	FRegexMatcher matcher(pattern, source);

	return matcher.FindNext();
}
