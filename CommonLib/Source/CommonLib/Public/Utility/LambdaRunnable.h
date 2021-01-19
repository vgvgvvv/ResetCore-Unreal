#pragma once
#include "Runnable.h"
#include "RunnableThread.h"
#include "TaskGraphInterfaces.h"
#include "ThreadSafeBool.h"

/*
Long duration lambda wrapper, which are generally not supported by the taskgraph system. New thread per lambda and they will auto-delete upon
completion.
*/
class COMMONLIB_API FLambdaRunnable : public FRunnable
{
private:
	/** Thread to run the worker FRunnable on */
	FRunnableThread* Thread;
	uint64 Number;

	//Lambda function pointer
	TFunction< void()> FunctionPointer;

	/** Use this thread-safe boolean to allow early exits for your threads */
	FThreadSafeBool Finished;

	//static TArray<FLambdaRunnable*> Runnables;
	static uint64 ThreadNumber;

public:
	//~~~ Thread Core Functions ~~~

	//Constructor / Destructor
	FLambdaRunnable(TFunction< void()> InFunction);
	virtual ~FLambdaRunnable();

	// Begin FRunnable interface.
	virtual bool Init();
	virtual uint32 Run();
	virtual void Stop();
	virtual void Exit() override;
	// End FRunnable interface

	/** Makes sure this thread has stopped properly */
	void EnsureCompletion();

	/*
	Runs the passed lambda on the background thread, new thread per call
	*/
	static FLambdaRunnable* RunLambdaOnBackGroundThread(TFunction< void()> InFunction);
};

//Using TaskGraph
// FGraphEventRef RunLambdaOnGameThread(TFunction< void()> InFunction)
// {
// 	return FFunctionGraphTask::CreateAndDispatchWhenReady(InFunction, TStatId(), nullptr, ENamedThreads::GameThread);
// }
//
// FGraphEventRef RunLambdaOnAnyThread(TFunction< void()> InFunction)
// {
// 	return FFunctionGraphTask::CreateAndDispatchWhenReady(InFunction, TStatId(), nullptr, ENamedThreads::AnyThread);
// }
//
// //Uses proper threading, for any task that may run longer than about 2 seconds.
// void RunLongLambdaOnAnyThread(TFunction< void()> InFunction)
// {
// 	FLambdaRunnable::RunLambdaOnBackGroundThread(InFunction);
// }