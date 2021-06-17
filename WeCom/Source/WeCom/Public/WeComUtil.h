#pragma once
#include "Kismet/BlueprintFunctionLibrary.h"
#include "WeComUtil.generated.h"


USTRUCT()
struct WECOM_API FTextContent
{
	GENERATED_BODY()
public:
	
	UPROPERTY()
	FString Content;

	UPROPERTY()
	TArray<FString> MentionedList;

	UPROPERTY()
	TArray<FString> MentionedMobileList;
};

USTRUCT()
struct WECOM_API FMarkdownContent
{
	GENERATED_BODY()
	public:
	
	UPROPERTY()
	FString Content;

};

UCLASS()
class WECOM_API UWeComUtil : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:
	static FString WebHook;

	static void SetKey(const FString& Key);
	
	static void SendTextMessage(const FTextContent& TextContent);

	static void SendMarkdownMessage(const FMarkdownContent MarkdownContent);

private:

	static TArray<uint8> ToUTF8(const FString& msg);
	
	static FString WeComKey;
};
