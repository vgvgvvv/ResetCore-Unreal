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

	TCHAR* pSendData = jsonStr.GetCharArray().GetData();
	uint8* dst = (uint8*)TCHAR_TO_UTF8(pSendData);

	FTCHARToUTF8 EchoStrUtf8(*jsonStr);
	length = EchoStrUtf8.Length();
	
	data.Init(0, length);
	for(int i = 0; i < length ; i ++)
	{
		data[i] = dst[i];
		UE_LOG(LogTemp, Log, TEXT("xxx %x"), data[i])
	}

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