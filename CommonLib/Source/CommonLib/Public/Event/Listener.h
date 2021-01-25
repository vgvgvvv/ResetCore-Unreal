#pragma once

#include "CoreMinimal.h"

class FEventDispatcher;

class COMMONLIB_API IArg
{

};

class COMMONLIB_API IListener
{
public:

	IListener();
	
	virtual ~IListener() = default;

	virtual IListener& GetParent() { IListener* listener = nullptr; return *listener; };

	virtual const FName& GetEventName() const { FName* name = nullptr; return *name; };

	virtual void OnTrigger(IArg& arg) {};


	friend bool operator==(const IListener& Lhs, const IListener& RHS)
	{
		return Lhs.Index == RHS.Index;
	}

	friend bool operator!=(const IListener& Lhs, const IListener& RHS)
	{
		return !(Lhs == RHS);
	}

	// Operators
	const IListener* RegisterListener(FEventDispatcher& EventDispatcher);

	class WhereListener Where(TFunction<bool(IArg*)> Condition);

	IListener& AddHandler(TFunction<void(IArg*)> Handler);

private:
	int32 Index;
	static int32 CurrentIndex;
};

class COMMONLIB_API BaseListener : public IListener
{
	TArray<TFunction<void(IArg*)>> OnTriggerActionList;

public:

	friend IListener;

	BaseListener(const FName& eventName);

	BaseListener(FName&& eventName);

	virtual IListener& GetParent() override;

	virtual const FName& GetEventName() const override;

	virtual void OnTrigger(IArg& arg) override;
private:

	FName EventName;
};

class COMMONLIB_API WhereListener : public IListener
{
public:
	WhereListener(IListener& Parent, TFunction<bool(IArg*)> Condition);

	virtual IListener& GetParent() override;
	virtual const FName& GetEventName() const override;
	virtual void OnTrigger(IArg& arg) override;
private:
	IListener& Parent;
	TFunction<bool(IArg*)> Condition;
};