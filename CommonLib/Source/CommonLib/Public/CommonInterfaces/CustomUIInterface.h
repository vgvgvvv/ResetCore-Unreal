#pragma once

#include "CoreMinimal.h"
#include "Interface.h"
#include "CustomUIInterface.generated.h"

UINTERFACE(MinimalAPI)
class UTextWidgetInterface : public UInterface
{
	GENERATED_BODY()
};

/**
* 
*/
class ITextWidgetInterface
{
	GENERATED_BODY()
public:
	
	UFUNCTION()
	virtual const FString& GetTextContent() const
	{
		return *(FString*)nullptr;
	}
};	

