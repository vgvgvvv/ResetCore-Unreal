#include "TestCommandlet.h"
#include <iostream>

UTestCommandlet::UTestCommandlet()
{
}

int32 UTestCommandlet::Main(const FString& Params)
{
	std::cout << "Hello" << std::endl;
	UE_LOG(LogTemp, Display, TEXT("Hello %s"), *Params);
	return 0;
}
