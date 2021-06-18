#pragma once
#include "Interface.h"
#include "LuaService.generated.h"

UINTERFACE(MinimalAPI)
class ULuaService : public UInterface
{
	GENERATED_BODY()
};

class COMMONLIB_API ILuaService
{
	GENERATED_BODY()
public:
	virtual void InitLuaState(){};

	virtual void RunLua(const FString& LuaFile){};
	
	virtual void UnInitLuaState(){};
};