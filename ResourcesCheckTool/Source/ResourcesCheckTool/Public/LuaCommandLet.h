#pragma once
#include "Commandlets/Commandlet.h"
#include "LuaCommandLet.generated.h"

UCLASS()
class RESOURCESCHECKTOOL_API ULuaCommandLet : public UCommandlet
{
	GENERATED_BODY()
public:
	virtual int32 Main(const FString& Params) override;
};
