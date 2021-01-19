#pragma once
#include "SharedPointer.h"

class FJsonObject;



class COMMONLIB_API FJsonUtil
{
public:
	
	/// 将JsonObject 转换为字符串
	static FString JsonObjectToString(TSharedPtr<FJsonObject> jonObject);

	///将字符串转换为JsonObject
	static TSharedPtr<FJsonObject> StringToJsonObject(FString& jsonString);
};
