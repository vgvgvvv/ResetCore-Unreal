#pragma once
#include "UObject/Class.h"
#include "UObject/Package.h"
#include "Containers/UnrealString.h"
#include "UObject/UObjectGlobals.h"

class COMMONLIB_API FEnumUtil
{
public:

	template<typename T>
	static FString EnumToString(const FString& enumName, const T value, const FString& defaultValue = TEXT(""));

	static FString ExpandEnumString(const FString& name, const FString& enumName);
};

template <typename T>
FString FEnumUtil::EnumToString(const FString& enumName, const T value, const FString& defaultValue)
{
	UEnum* pEnum = FindObject<UEnum>(ANY_PACKAGE, *enumName, true);
	return pEnum
		       ? ExpandEnumString(pEnum->GetNameByIndex(static_cast<uint8>(value)).ToString(), enumName)
		       : defaultValue;
}
