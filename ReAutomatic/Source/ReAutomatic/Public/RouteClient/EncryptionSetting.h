#pragma once

#include "Containers/Array.h"

class FEncryptionSetting
{
public:
	static TArray<uint8> GetKey();
	static TArray<uint8> GetIV();
};
