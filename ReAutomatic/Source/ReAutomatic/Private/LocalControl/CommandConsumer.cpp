#include "LocalControl/CommandConsumer.h"
#include "LocalControl/AutomaticManager.h"
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
	FJsonArg Result;
	if(ParseCommandString(ReceivedStr, Result))
	{
		JsonQueue.Enqueue(Result);
	}
}

void FCommandConsumer::SendCommandToClient(IArg* Arg)
{

	FAutomaticManager::Get().SendCommandToClient(GetSendString(*(FJsonArg*)Arg));
}


void FCommandConsumer::Tick(float DeltaTime)
{
	while(!JsonQueue.IsEmpty())
	{
		FJsonArg JsonArg;
		JsonQueue.Dequeue(JsonArg);
		FEventDispatcher::TriggerEvent(FName("AutomaticEvent"), JsonArg);
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

bool FCommandConsumer::ParseCommandString(const FString& CommandStr, FJsonArg& JsonArg) const
{
	TSharedPtr<FJsonObject> JsonObject;
	TSharedRef< TJsonReader<TCHAR> > Reader = TJsonReaderFactory<TCHAR>::Create(CommandStr);

	int CmdId;

	if (FJsonSerializer::Deserialize(Reader, JsonObject) == false)
	{
		return false;
	}

	if (JsonObject->TryGetNumberField(TEXT("cmd"),   CmdId) == false)
	{
		return false;
	}

	JsonArg = FJsonArg(CmdId, JsonObject);

	return true;
}

FString FCommandConsumer::GetSendString(const FJsonArg& Arg)
{
	typedef TJsonWriter<TCHAR, TPrettyJsonPrintPolicy<TCHAR>> FStringWriter;
	typedef TJsonWriterFactory<TCHAR, TPrettyJsonPrintPolicy<TCHAR>> FStringWriterFactory;

	FString CopyStr;
	TSharedRef<FStringWriter> Writer = FStringWriterFactory::Create(&CopyStr);
	Writer->WriteValue("cmd", Arg.CmdId);

	FJsonSerializer::Serialize(Arg.JsonObject.ToSharedRef(), Writer);

	return CopyStr;
}
