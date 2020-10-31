#include "Event/Listener.h"
#include "Event/EventDispatcher.h"
#include "CoreMinimal.h"

namespace CommonLib
{
	
	void IListener::RegisterListener()
	{
		FEventDispatcher::RegisterListener(*this);
	}

	Where IListener::Where(TFunction<bool(IArg&)> Condition)
	{
		return Where(Condition);
	}

	IListener& IListener::AddHandler(TFunction<void(IArg&)> Handler)
	{
		IListener* Root = this;
		while(&GetParent() != this)
		{
			Root = &(Root->GetParent());
		}

		static_cast<BaseListener*>(Root)->OnTriggerActionList.Add(Handler);
		
		return *this;
	}

	BaseListener::BaseListener(FName& eventName)
	{
	}

	IListener& BaseListener::GetParent()
	{
		return *this;
	}

	const FName& BaseListener::GetEventName() const
	{
		return EventName;
	}

	void BaseListener::OnTrigger(IArg& arg)
	{
		for(auto& act : OnTriggerActionList)
		{
			act(arg);
		}
	}


	Where::Where(IListener& Parent, TFunction<bool(IArg&)> Condition)
		: Parent(Parent)
		, Condition(Condition)
	{
		
	}

	IListener& Where::GetParent()
	{
		return Parent;
	}

	const FName& Where::GetEventName() const
	{
		return Parent.GetEventName();
	}

	void Where::OnTrigger(IArg& arg)
	{
		if(Condition(arg))
		{
			Parent.OnTrigger(arg);
		}
	}
}

