#include "RouteClient/NetPackageHandler/RouteMessageHandler.h"


#include "Utility/EnumUtil.h"
#include "HttpModule.h"
#include "Utility/JsonUtil.h"
#include "ReAutomatic.h"
#include "JsonUtilities.h"
#include "UE4ControlCenter/LuaScriptMessage.h"
#include "RouteClient/RouteProto.h"
#include "Utility/RuntimeFilesDownloaderLibrary.h"
#include "Network/SocketClient.h"
#include "Async/Async.h"
#include "UE4ControlCenter/UE4CmdArg.h"
#include "UE4ControlCenter/UE4ControlCenter.h"
#include "Utility/JsonUtil.h"

static void OnDownloadFileProgress(const int32 BytesSent, const int32 BytesReceived,
    const int32 ContentLength)
{
	UE_LOG(LogAutomatic, Log, TEXT("OnDownloadFileProgress BytesSent: %d, BytesReceived %d, ContentLength: %d"), BytesSent, BytesReceived, ContentLength);
}

static void OnDownloadFileResult(TEnumAsByte<DownloadResult> Result)
{
	UE_LOG(LogAutomatic, Log, TEXT("OnDownloadFileResult %d"), static_cast<int32>(Result.GetValue()));
}


void FRouteMessageHandler::HandleJsonInfo(FSocketClient& from, TSharedPtr<FJsonObject> jsonObject)
{
	FRouteSendMessage msg;
	FJsonObjectConverter::JsonObjectToUStruct<FRouteSendMessage>(jsonObject.ToSharedRef(), &msg);

	UE_LOG(LogAutomatic, Log, TEXT("recv from %s (%s) %s"), *msg.FromRouteName,
		*FEnumUtil::EnumToString(TEXT("ERouteType"), msg.FromRouteType),
		*FJsonUtil::JsonObjectToString(jsonObject))

	FRemoteControllerInfo controllerInfo;
	controllerInfo.ControllerName = msg.FromRouteName;
	controllerInfo.ControllerType = msg.FromRouteType;
	
	auto ContentObject = jsonObject->GetObjectField("Content");
	// RawCommand
	auto CmdId = ContentObject->GetStringField("CmdId");
	auto RawCommandContent = ContentObject->GetField<EJson::None>("Content");

	if(CmdId == FCmdTypes::Log)
	{
		auto LogStr = RawCommandContent->AsString();
		UE_LOG(LogAutomatic, Log, TEXT("Log From Route Server : %s"), *LogStr)
	}
	else if (CmdId == FCmdTypes::UE4Msg)
	{
		auto UE4MessageObject = RawCommandContent->AsObject();
		
		auto Id = UE4MessageObject->GetStringField("CmdId");
		auto Index = UE4MessageObject->GetIntegerField("Index");
		auto CmdContent = UE4MessageObject->GetField<EJson::None>("Content");

		FUE4CmdArg Arg;
		Arg.CmdId = Id;
		Arg.Index = Index;
		Arg.Content = CmdContent;
		Arg.ControllerInfo = controllerInfo;

		AsyncTask(ENamedThreads::GameThread, [Id, Arg]()
                                             		{
                                             			FUE4CmdArg Temp = Arg;
                                             			FName Name = FName(*Id);
                                             			FUE4ControlCenter::Get().Trigger(Name, Temp);
                                             		});
	}
	else if (CmdId == FCmdTypes::RunLua)
	{
		auto RunLuaMessageObject = RawCommandContent->AsObject();
		auto Index = RunLuaMessageObject->GetIntegerField("Index");
		auto LuaStr = RunLuaMessageObject->GetStringField("LuaScript");

		FLuaScriptMessage Message;
		Message.Index = Index;
		Message.LuaScript = LuaStr;

		AsyncTask(ENamedThreads::GameThread, [Message, controllerInfo]()
                                             		{
                                             			FUE4ControlCenter::Get().OnRunLua.Broadcast(Message, controllerInfo);
                                             		});
	}
	else if(CmdId == FCmdTypes::SendFile)
	{
		auto SendFileMessageObject = RawCommandContent->AsObject();
		auto FileSize = SendFileMessageObject->GetIntegerField("FileSize");
		auto FileName = SendFileMessageObject->GetStringField("FileName");
		auto TargetPath = SendFileMessageObject->GetStringField("TargetPath");
		auto FileServerUrl = SendFileMessageObject->GetStringField("FileServerUrl");

		URuntimeFilesDownloaderLibrary* downloader = URuntimeFilesDownloaderLibrary::CreateDownloader();
		auto RequestUrl = FString::Printf(TEXT("%s/api/file/download/%s/%s"), *FileServerUrl, *msg.TargetRouteName, *FileName);
		UE_LOG(LogAutomatic, Log, TEXT("Download From %s"), *RequestUrl)
		downloader->OnProgressCpp.AddStatic(&OnDownloadFileProgress);
		downloader->OnResultCpp.AddStatic(&OnDownloadFileResult);
		
		downloader->DownloadFile(RequestUrl, TargetPath);
	}
	else
	{
		UE_LOG(LogAutomatic, Error, TEXT("Not Support Cmdid : %s"), *CmdId)
	}
}

