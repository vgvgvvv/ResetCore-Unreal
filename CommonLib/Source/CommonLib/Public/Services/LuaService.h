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
public:
	virtual void InitLuaState(const FString& LuaRoot){};

	virtual void RunLua(const FString& LuaFile){};
	
	virtual void UnInitLuaState(){};
};