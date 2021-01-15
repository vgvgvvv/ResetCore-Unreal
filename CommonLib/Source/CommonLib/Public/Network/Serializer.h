#pragma once
#include "CoreMinimal.h"

class ISerializer
{
public:
	template<typename T>
	int32 Serialize(T obj, int8* data){ return 0; }

	template<typename T>
	T DeSerialize(int8* data, int32 length){ return T(); }
};
