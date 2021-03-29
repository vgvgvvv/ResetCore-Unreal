#include "Utility/ServiceManager.h"

#include "CommonLib.h"
#include "UObject/Class.h"

UServiceManager* UServiceManager::Instance;

UServiceManager* UServiceManager::Get(UGameInstance* GameInstance)
{
#if ENGINE_MINOR_VERSION >= 22 
	return GameInstance->GetSubsystem<UServiceManager>();
#else
	Instance = NewObject<UServiceManager>(GameInstance);
	return Instance;
#endif
}

void UServiceManager::RegisterService(UObject* Service, UClass* ServiceType, const FString& ServiceName)
{
	FString SearchName = ServiceName;
	if(SearchName == "")
	{
		SearchName = ServiceType->GetFName().ToString();
	}
	auto& Collection = Services.FindOrAdd(ServiceType->GetFName());
	if(!Service->GetClass()->ImplementsInterface(ServiceType))
	{
		UE_LOG(ResetCore_CommonLib, Warning, TEXT("UObject is not a "), *ServiceType->GetFName().ToString());
		return;
	}
	if(Collection.ServiceMap.Contains(SearchName))
	{
		UE_LOG(ResetCore_CommonLib, Warning, TEXT("Has Register %s With Named %s"), *ServiceType->GetFName().ToString(), *SearchName);
		return;
	}
	Collection.ServiceMap.Add(SearchName, Service);
}


void UServiceManager::UnregisterService(UClass* ServiceType, const FString& ServiceName)
{
	FString SearchName = ServiceName;
	if (SearchName == "")
	{
		SearchName = ServiceType->GetFName().ToString();
	}
	auto Collection = Services.Find(ServiceType->GetFName());
	if(Collection == nullptr)
	{
		return;
	}
	auto Service = Collection->ServiceMap.Find(SearchName);
	if(Service == nullptr)
	{
		return;
	}
	Collection->ServiceMap.Remove(SearchName);
}

UObject* UServiceManager::GetService(UClass* ServiceType, const FString& ServiceName)
{
	FString SearchName = ServiceName;
	if (SearchName == "")
	{
		SearchName = ServiceType->GetFName().ToString();
	}
	auto Collection = Services.Find(ServiceType->GetFName());
	if(Collection == nullptr)
	{
		return nullptr;
	}
	auto Service = Collection->ServiceMap.Find(SearchName);
	if(Service == nullptr)
	{
		return nullptr;
	}
	return *Service;
}
