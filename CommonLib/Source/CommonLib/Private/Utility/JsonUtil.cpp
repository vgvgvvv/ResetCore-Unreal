#include "Utility/JsonUtil.h"

#include "JsonSerializer.h"
#include "JsonWriter.h"
#include "CommonLib.h"

using FJsonObjectPtr = TSharedPtr<FJsonObject>;
using FJsonWriterRef = TSharedRef<TJsonWriter<>>;
using FJsonReaderRef = TSharedRef<TJsonReader<>>;

FString FJsonUtil::JsonObjectToString(FJsonObjectPtr jonObject)
{
	FString jsonStr;
	FJsonWriterRef Writer = TJsonWriterFactory<>::Create(&jsonStr);
	FJsonSerializer::Serialize(jonObject.ToSharedRef(), Writer);
	return jsonStr;
}

FJsonObjectPtr FJsonUtil::StringToJsonObject(FString& jsonString)
{
	FJsonReaderRef Reader = TJsonReaderFactory<>::Create(jsonString);
	FJsonObjectPtr Object;
	if (!FJsonSerializer::Deserialize(Reader, Object))
	{
		UE_LOG(ResetCore_CommonLib, Error, TEXT("Cannot Parse Json!!!"))
		return nullptr;
	}
	return Object;
}
