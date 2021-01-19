#pragma once
#include "CoreMinimal.h"

struct COMMONLIB_API FNetPackage
{
	int32 Length = 0;
	TArray<uint8> Data;

	FNetPackage();
	FNetPackage(int32 length, TArray<uint8> data);
	
	bool IsEmpty() const;

	template <typename T, typename TSerializer>
	T GetValue();
};

class COMMONLIB_API FNetPackageUtil
{
public:
	static FNetPackage MakeEmptyPackage();

	template <typename T, typename TSerializer>
	static FNetPackage MakePack(T Obj);
};

template <typename T, typename TSerializer>
T FNetPackage::GetValue()
{
	return TSerializer::DeSerialize(Data);
}

template <typename T, typename TSerializer>
auto FNetPackageUtil::MakePack(T Obj) -> FNetPackage
{
	FNetPackage result;
	TSerializer::Serialize(Obj, result.Data);
	result.Length = result.Data.Num();
	return result;
}
