#include "CommandConsumer.h"
#include "AutomaticManager.h"
#include "EventDispatcher.h"
#include "JsonSerializer.h"

using namespace CommonLib;


FCommandConsumer::FCommandConsumer()
{
	FEventDispatcher::CreateListener(FName("SendMessage"))
		.AddHandler(SendCommandToClient)
		.RegisterListener();
}

FCommandConsumer::~FCommandConsumer()
{
	FEventDispatcher::RemoveListener(FName("SendMessage"));
}

void FCommandConsumer::SetEnable(bool Enable)
{
	IsEnable = Enable;
}


void FCommandConsumer::PushCommand(FString ReceivedStr)
{
	FCommand Result;
	if(ParseCommandString(ReceivedStr, Result))
	{
		CommandQueue.Enqueue(Result);
	}
}

void FCommandConsumer::SendCommandToClient(const FJsonArg& Command) const
{
	FAutomaticManager::Get().SendCommandToClient(GetSendString(Command));
}


void FCommandConsumer::Tick(float DeltaTime)
{
	while(!CommandQueue.Empty())
	{
		FCommand Command;
		CommandQueue.Dequeue(Command);
		FEventDispatcher::TriggerEvent(FName("AutomaticEvent"), Command);
	}
}

bool FCommandConsumer::IsTickable() const
{
	return IsEnable;
}

TStatId FCommandConsumer::GetStatId() const
{
	return TStatId();
}

bool FCommandConsumer::ParseCommandString(const FString& CommandStr, FCommand& Command) const
{
	TSharedPtr<FJsonObject> JsonObject;
	TSharedRef< TJsonReader<TCHAR> > Reader = TJsonReaderFactory<TCHAR>::Create(CommandStr);

	int CmdId;
	FString CmdStr;

	if (FJsonSerializer::Deserialize(Reader, JsonObject) == false)
	{
		return false;
	}

	if (JsonObject->TryGetNumberField(TEXT("cmd"),   CmdId) == false)
	{
		return false;
	}

	if (JsonObject->TryGetStringField(TEXT("value"), CmdStr) == false)
	{
		return false;
	}

	Command = FCommand(CmdId, CmdStr);

	return true;
}

FString FCommandConsumer::GetSendString(const FJsonArg& Arg) const
{
	typedef TJsonWriter<TCHAR, TPrettyJsonPrintPolicy<TCHAR>> FStringWriter;
	typedef TJsonWriterFactory<TCHAR, TPrettyJsonPrintPolicy<TCHAR>> FStringWriterFactory;

	FString CopyStr;
	TSharedRef<FStringWriter> Writer = FStringWriterFactory::Create(&CopyStr);
	FJsonSerializer::Serialize(Arg.JsonObject, Writer);

	return CopyStr;
}
