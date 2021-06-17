#include "WeComUtil.h"

#include "HttpModule.h"
#include "JsonObject.h"
#include "JsonObjectConverter.h"
#include "JsonSerializer.h"
#include "JsonUtil.h"
#include "Utility/JsonUtil.h"

class IHttpRequest;

FString UWeComUtil::WebHook = "https://qyapi.weixin.qq.com/cgi-bin/webhook/send?key=%s";

FString UWeComUtil::WeComKey;


TArray<uint8> UWeComUtil::ToUTF8(const FString& msg)
{
	TArray<uint8> data;
    const TCHAR* pSendData = *msg;

    FTCHARToUTF8 EchoStrUtf8(pSendData);
    auto length = EchoStrUtf8.Length();
    
    data.SetNum(length);
    memcpy(data.GetData(), EchoStrUtf8.Get(), length);
	return data;
}

void UWeComUtil::SetKey(const FString&  Key)
{
	WeComKey = Key;
}

void UWeComUtil::SendTextMessage(const FTextContent& TextContent)
{
	auto Http = &FHttpModule::Get();
	TSharedRef<IHttpRequest> Request = Http->CreateRequest();
	// Request->OnProcessRequestComplete().BindUObject(this, &AHttpActor::OnResponseReceived);
	//This is the url on which to process the request

	auto Url = FString::Printf(*WebHook, *WeComKey);

	UE_LOG(LogTemp, Log, TEXT("Current Url Is %s"), *Url)
	
	Request->SetURL(Url);
	Request->SetVerb("POST");
	Request->SetHeader("Content-Type", TEXT("application/json"));

	TSharedPtr<FJsonObject> Content = MakeShared<FJsonObject>();
	Content->SetStringField("msgtype", "text");
	TSharedPtr<FJsonObject> TextContentObj = MakeShared<FJsonObject>();
	TSharedPtr<FJsonObject> TempObj = 	FJsonObjectConverter::UStructToJsonObject(TextContent);

	TextContentObj->SetStringField("content", TextContent.Content);
	TextContentObj->SetArrayField("mentioned_list",  TempObj->GetArrayField("mentionedList"));
	TextContentObj->SetArrayField("mentioned_mobile_list",  TempObj->GetArrayField("mentionedMobileList"));
	
	Content->SetObjectField("text", TextContentObj);

	auto JsonStr  =FJsonUtil::JsonObjectToString(Content);
	
	UE_LOG(LogTemp, Log, TEXT("Current Json Is %s"), *JsonStr);
	
	Request->SetContent(ToUTF8(JsonStr));
	Request->ProcessRequest();
}

void UWeComUtil::SendMarkdownMessage(const FMarkdownContent MarkdownContent)
{

	auto Http = &FHttpModule::Get();
	TSharedRef<IHttpRequest> Request = Http->CreateRequest();
	// Request->OnProcessRequestComplete().BindUObject(this, &AHttpActor::OnResponseReceived);
	//This is the url on which to process the request

	auto Url = FString::Printf(*WebHook, *WeComKey);

	UE_LOG(LogTemp, Log, TEXT("Current Url Is %s"), *Url)
	
	Request->SetURL(Url);
	Request->SetVerb("POST");
	Request->SetHeader("Content-Type", TEXT("application/json"));

	TSharedPtr<FJsonObject> Content = MakeShared<FJsonObject>();
	Content->SetStringField("msgtype", "markdown");
	TSharedPtr<FJsonObject> TextContentObj = MakeShared<FJsonObject>();
	TSharedPtr<FJsonObject> TempObj = 	FJsonObjectConverter::UStructToJsonObject(MarkdownContent);

	TextContentObj->SetStringField("content", MarkdownContent.Content);
	
	Content->SetObjectField("markdown", TextContentObj);

	auto JsonStr  =FJsonUtil::JsonObjectToString(Content);
	
	UE_LOG(LogTemp, Log, TEXT("Current Json Is %s"), *JsonStr);
	
	Request->SetContent(ToUTF8(JsonStr));
	Request->ProcessRequest();
	
}
