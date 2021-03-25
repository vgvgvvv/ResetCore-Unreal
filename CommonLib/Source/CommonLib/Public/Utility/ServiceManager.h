#pragma once

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
class COMMONLIB_API UServiceManager : public UGameInstanceSubsystem
{
	GENERATED_BODY()
public:

	UFUNCTION()
	void RegisterService(UObject* Service, UClass* ServiceType, const FString& ServiceName = "");

	UFUNCTION()
	void UnregisterService(UClass* ServiceType, const FString& ServiceName = "");
	
	UFUNCTION()
	UObject* GetService(UClass* ServiceType, const FString& ServiceName = "");
private:
	UPROPERTY()
	TMap<FName, FServiceCollection> Services;
};
