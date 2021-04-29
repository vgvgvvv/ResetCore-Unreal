#pragma once
#include "Kismet/BlueprintFunctionLibrary.h"
#include "AutomaticUIHelper.generated.h"

UENUM()
enum class EUIType : uint8
{
	Any = 0,
	Clickable = 1,
	TextInput = 2,
	Scrollable = 3,
	Text = 4,
	Checkable = 5,
};

USTRUCT()
struct FFindUICondition
{
	GENERATED_BODY()

	UPROPERTY()
	EUIType UIType;

	UPROPERTY()
	FString UINameRegex;

	UPROPERTY()
	FString ContainTextRegex;
	
};


class UWidget;
UCLASS()
class REAUTOMATIC_API UAutomaticUIHelper : public UBlueprintFunctionLibrary 
{
	GENERATED_BODY()
public:

	UFUNCTION()
	static UWidget* FindUWidgetObject(const FString& name, int index);

	UFUNCTION()
	static UWidget* FindWidgetObjectWithUICondition(const FFindUICondition& condition, int index);
	
	static UWidget* FindUWidgetObjectWithCondition(TFunction<bool(UWidget*)> condition, int index);

	static UWidget* FindInChildUserWidgetByCondition(const UWidget* parentWidget, const FString& path, TFunction<bool(UWidget*)> condition, int targetindex, int& currindex);
	
	UFUNCTION()
	static bool IsWidgetVisible(const UWidget * Widget);

	UFUNCTION()
	static bool IsWidgetMatchType(const UWidget* Widget, EUIType type);
	
	UFUNCTION()
	static bool IsWidgetTextMatchRegex(const UWidget* Widget, const FString& text);
	
	UFUNCTION()
	static bool IsWidgetVisibleWithUICondition(const FFindUICondition& condition, int index);
	
	static bool IsWidgetVisibleWithCondition(TFunction<bool(UWidget*)> condition);

};
