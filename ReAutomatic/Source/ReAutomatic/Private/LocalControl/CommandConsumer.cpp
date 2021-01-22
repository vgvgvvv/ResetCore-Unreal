// #include "LocalControl/CommandConsumer.h"
// #include "LocalControl/AutomaticManager.h"
// #include "EventDispatcher.h"
// #include "JsonSerializer.h"
// #include "UE4ControlCenter/UE4CmdArg.h"
// #include "UE4ControlCenter/UE4ControlCenter.h"
//
// using namespace CommonLib;
//
//
// FCommandConsumer::FCommandConsumer()
// {
// 	FEventDispatcher::CreateListener(FName("SendMessage"))
// 		.AddHandler(SendCommandToClient)
// 		.RegisterListener(EventDispatcher);
// }
//
// FCommandConsumer::~FCommandConsumer()
// {
// 	EventDispatcher.RemoveAllListener(FName("SendMessage"));
// }
//
// void FCommandConsumer::SetEnable(bool Enable)
// {
// 	IsEnable = Enable;
// }
//
//
// void FCommandConsumer::PushCommand(FString ReceivedStr)
// {
// 	FUE4CmdArg Result;
// 	if(ParseCommandString(ReceivedStr, Result))
// 	{
// 		JsonQueue.Enqueue(Result);
// 	}
// }
//
// void FCommandConsumer::SendCommandToClient(IArg* Arg)
// {
// 	FAutomaticManager::Get().SendCommandToClient(GetSendString(*static_cast<FUE4CmdArg*>(Arg)));
// }
//
//
// void FCommandConsumer::Tick(float DeltaTime)
// {
// 	while(!JsonQueue.IsEmpty())
// 	{
// 		FUE4CmdArg JsonArg;
// 		JsonQueue.Dequeue(JsonArg);
// 		EventDispatcher.TriggerEvent(FName("AutomaticEvent"), JsonArg);
// 	}
// }
//
// bool FCommandConsumer::IsTickable() const
// {
// 	return IsEnable;
// }
//
// TStatId FCommandConsumer::GetStatId() const
// {
// 	return TStatId();
// }
//
// bool FCommandConsumer::ParseCommandString(const FString& CommandStr, FUE4CmdArg& JsonArg) const
// {
// 	TSharedPtr<FJsonObject> JsonObject;
// 	TSharedRef< TJsonReader<TCHAR> > Reader = TJsonReaderFactory<TCHAR>::Create(CommandStr);
//
// 	FString CmdId;
//
// 	if (FJsonSerializer::Deserialize(Reader, JsonObject) == false)
// 	{
// 		return false;
// 	}
//
// 	if (JsonObject->TryGetStringField(TEXT("cmd"),   CmdId) == false)
// 	{
// 		return false;
// 	}
//
// 	TSharedPtr<FJsonValueObject> JsonValue = MakeShared<FJsonValueObject>(JsonObject);
// 	JsonArg = FUE4CmdArg(CmdId, JsonValue);
//
// 	return true;
// }
//
// FString FCommandConsumer::GetSendString(const FUE4CmdArg& Arg)
// {
// 	typedef TJsonWriter<TCHAR, TPrettyJsonPrintPolicy<TCHAR>> FStringWriter;
// 	typedef TJsonWriterFactory<TCHAR, TPrettyJsonPrintPolicy<TCHAR>> FStringWriterFactory;
//
// 	FString CopyStr;
// 	TSharedRef<FStringWriter> Writer = FStringWriterFactory::Create(&CopyStr);
// 	Writer->WriteValue("cmd", Arg.CmdId);
//
// 	auto Object = Arg.Content->AsObject();
// 	FJsonSerializer::Serialize(Object.ToSharedRef(), Writer);
//
// 	return CopyStr;
// }
