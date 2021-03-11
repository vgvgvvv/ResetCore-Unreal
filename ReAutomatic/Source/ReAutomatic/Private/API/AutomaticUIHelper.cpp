#include "API/AutomaticUIHelper.h"

#include "Components/Button.h"
#include "Components/ScrollBox.h"
#include "Components/SlateWrapperTypes.h"
#include "UObject/UObjectIterator.h"
#include "Components/WidgetSwitcher.h"
#include "Blueprint/WidgetTree.h"

class UUserWidget;

UWidget* UAutomaticUIHelper::FindUWidgetObject(const FString& name, int index)
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
        if (fullChildPathName.Contains(name))
        {
            return UserWidget;
        }

        UWidget* findedWidget = FindInChildUserWidgetByName(UserWidget, UserWidget->GetName(), name, index, currentindex);
        if (findedWidget != nullptr) {
            return findedWidget;
        }
    }

    return nullptr;
}

UWidget* UAutomaticUIHelper::FindInChildUserWidgetByName(UWidget* parentWidget, const FString& path, const FString& name,
	int targetindex, int& currindex)
{
	UWidget* findedWidget = nullptr;

	TArray<UWidget*> Children;
	if (UUserWidget* UserWidget = Cast<UUserWidget>(parentWidget))
	{
		UWidget* ChildWidget = UserWidget->WidgetTree->RootWidget;
		if (ChildWidget)
		{
			Children.Add(ChildWidget);
		}
	}
	else if (UPanelWidget* Panel = Cast<UPanelWidget>(parentWidget))
	{
		for (int32 ChildIndex = 0; ChildIndex < Panel->GetChildrenCount(); ChildIndex++)
        {
        	if (UWidget* ChildWidget = Panel->GetChildAt(ChildIndex))
        	{
        		Children.Add(ChildWidget);
        	}
        }
	}
	else if (UScrollBox* ScrollBox = Cast<UScrollBox>(parentWidget))
	{
		for (int32 ChildIndex = 0; ChildIndex < ScrollBox->GetChildrenCount(); ChildIndex++)
		{
			if (UWidget* ChildWidget = ScrollBox->GetChildAt(ChildIndex))
			{
				Children.Add(ChildWidget);
			}
		}
	}
	else if (UWidgetSwitcher* widgetSwitcher = Cast<UWidgetSwitcher>(parentWidget))
	{
		UWidget* activeWidget = widgetSwitcher->GetActiveWidget();
		if (activeWidget)
		{
			Children.Add(activeWidget);
		}
	}
	else if (INamedSlotInterface* NamedSlotHost = Cast<INamedSlotInterface>(parentWidget))
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
	else if (UPanelWidget* PanelParent = Cast<UPanelWidget>(parentWidget))
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
			if (fullChildPathName.Contains(name))
			{
				if (targetindex == currindex)
					return childWidget;
				else
					currindex++;
			}

			findedWidget = FindInChildUserWidgetByName(childWidget, fullPathName, name, targetindex, currindex);
			if (findedWidget != nullptr)
				return findedWidget;
		}
	}

	return findedWidget;
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

