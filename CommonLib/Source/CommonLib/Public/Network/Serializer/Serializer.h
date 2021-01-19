#pragma once
#include "CoreMinimal.h"

class ISerializer
{
public:
	virtual ~ISerializer() = default;
	
	virtual void Serialize(void* obj, TArray<uint8>& data) = 0;
	virtual void* DeSerialize(TArray<uint8> data) = 0;
};

