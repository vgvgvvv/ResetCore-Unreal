#include "Network/NetPackage.h"


FNetPackage::FNetPackage()
{
}

FNetPackage::FNetPackage(int32 length, TArray<uint8> data)
{
	Length = length;
	Data = data;
}

bool FNetPackage::IsEmpty() const
{
	return Length == 0;
}

FNetPackage FNetPackageUtil::MakeEmptyPackage()
{
	return FNetPackage();
}
