#include "Utility/LambdaRunnable.h"
#include "CommonLib.h"

uint64 FLambdaRunnable::ThreadNumber = 0;

FLambdaRunnable::FLambdaRunnable(TFunction< void()> InFunction)
{
	FunctionPointer = InFunction;
	Finished = false;
	Number = ThreadNumber;
	
	FString threadStatGroup = FString::Printf(TEXT("FLambdaRunnable%d"), ThreadNumber);
	Thread = FRunnableThread::Create(this, *threadStatGroup, 0, TPri_BelowNormal); //windows default = 8mb for thread, could specify more
	ThreadNumber++;
}

FLambdaRunnable::~FLambdaRunnable()
{
	if (Thread)
	{
		delete Thread;
		Thread = nullptr;
	}
}

//Init
bool FLambdaRunnable::Init()
{
	UE_LOG(ResetCore_CommonLib, Log, TEXT("FLambdaRunnable %d Init"), Number);
	return true;
}

//Run
uint32 FLambdaRunnable::Run()
{
	
	
	if (FunctionPointer)
		FunctionPointer();
	

	UE_LOG(ResetCore_CommonLib, Log, TEXT("FLambdaRunnable %d Run complete"), Number);
	return 0;
}

//stop
void FLambdaRunnable::Stop()
{
	Finished = true;
}

void FLambdaRunnable::Exit()
{
	Stop();
	UE_LOG(ResetCore_CommonLib, Log, TEXT("FLambdaRunnable %d Exit"), Number);
}


void FLambdaRunnable::EnsureCompletion()
{
	Stop();
	Thread->WaitForCompletion();
}

FLambdaRunnable* FLambdaRunnable::RunLambdaOnBackGroundThread(TFunction< void()> InFunction)
{
	FLambdaRunnable* Runnable;
	if (FPlatformProcess::SupportsMultithreading())
	{
		Runnable = new FLambdaRunnable(InFunction);
		//UE_LOG(LogClass, Log, TEXT("FLambdaRunnable RunLambdaBackGroundThread"));
		return Runnable;
	}
	else 
	{
		return nullptr;
	}
}