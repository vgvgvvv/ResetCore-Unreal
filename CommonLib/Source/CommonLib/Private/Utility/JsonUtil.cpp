#include "Utility/JsonUtil.h"

#include "JsonSerializer.h"
#include "JsonWriter.h"
#include "CommonLib.h"

using FJsonObjectPtr = TSharedPtr<FJsonObject>;
using FJsonWriterRef = TSharedRef<TJsonWriter<>>;
using FJsonReaderRef = TSharedRef<TJsonReader<>>;

FString FJsonUtil::JsonObjectToString(FJsonObjectPtr JSONObject)
{
	FString jsonStr;
	FJsonWriterRef Writer = TJsonWriterFactory<>::Create(&jsonStr);
	FJsonSerializer::Serialize(JSONObject.ToSharedRef(), Writer);
	return jsonStr;
}

FJsonObjectPtr FJsonUtil::StringToJsonObject(const FString& JSONString)
{
	FJsonReaderRef Reader = TJsonReaderFactory<>::Create(JSONString);
	FJsonObjectPtr Object;
	if (!FJsonSerializer::Deserialize(Reader, Object))
	{
		UE_LOG(ResetCore_CommonLib, Error, TEXT("Cannot Parse Json!!!"))
		return nullptr;
	}
	return Object;
}

FString FJsonUtil::JsonValueToString(TSharedPtr<FJsonValue> JSONValue, const FString& Identifier)
{
	FString jsonStr;
	FJsonWriterRef Writer = TJsonWriterFactory<>::Create(&jsonStr);
	if(!FJsonSerializer::Serialize(JSONValue, Identifier, Writer))
	{
		return jsonStr;
	}
	return jsonStr;
}

TSharedPtr<FJsonValue> FJsonUtil::StringToJsonValue(const FString& JSONString)
{
	FJsonReaderRef Reader = TJsonReaderFactory<>::Create(JSONString);
	TSharedPtr<FJsonValue> Object;
	if (!FJsonSerializer::Deserialize(Reader, Object))
	{
		UE_LOG(ResetCore_CommonLib, Error, TEXT("Cannot Parse Json!!!"))
			return nullptr;
	}
	return Object;
}
