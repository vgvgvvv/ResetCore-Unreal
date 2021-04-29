#include "ResourcesCheckCommandlet.h"

UResourcesCheckCommandlet::UResourcesCheckCommandlet()
{
	LogToConsole = true;
}

int32 UResourcesCheckCommandlet::Main(const FString& Params)
{
	UE_LOG(LogTemp, Display, TEXT("Hello Yaksue!"));
	return 0;
}
