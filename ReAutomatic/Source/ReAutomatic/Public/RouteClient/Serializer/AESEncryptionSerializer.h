#pragma once

#include "Network/Serializer/FNetJsonSerializer.h"
#include "RouteClient/EncryptionSetting.h"
#include "Dom/JsonObject.h"
#include "Network/Serializer/Serializer.h"
#include "Security/AES.h"

template<typename ParentSerializer = FNetJsonSerializer>
class FAESEncryptionSerializer
{
public:
	static void Serialize(TSharedPtr<FJsonObject>json, TArray<uint8>& data, int& length);
	
	static TSharedPtr<FJsonObject> DeSerialize(TArray<uint8>& data, int length);
};

template<typename ParentSerializer>
void FAESEncryptionSerializer<ParentSerializer>::Serialize(TSharedPtr<FJsonObject> json, TArray<uint8>& data, int& length)
{
	int32 JsonLength;
	ParentSerializer::Serialize(json, data, JsonLength);
	AES aes;
	auto key = FEncryptionSetting::GetKey();
	auto iv = FEncryptionSetting::GetIV();
	uint32 outLen;
	auto result = aes.EncryptCBC(data.GetData(), JsonLength, key.GetData(), iv.GetData(), outLen);
	length = outLen;
	data = TArray<uint8>(result, length);
}

template<typename ParentSerializer>
TSharedPtr<FJsonObject> FAESEncryptionSerializer<ParentSerializer>::DeSerialize(TArray<uint8>& data, int length)
{
	AES aes;
	auto key = FEncryptionSetting::GetKey();
	auto iv = FEncryptionSetting::GetIV();
	uint32 outLen;
	auto result = aes.DecryptCBC(data.GetData(), length, key.GetData(), iv.GetData(), outLen);

	TArray<uint8> unencrypt(result, outLen);
	return ParentSerializer::DeSerialize(unencrypt, outLen);
}