#include "Event/Listener.h"
#include "Event/EventDispatcher.h"

int32 IListener::CurrentIndex = 0;
	
	IListener::IListener()
	{
		Index = ++CurrentIndex;
	}

	const IListener* IListener::RegisterListener(FEventDispatcher& EventDispatcher)
	{
		EventDispatcher.RegisterListener(*this);
		return this;
	}

	WhereListener IListener::Where(TFunction<bool(IArg*)> Condition)
	{
		return WhereListener(*this, Condition);
	}

	IListener& IListener::AddHandler(TFunction<void(IArg*)> Handler)
	{
		IListener* Root = this;
		while(&GetParent() != this)
		{
			Root = &(Root->GetParent());
		}

		static_cast<BaseListener*>(Root)->OnTriggerActionList.Add(Handler);

		return *this;
	}

	BaseListener::BaseListener(const FName& eventName)
		: EventName(eventName)
	{
	}

	BaseListener::BaseListener(FName&& eventName)
		: EventName(MoveTemp(eventName))
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
			act(&arg);
		}
	}


	WhereListener::WhereListener(IListener& Parent, TFunction<bool(IArg*)> Condition)
		: Parent(Parent)
		, Condition(Condition)
	{

	}

	IListener& WhereListener::GetParent()
	{
		return Parent;
	}

	const FName& WhereListener::GetEventName() const
	{
		return Parent.GetEventName();
	}

	void WhereListener::OnTrigger(IArg& arg)
	{
		if(Condition(&arg))
		{
			Parent.OnTrigger(arg);
		}
	}