#pragma once

#include "JsonObject.h"
#include "Serializer.h"

class FNetJsonSerializer
{
public:

	void SerializeJson(TSharedPtr<FJsonObject>, TArray<uint8>& data);
	TSharedPtr<FJsonObject> DeSerializeJson(TArray<uint8>& data);
};
