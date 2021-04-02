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

	length = sizeof(ANSICHAR)*(jsonStr.Len() + 1);
	std::string jsonstdstr(TCHAR_TO_ANSI(*jsonStr));

	data.Empty(length);
	for(int i = 0; i < length ; i ++)
	{
		data.Emplace(jsonstdstr[i]);
	}
}

TSharedPtr<FJsonObject> FNetJsonSerializer::DeSerialize(TArray<uint8>& data, int length)
{
	data.Add('\0');
	auto jsonString = FString(length, ANSI_TO_TCHAR(reinterpret_cast<const char*>(data.GetData())));

	TSharedRef< TJsonReader<> > Reader = TJsonReaderFactory<>::Create( jsonString );
	TSharedPtr<FJsonObject> Object;
	if(!FJsonSerializer::Deserialize(Reader, Object))
	{
		UE_LOG(ResetCore_CommonLib, Error, TEXT("Cannot Parse Json!!! %s"), *jsonString)
		return nullptr;
	}
	return Object; 
}