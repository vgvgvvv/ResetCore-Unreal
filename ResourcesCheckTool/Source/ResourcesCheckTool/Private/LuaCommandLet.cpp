#include "LuaCommandLet.h"
#include "LinqCore.h"
#include "LuaService.h"
#include "Paths.h"
#include "ServiceManager.h"

int32 URunLuaCommandLet::Main(const FString& Params)
{
	TArray<FString> Tokens;
	TArray<FString> Switches;
	ParseCommandLine(*Params, Tokens, Switches);

	TMap<FString, FString> SwitchKeyValue;
	for(auto& Switch : Switches)
	{
		if(Switch.Contains("="))
		{
			FString Key;
			FString Value;
			if(Switch.Split("=",&Key, &Value))
			{
				SwitchKeyValue.Add(Key, Value);
			};
		}else
		{
			SwitchKeyValue.Add(Switch, "");
		}
	}

	TArray<FString> Keys;
	SwitchKeyValue.GetKeys(Keys);

	for(auto& key : Keys)
	{
		UE_LOG(LogTemp, Display, TEXT("switch %s : %s"), *key, *SwitchKeyValue[key]);
	}
	
	if(!Keys.Contains("LuaFile"))
	{
		UE_LOG(LogTemp, Display, TEXT("Must Contain LuaFile=xxx Switch!!"))
		return 1;
	}

	auto& LuaFile = SwitchKeyValue["LuaFile"];

	auto luaService = Cast<ILuaService>(
        UServiceManager::Get(nullptr)->GetService(
            ULuaService::StaticClass(),
            "EditorLua"));

	FString LuaRoot = FPaths::Combine(FPaths::ProjectPluginsDir(), TEXT("resetcore-unreal/ResourcesCheckTool/Source/LuaSource"));
	luaService->InitLuaStateWithLuaRoot(LuaRoot);
	
	UE_LOG(LogTemp, Display, TEXT("Begin RunLua : %s"), *LuaFile);
	luaService->RunLua(LuaFile, TEXT("main"));
	UE_LOG(LogTemp, Display, TEXT("End RunLua : %s"), *LuaFile);
	
	luaService->UnInitLuaState();
	
	
	return 0;
}
