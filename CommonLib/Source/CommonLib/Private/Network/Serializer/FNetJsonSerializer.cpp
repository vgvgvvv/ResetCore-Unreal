#include "Network/Serializer/FNetJsonSerializer.h"

#include <string>


#include "CommonLib.h"
#include "Serialization/JsonSerializer.h"
#include "Serialization/JsonWriter.h"

void FNetJsonSerializer::Serialize(TSharedPtr<FJsonObject> json, TArray<uint8>& data, int& length)
{
	FString jsonStr;
	TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&jsonStr);
	FJsonSerializer::Serialize(json.ToSharedRef(), Writer);

	const TCHAR* pSendData = *jsonStr;

	FTCHARToUTF8 EchoStrUtf8(pSendData);
	length = EchoStrUtf8.Length();
	
	data.SetNum(length);
	memcpy(data.GetData(), EchoStrUtf8.Get(), length);

	UE_LOG(LogTemp, Log, TEXT("Json Data : str : %s hex : %s"), *jsonStr, *BytesToHex(data.GetData(), length))
}

TSharedPtr<FJsonObject> FNetJsonSerializer::DeSerialize(TArray<uint8>& data, int length)
{
	data.Add('\0');
	auto jsonString = FString(UTF8_TO_TCHAR(data.GetData()));

	TSharedRef< TJsonReader<> > Reader = TJsonReaderFactory<>::Create( jsonString );
	TSharedPtr<FJsonObject> Object;
	if(!FJsonSerializer::Deserialize(Reader, Object))
	{
		UE_LOG(ResetCore_CommonLib, Error, TEXT("Cannot Parse Json!!! %s"), *jsonString)
		return nullptr;
	}
	return Object; 
}