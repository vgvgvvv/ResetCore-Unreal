#include "Network/Serializer/FNetJsonSerializer.h"

#include <string>


#include "CommonLib.h"
#include "JsonSerializer.h"
#include "JsonUtil.h"
#include "JsonWriter.h"

void FNetJsonSerializer::Serialize(TSharedPtr<FJsonObject> json, TArray<uint8>& data)
{
	FString jsonStr;
	TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&jsonStr);
	FJsonSerializer::Serialize(json.ToSharedRef(), Writer);

	auto size =sizeof(ANSICHAR)*(jsonStr.Len() + 1);
	std::string jsonstdstr(TCHAR_TO_ANSI(*jsonStr));

	data.Empty(size);
	for(int i = 0; i < size ; i ++)
	{
		data.Emplace(jsonstdstr[i]);
	}
}

TSharedPtr<FJsonObject> FNetJsonSerializer::DeSerialize(TArray<uint8>& data)
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