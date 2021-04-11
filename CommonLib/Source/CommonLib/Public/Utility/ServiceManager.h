#pragma once
#include "Engine/GameInstance.h"

#include "ServiceManager.generated.h"


USTRUCT()
struct FServiceCollection
{
	GENERATED_BODY()
public:
	UPROPERTY()
	TMap<FString, UObject*> ServiceMap;
};

UCLASS()
class COMMONLIB_API UServiceManager
#if ENGINE_MINOR_VERSION >= 22 
	: public UGameInstanceSubsystem
#else
	: public UObject
#endif
{
	GENERATED_BODY()
public:

	static UServiceManager* Get(UGameInstance* GameInstance);
	
	UFUNCTION()
	void RegisterService(UObject* Service, UClass* ServiceType, const FString& ServiceName = "");

	UFUNCTION()
	void UnregisterService(UClass* ServiceType, const FString& ServiceName = "");
	
	UFUNCTION()
	UObject* GetService(UClass* ServiceType, const FString& ServiceName = "");
private:
	UPROPERTY()
	TMap<FName, FServiceCollection> Services;

	static UServiceManager* Instance;
};
