#pragma once
#include "CoreMinimal.h"

struct FPackage
{
	int32 Length = 0;
	int8 Data[] = {};
};

class PackageUtil
{
public:
	static FPackage MakeEmptyPackage()
	{
		return FPackage();
	}
};
