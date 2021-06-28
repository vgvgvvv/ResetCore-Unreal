#pragma once
#include "UObject/Interface.h"
#include "Kismet/BlueprintFunctionLibrary.h"

#include "LuaService.generated.h"

UINTERFACE(MinimalAPI)
class ULuaService : public UInterface
{
	GENERATED_BODY()
};

typedef TArray<uint8> (*LoadFileDelegate) (const char* fn, FString& filepath);

class COMMONLIB_API ILuaService
{
	GENERATED_BODY()
public:
	virtual void InitLuaState(LoadFileDelegate loadFile){};

	virtual void InitLuaStateWithLuaRoot(const FString& LuaRoot);
	
	virtual void RunLua(const FString& LuaFile, const FString& FunctionName){};
	
	virtual void UnInitLuaState(){ SearchPaths.Empty(); }

private:

	static TArray<FString> SearchPaths;
};

UCLASS()
class COMMONLIB_API UCommonLuaLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:

	UFUNCTION()
	static UWorld* GetGWorld();

	UFUNCTION()
	static FString GetName(UObject* obj);

	UFUNCTION()
	static FString GetFullName(UObject* obj);

	UFUNCTION()
	static TArray<FString> GetModNames();

	UFUNCTION()
	static bool IsMatch(const FString& source, const FString& regex);

};