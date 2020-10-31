#pragma once

#include "CoreMinimal.h"


namespace CommonLib
{
	class IListener
	{
		IListener GetParent();

		FName GetEventName();

		void OnTrigger();
	};
	
	class Listener
	{
		
	};
	
	class EventDispatcher
	{
	public:
		
	};
}