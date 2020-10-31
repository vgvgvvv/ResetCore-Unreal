#pragma once

#include "CoreMinimal.h"


namespace CommonLib
{
	class IArg
	{

	};

	class IListener
	{
	public:

		virtual ~IListener() = default;

		virtual IListener& GetParent() = 0;

		virtual const FName& GetEventName() const = 0;

		virtual void OnTrigger(IArg& arg) = 0;

	public:
		// Operators
		void RegisterListener();

		class Where Where(TFunction<bool(IArg&)> Condition);

		IListener& AddHandler(TFunction<void(IArg&)> Handler);


	};

	class BaseListener : public IListener
	{
		TArray<TFunction<void(IArg)>> OnTriggerActionList;

	public:

		friend IListener;

		BaseListener(FName& eventName);

		BaseListener(FName&& eventName);

		IListener& GetParent() override;

		const FName& GetEventName() const override;

		void OnTrigger(IArg& arg) override;
	private:

		FName EventName;
	};

	class Where : public IListener
	{
	public:
		Where(IListener& Parent, TFunction<bool(IArg&)> Condition);

		IListener& GetParent() override;
		const FName& GetEventName() const override;
		void OnTrigger(IArg& arg) override;
	private:
		IListener& Parent;
		TFunction<bool(IArg&)> Condition;
	};
}