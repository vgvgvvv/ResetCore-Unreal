#pragma once
#include "Event/Listener.h"

class COMMONLIB_API FListenerGroup
{
	public:
		FListenerGroup() = default;
		void AddListener(const IListener& Listener);
		void RemoveListener(const IListener& Listener);
		void Trigger(IArg& arg);
		private:
		void CleanWaitToRemove();
	private:
	TArray<IListener> Listeners;
	TArray<IListener> WaitToRemove;
	bool IsRunning = false;
};
	
class COMMONLIB_API FEventDispatcher
{
	TMap<FName, FListenerGroup> ListenerCenter;

	public:

	static BaseListener CreateListener(const FName& eventName);

	static BaseListener CreateListener(FName&& eventName);

	void RegisterListener(IListener& Listener);

	void RemoveAllListener(const FName& EventName);

	void RemoveAllListener(FName&& EventName);

	void RemoveListener(const IListener& Listener);

	void Clear();

	void TriggerEvent(const FName& EventName, IArg& arg);

	void TriggerEvent(FName&& EventName, IArg& arg);
};