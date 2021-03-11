#pragma once
#include "Kismet/BlueprintFunctionLibrary.h"
#include "AutomaticUIHelper.generated.h"

class UWidget;
UCLASS()
class REAUTOMATIC_API UAutomaticUIHelper : public UBlueprintFunctionLibrary 
{
	GENERATED_BODY()
public:

	UFUNCTION(BlueprintCallable)
	static UWidget* FindUWidgetObject(const FString& name, int index);
	
	UFUNCTION(BlueprintCallable)
	static UWidget* FindInChildUserWidgetByName(UWidget* parentWidget, const FString& path, const FString& name, int targetindex, int& currindex);
	
	UFUNCTION(BlueprintCallable)
	static bool IsWidgetVisible(const UWidget * Widget);

};
