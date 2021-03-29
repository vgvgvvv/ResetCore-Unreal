#include "RouteClient/EncryptionSetting.h"
#include "Containers/UnrealString.h"

TArray<uint8> FEncryptionSetting::GetKey()
{
	uint8* Key = nullptr;
	auto KeyHex = TEXT("4229E5461D3B0F289041A15A6751B0CCDA05C85D33128EFB52C9392D33CB32F6");
	const int Count = HexToBytes(KeyHex, Key);
	return TArray<uint8>(Key, Count);
}

TArray<uint8> FEncryptionSetting::GetIV()
{
	uint8* IV = nullptr;
	auto IVHex = TEXT("B4ADCEE216933B9AFA6A6B6AFFEED67E");
	const int Count = HexToBytes(IVHex, IV);
	return TArray<uint8>(IV, Count);
}
