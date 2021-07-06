#include "API/Log.h"
#include "CoreMinimal.h"


void FLog::Log(const std::string& Info)
{
	UE_LOG(LogTemp, Log, TEXT("%s"), *FString(Info.c_str()));
}

void FLog::Warn(const std::string& Info)
{
	UE_LOG(LogTemp, Warning, TEXT("%s"), *FString(Info.c_str()));
}

void FLog::Error(const std::string& Info)
{
	UE_LOG(LogTemp, Error, TEXT("%s"), *FString(Info.c_str()));
}
