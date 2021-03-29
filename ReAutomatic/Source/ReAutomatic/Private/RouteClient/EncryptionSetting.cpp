#include "RouteClient/EncryptionSetting.h"
#include "Containers/UnrealString.h"

TArray<uint8> FEncryptionSetting::GetKey()
{
	const int32 MaxBuffLen = 2048;
	uint8 buffer[MaxBuffLen] = { 0 };
	auto KeyHex = TEXT("4229E5461D3B0F289041A15A6751B0CCDA05C85D33128EFB52C9392D33CB32F6");
	const int Count = HexToBytes(KeyHex, buffer);
	return TArray<uint8>(buffer, Count);
}

TArray<uint8> FEncryptionSetting::GetIV()
{
	const int32 MaxBuffLen = 2048;
	uint8 buffer[MaxBuffLen] = { 0 };
	auto IVHex = TEXT("B4ADCEE216933B9AFA6A6B6AFFEED67E");
	const int Count = HexToBytes(IVHex, buffer);
	return TArray<uint8>(buffer, Count);
}
