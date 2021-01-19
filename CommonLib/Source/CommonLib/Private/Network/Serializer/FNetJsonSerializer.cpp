#include "Network/Serializer/FNetJsonSerializer.h"


#include "CommonLib.h"
#include "Runtime/JsonUtilities/Public/JsonObjectConverter.h"

void FNetJsonSerializer::SerializeJson(TSharedPtr<FJsonObject> json, TArray<uint8>& data)
{
	FString jsonStr;
	TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&jsonStr);
	FJsonSerializer::Serialize(json.ToSharedRef(), Writer);

	auto bytes = (uint8*)TCHAR_TO_ANSI(*jsonStr);
	auto size =sizeof(ANSICHAR)*(jsonStr.Len() + 1);
	data.Empty(size);
	for(int i = 0; i < size ; i ++)
	{
		data.Add(bytes[i]);
	}
	
}

TSharedPtr<FJsonObject> FNetJsonSerializer::DeSerializeJson(TArray<uint8>& data)
{
	data.Add(0);
	auto jsonString = FString(ANSI_TO_TCHAR(reinterpret_cast<const char*>(data.GetData())));

	TSharedRef< TJsonReader<> > Reader = TJsonReaderFactory<>::Create( jsonString );
	TSharedPtr<FJsonObject> Object;
	if(!FJsonSerializer::Deserialize(Reader, Object))
	{
		UE_LOG(ResetCore_CommonLib, Error, TEXT("Cannot Parse Json!!!"))
		return nullptr;
	}
	return Object; 
}