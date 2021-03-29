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
	static void Serialize(TSharedPtr<FJsonObject>json, TArray<uint8>& data);
	
	static TSharedPtr<FJsonObject> DeSerialize(TArray<uint8>& data);
};

template<typename ParentSerializer>
void FAESEncryptionSerializer<ParentSerializer>::Serialize(TSharedPtr<FJsonObject> json, TArray<uint8>& data)
{
	ParentSerializer::Serialize(json, data);
	AES aes;
	auto key = FEncryptionSetting::GetKey();
	auto iv = FEncryptionSetting::GetIV();
	uint32 outlen;
	auto result = aes.EncryptCBC(data.GetData(), data.Num(), key.GetData(), iv.GetData(), outlen);
	data = TArray<uint8>(result, outlen);
}

template<typename ParentSerializer>
TSharedPtr<FJsonObject> FAESEncryptionSerializer<ParentSerializer>::DeSerialize(TArray<uint8>& data)
{
	AES aes;
	auto key = FEncryptionSetting::GetKey();
	auto iv = FEncryptionSetting::GetIV();
	uint32 outLen;
	auto result = aes.DecryptCBC(data.GetData(), data.Num(), key.GetData(), iv.GetData(), outLen);

	TArray<uint8> unencrypt(result, outLen);
	return ParentSerializer::DeSerialize(unencrypt);
}