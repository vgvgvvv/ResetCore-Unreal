#pragma once
#include "CoreMinimal.h"
#include "Event/Listener.h"

namespace CommonLib
{
	class FEventDispatcher
	{
		static TMap<FName, IListener> GlobalListener;

	public:

		static void RegisterListener(IListener& Listener);

		static void RemoveListener(FName& EventName);

		static void TriggerEvent(const FName& EventName, IArg& arg);

		static void TriggerEvent(FName&& EventName, IArg& arg);

	};
}
