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
	TArray<uint8> jsonData;
	ParentSerializer::Serialize(json, jsonData, JsonLength);

#if UE_BUILD_DEVELOPMENT
	{
		auto str = BytesToHex(jsonData.GetData(), JsonLength);
		UE_LOG(LogTemp, Log, TEXT("Net Package Json Bytes Hex Lenth: %d Content : %s"), JsonLength, *str)

		AES aes;
		auto key = FEncryptionSetting::GetKey();
		auto iv = FEncryptionSetting::GetIV();
		uint32 outLen;
		auto result = aes.EncryptCBC(jsonData.GetData(), JsonLength, key.GetData(), iv.GetData(), outLen);
		auto aes_data = TArray<uint8>(result, outLen);
		UE_LOG(LogTemp, Log, TEXT("AES Length : %d Content:%s"), outLen, *BytesToHex(aes_data.GetData(), outLen))
	}
#endif
	
	AES aes;
	auto key = FEncryptionSetting::GetKey();
	auto iv = FEncryptionSetting::GetIV();
	uint32 outLen;
	auto result = aes.EncryptCBC(jsonData.GetData(), JsonLength, key.GetData(), iv.GetData(), outLen);
	length = outLen;
	data = TArray<uint8>(result, outLen);
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