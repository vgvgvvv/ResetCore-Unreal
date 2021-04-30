#include "API/AutomaticUIHelper.h"



#include "Components/CheckBox.h"
#include "Components/MultiLineEditableText.h"
#include "Components/MultiLineEditableTextBox.h"
#include "Components/RichTextBlock.h"
#include "Components/TextBlock.h"
#include "Internationalization/Regex.h"
#include "Components/Button.h"
#include "Components/ScrollBox.h"
#include "Components/SlateWrapperTypes.h"
#include "UObject/UObjectIterator.h"
#include "Components/WidgetSwitcher.h"
#include "Blueprint/WidgetTree.h"

class UUserWidget;

UWidget* UAutomaticUIHelper::FindUWidgetObjectWithCondition(TFunction<bool(UWidget*)> condition, int index)
{
	int currentindex = 0;
	for (TObjectIterator<UUserWidget> Itr; Itr; ++Itr)
	{
		UUserWidget* UserWidget = *Itr;

    	
		if (UserWidget == nullptr || !IsWidgetVisible(UserWidget) || UserWidget->WidgetTree == nullptr) {
			// UE_LOG(GALog, Log, TEXT("UUserWidget Iterator get a null(unvisible) UUserWidget"));
			continue;
		}
    	
		FString fullChildPathName = UserWidget->GetFullName();
		UE_LOG(LogTemp, Log, TEXT("FindUWidgetObject %s"), *fullChildPathName)
		if (condition(UserWidget))
		{
			return UserWidget;
		}

		UWidget* findedWidget = FindInChildUserWidgetByCondition(UserWidget, UserWidget->GetName(), condition, index, currentindex);
		if (findedWidget != nullptr) {
			return findedWidget;
		}
	}

	return nullptr;
}

UWidget* UAutomaticUIHelper::FindInChildUserWidgetByCondition(const UWidget* parentWidget, const FString& path,
	TFunction<bool(UWidget*)> condition, int targetindex, int& currindex)
{
	UWidget* findedWidget = nullptr;

	TArray<UWidget*> Children;
	if (const UUserWidget* UserWidget = Cast<UUserWidget>(parentWidget))
	{
		UWidget* ChildWidget = UserWidget->WidgetTree->RootWidget;
		if (ChildWidget)
		{
			Children.Add(ChildWidget);
		}
	}
	else if (const UPanelWidget* Panel = Cast<UPanelWidget>(parentWidget))
	{
		for (int32 ChildIndex = 0; ChildIndex < Panel->GetChildrenCount(); ChildIndex++)
        {
        	if (UWidget* ChildWidget = Panel->GetChildAt(ChildIndex))
        	{
        		Children.Add(ChildWidget);
        	}
        }
	}
	else if (const UScrollBox* ScrollBox = Cast<UScrollBox>(parentWidget))
	{
		for (int32 ChildIndex = 0; ChildIndex < ScrollBox->GetChildrenCount(); ChildIndex++)
		{
			if (UWidget* ChildWidget = ScrollBox->GetChildAt(ChildIndex))
			{
				Children.Add(ChildWidget);
			}
		}
	}
	else if (const UWidgetSwitcher* widgetSwitcher = Cast<UWidgetSwitcher>(parentWidget))
	{
		UWidget* activeWidget = widgetSwitcher->GetActiveWidget();
		if (activeWidget)
		{
			Children.Add(activeWidget);
		}
	}
	else if (const INamedSlotInterface* NamedSlotHost = Cast<INamedSlotInterface>(parentWidget))
	{
		TArray<FName> SlotNames;
		NamedSlotHost->GetSlotNames(SlotNames);

		for (FName SlotName : SlotNames)
		{
			if (UWidget* SlotContent = NamedSlotHost->GetContentForSlot(SlotName))
			{
				Children.Add(SlotContent);
			}
		}
	}
	else if (const UPanelWidget* PanelParent = Cast<UPanelWidget>(parentWidget))
	{
		// Search standard children.
		for (int32 ChildIndex = 0; ChildIndex < PanelParent->GetChildrenCount(); ChildIndex++)
		{
			if (UWidget* ChildWidget = PanelParent->GetChildAt(ChildIndex))
			{
				Children.Add(ChildWidget);
			}
		}
	}

	FString childPath = path ;
	for (size_t i = 0; i < Children.Num(); i++)
	{
		UWidget* childWidget = Children[i];
		FString childWidgetName = childWidget->GetName();
		FString fullPathName = childPath + "." + childWidgetName;

		bool IsVisible = IsWidgetVisible(childWidget);
		if (IsVisible)
		{
			//FString fullChildPathName = fullPathName + "." + childWidget->GetName();
			FString fullChildPathName = childWidget->GetFullName();
			if (condition(childWidget))
			{
				if (targetindex == currindex)
					return childWidget;
				else
					currindex++;
			}

			findedWidget = FindInChildUserWidgetByCondition(childWidget, fullPathName, condition, targetindex, currindex);
			if (findedWidget != nullptr)
				return findedWidget;
		}
	}

	return findedWidget;
}

bool UAutomaticUIHelper::IsWidgetMatchType(const UWidget* widget, EUIType type)
{
	if(type == EUIType::Any)
	{
		return true;
	}
	
	switch (type)
   {
        case EUIType::Clickable:
    	    if(!widget->IsA(UButton::StaticClass()))
    	    {
    		    return false;
    	    }
    	    break;
    	case EUIType::TextInput:
    		if(!widget->IsA(UMultiLineEditableText::StaticClass()) &&
    			widget->IsA(UMultiLineEditableTextBox::StaticClass()))
    		{
    			return false;
    		}
    		break;
    	case EUIType::Scrollable:
    		if(!widget->IsA(UScrollBox::StaticClass()))
    		{
    		    return false;	
    		}
    		break;
    	case EUIType::Text:
    		if(!widget->IsA(UTextBlock::StaticClass()))
    		{
    			return false;
    		}
    		break;
    	case EUIType::Checkable:
    		if(!widget->IsA(UCheckBox::StaticClass()))
    		{
    			return false;
    		}
    		break;
   }
	return true;
}

bool UAutomaticUIHelper::IsWidgetTextMatchRegex(const UWidget* widget, const FString& regex)
{
	FString content;
	if(widget->IsA(UTextBlock::StaticClass()))
	{
		auto TextWidget = Cast<UTextBlock>(widget);
		content = TextWidget->Text.ToString();
	}
	else if(widget->IsA(URichTextBlock::StaticClass()))
	{
		auto TextWidget = Cast<URichTextBlock>(widget);
		content = TextWidget->GetLabelText().ToString();
	}
	else if(widget->IsA(UMultiLineEditableText::StaticClass()))
	{
		auto TextWidget = Cast<UMultiLineEditableText>(widget);
		content = TextWidget->Text.ToString();
	}
	else if(widget->IsA(UMultiLineEditableTextBox::StaticClass()))
	{
		auto TextWidget = Cast<UMultiLineEditableTextBox>(widget);
		content = TextWidget->Text.ToString();
	}
	else
	{
		int currentindex = 0;
		return FindInChildUserWidgetByCondition(widget, widget->GetName(), [regex](UWidget* widget)
		{
			return IsWidgetTextMatchRegex(widget, regex);
		}, 0, currentindex) != nullptr;
    }
	
	FRegexPattern pattern(regex);
	FRegexMatcher matcher(pattern, content);
	if(!matcher.FindNext())
	{
		return false;
	}
	return true;
}

UWidget* UAutomaticUIHelper::FindUWidgetObject(const FString& name, int index)
{
    return FindUWidgetObjectWithCondition([name](UWidget* widget) -> bool
    {
	    return widget->GetFullName().Contains(name);
    }, index);
}

UWidget* UAutomaticUIHelper::FindWidgetObjectWithUICondition(const FFindUICondition& condition, int index)
{
	return FindUWidgetObjectWithCondition([condition](UWidget* widget) -> bool
    {
		if(condition.UIType != EUIType::Any)
		{
			if(!IsWidgetMatchType(widget, condition.UIType))
			{
				return false;
			}
		}

		if(condition.ContainTextRegex != "")
		{
			if(!IsWidgetTextMatchRegex(widget, condition.ContainTextRegex))
			{
				return false;
			}
		}

		if(condition.UINameRegex != "")
		{
			FRegexPattern pattern(condition.UINameRegex);
            FRegexMatcher matcher(pattern, widget->GetFullName());
            if(!matcher.FindNext())
            {
                return false;
            }
		}

		return true;
        
    }, index);
}

bool UAutomaticUIHelper::IsWidgetVisible(const UWidget * Widget)
{
    if ((Widget->Visibility == ESlateVisibility::Collapsed) ||
    	(Widget->Visibility == ESlateVisibility::Hidden))
    {
    	return false;
    }

	return Widget->IsVisible();
}

bool UAutomaticUIHelper::IsWidgetVisibleWithCondition(TFunction<bool(UWidget*)> condition)
{
	auto widget = FindUWidgetObjectWithCondition(condition, 0);
	if(widget == nullptr)
	{
		return false;
	}
	return IsWidgetVisible(widget);
}

bool UAutomaticUIHelper::IsWidgetVisibleWithUICondition(const FFindUICondition& condition, int index)
{
	auto widget = FindWidgetObjectWithUICondition(condition, 0);
	if(widget == nullptr)
	{
		return false;
	}
	return IsWidgetVisible(widget);
}


