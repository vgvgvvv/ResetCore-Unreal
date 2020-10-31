#pragma once

#include "CoreMinimal.h"


namespace CommonLib
{
	class COMMONLIB_API IArg
	{

	};

	class COMMONLIB_API IListener
	{
	public:

		virtual ~IListener() = default;

		virtual IListener& GetParent() { IListener* listener = nullptr; return *listener; };

		virtual const FName& GetEventName() const { FName* name = nullptr; return *name; };

		virtual void OnTrigger(IArg& arg) {};

	public:
		// Operators
		void RegisterListener();

		class WhereListener Where(TFunction<bool(IArg*)> Condition);

		IListener& AddHandler(TFunction<void(IArg*)> Handler);


	};

	class COMMONLIB_API BaseListener : public IListener
	{
		TArray<TFunction<void(IArg*)>> OnTriggerActionList;

	public:

		friend IListener;

		BaseListener(const FName& eventName);

		BaseListener(FName&& eventName);

		IListener& GetParent() override;

		const FName& GetEventName() const override;

		void OnTrigger(IArg& arg) override;
	private:

		FName EventName;
	};

	class COMMONLIB_API WhereListener : public IListener
	{
	public:
		WhereListener(IListener& Parent, TFunction<bool(IArg*)> Condition);

		IListener& GetParent() override;
		const FName& GetEventName() const override;
		void OnTrigger(IArg& arg) override;
	private:
		IListener& Parent;
		TFunction<bool(IArg*)> Condition;
	};
}