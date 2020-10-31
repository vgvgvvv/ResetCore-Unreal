#include "Event/EventDispatcher.h"
#include "UnrealTemplate.h"

namespace CommonLib
{
	TMap<FName, IListener> FEventDispatcher::GlobalListener;


	BaseListener FEventDispatcher::CreateListener(const FName& eventName)
	{
		return BaseListener(eventName);
	}


	BaseListener FEventDispatcher::CreateListener(FName&& eventName)
	{
		return BaseListener(MoveTemp(eventName));
	}

	void FEventDispatcher::RegisterListener(IListener& Listener)
	{
		GlobalListener.Add(Listener.GetEventName(), Listener);
	}

	void FEventDispatcher::RemoveListener(const FName& EventName)
	{
		GlobalListener.Remove(EventName);
	}

	void FEventDispatcher::RemoveListener(FName&& EventName)
	{
		GlobalListener.Remove(MoveTemp(EventName));
	}


	void FEventDispatcher::TriggerEvent(const FName& EventName, IArg& arg)
	{
		auto Listener = GlobalListener.Find(EventName);
		if(Listener)
		{
			Listener->OnTrigger(arg);
		}
	}

	void FEventDispatcher::TriggerEvent(FName&& EventName, IArg& arg)
	{
		auto Listener = GlobalListener.Find(MoveTemp(EventName));
		if (Listener)
		{
			Listener->OnTrigger(arg);
		}
	}


}
