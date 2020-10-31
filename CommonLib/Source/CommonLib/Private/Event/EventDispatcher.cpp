#include "Event/EventDispatcher.h"


namespace CommonLib
{
	TMap<FName, IListener> FEventDispatcher::GlobalListener;
	
	void FEventDispatcher::RegisterListener(IListener& Listener)
	{
		GlobalListener.Add(Listener.GetEventName(), Listener);
	}

	void FEventDispatcher::RemoveListener(FName& EventName)
	{
		GlobalListener.Remove(EventName);
	}

	void FEventDispatcher::TriggerEvent(FName& EventName, IArg& arg)
	{
		auto Listener = GlobalListener.Find(EventName);
		if(Listener)
		{
			Listener->OnTrigger(arg);
		}
	}

}
