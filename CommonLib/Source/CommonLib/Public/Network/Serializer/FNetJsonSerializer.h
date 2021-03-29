#pragma once

#include "Dom/JsonObject.h"
#include "Serializer.h"

class COMMONLIB_API FNetJsonSerializer
{
public:

	static void Serialize(TSharedPtr<FJsonObject> json, TArray<uint8>& data);
	static TSharedPtr<FJsonObject> DeSerialize(TArray<uint8>& data);
};
