#include "Event/EventDispatcher.h"
#include "UnrealTemplate.h"

namespace CommonLib
{
	void FListenerGroup::AddListener(const IListener& Listener)
	{
		if (!Listeners.Contains(Listener))
		{
			Listeners.Add(Listener);
		}
		//等待被删除
		if (WaitToRemove.Contains(Listener))
		{
			WaitToRemove.Remove(Listener);
		}
	}

	void FListenerGroup::RemoveListener(const IListener& Listener)
	{
		if (IsRunning)
		{
			if (!WaitToRemove.Contains(Listener))
			{
				WaitToRemove.Add(Listener);
			}
		}
		else
		{
			if (Listeners.Contains(Listener))
			{
				Listeners.Remove(Listener);
			}
		}
	}

	void FListenerGroup::Trigger(IArg& arg)
	{
		IsRunning = true;
		for (int i = 0; i < Listeners.Num(); i ++)
		{
			if (WaitToRemove.Contains(Listeners[i]))
			{
				continue;
			}
			Listeners[i].OnTrigger(arg);
		}
		IsRunning = false;
		CleanWaitToRemove();
	}

	void FListenerGroup::CleanWaitToRemove()
	{
		if (IsRunning || WaitToRemove.Num() == 0)
		{
			return;
		}
		for (auto& Listener : WaitToRemove)
		{
			Listeners.Remove(Listener);
		}
		WaitToRemove.Empty();
	}

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
		FListenerGroup* Group = ListenerCenter.Find(Listener.GetEventName());
		if(!Group)
		{
			const FListenerGroup NewGroup;
			ListenerCenter.Add(Listener.GetEventName(), NewGroup);
			Group = ListenerCenter.Find(Listener.GetEventName());
		}
		Group->AddListener(Listener);
	}

	void FEventDispatcher::RemoveAllListener(const FName& EventName)
	{
		ListenerCenter.Remove(EventName);
	}

	void FEventDispatcher::RemoveAllListener(FName&& EventName)
	{
		ListenerCenter.Remove(MoveTemp(EventName));
	}

	void FEventDispatcher::RemoveListener(const IListener& Listener)
	{
		auto Group = ListenerCenter.Find(Listener.GetEventName());
		if(Group)
		{
			Group->RemoveListener(Listener);
		}
	}

	void FEventDispatcher::Clear()
	{
		ListenerCenter.Empty();
	}

	void FEventDispatcher::TriggerEvent(const FName& EventName, IArg& arg)
	{
		auto Group = ListenerCenter.Find(EventName);
		if(Group)
		{
			Group->Trigger(arg);
		}
	}

	void FEventDispatcher::TriggerEvent(FName&& EventName, IArg& arg)
	{
		auto Group = ListenerCenter.Find(MoveTemp(EventName));
		if (Group)
		{
			Group->Trigger(arg);
		}
	}


}
