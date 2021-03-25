#pragma once
#include "Templates/SharedPointer.h"

class FJsonObject;
class FJsonValue;



class COMMONLIB_API FJsonUtil
{
public:
	
	/// 将JsonObject 转换为字符串
	static FString JsonObjectToString(TSharedPtr<FJsonObject> JSONObject);

	///将字符串转换为JsonObject
	static TSharedPtr<FJsonObject> StringToJsonObject(const FString& JSONString);

	static FString JsonValueToString(TSharedPtr<FJsonValue> JSONValue, const FString& Identifier);

	static TSharedPtr<FJsonValue> StringToJsonValue(const FString& JSONString);
};
