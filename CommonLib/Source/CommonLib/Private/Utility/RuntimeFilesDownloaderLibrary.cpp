// Respirant 2020.
#include "Utility/RuntimeFilesDownloaderLibrary.h"
#include "Http.h"
#include "Misc/Paths.h"
#include "HAL/PlatformFilemanager.h"
#include "GenericPlatform/GenericPlatformFile.h"

URuntimeFilesDownloaderLibrary* URuntimeFilesDownloaderLibrary::CreateDownloader()
{
	URuntimeFilesDownloaderLibrary* Downloader = NewObject<URuntimeFilesDownloaderLibrary>();
	return Downloader;
}

URuntimeFilesDownloaderLibrary* URuntimeFilesDownloaderLibrary::DownloadFile(const FString& URL, FString SavePath)
{
	FileUrl = URL;
	FileSavePath = SavePath;

#if ENGINE_MINOR_VERSION > 22
	TSharedRef<IHttpRequest, ESPMode::ThreadSafe> HttpRequest = FHttpModule::Get().CreateRequest();
#else
	TSharedRef<IHttpRequest> HttpRequest = FHttpModule::Get().CreateRequest();
#endif
	
	HttpRequest->SetVerb("GET");
	HttpRequest->SetURL(URL);
	HttpRequest->OnProcessRequestComplete().BindUObject(this, &URuntimeFilesDownloaderLibrary::OnReady_Internal);
	HttpRequest->OnRequestProgress().BindUObject(this, &URuntimeFilesDownloaderLibrary::OnProgress_Internal);

	// Execute the request
	HttpRequest->ProcessRequest();
	AddToRoot();

	return this;
}

void URuntimeFilesDownloaderLibrary::OnProgress_Internal(FHttpRequestPtr Request, int32 BytesSent, int32 BytesReceived)
{
	FHttpResponsePtr Response = Request->GetResponse();
	if (Response.IsValid())
	{
		int32 FullSize = Response->GetContentLength();
		OnProgress.Broadcast(BytesSent, BytesReceived, FullSize);
		OnProgressCpp.Broadcast(BytesSent, BytesReceived, FullSize);
	}
}

void URuntimeFilesDownloaderLibrary::OnReady_Internal(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
	RemoveFromRoot();
	Request->OnProcessRequestComplete().Unbind();

	if (Response.IsValid() && EHttpResponseCodes::IsOk(Response->GetResponseCode()))
	{
		// Save file
		IPlatformFile& PlatformFile = FPlatformFileManager::Get().GetPlatformFile();

		// Create save directory if not existent
		FString Path, Filename, Extension;
		FPaths::Split(FileSavePath, Path, Filename, Extension);
		if (!PlatformFile.DirectoryExists(*Path))
		{
			if (!PlatformFile.CreateDirectoryTree(*Path))
			{
				OnResult.Broadcast(DownloadResult::DirectoryCreationFailed);
				OnResultCpp.Broadcast(DownloadResult::DirectoryCreationFailed);
				return;
			}
		}

		// Open/Create the file
		IFileHandle* FileHandle = PlatformFile.OpenWrite(*FileSavePath);
		if (FileHandle)
		{
			// Write the file
			FileHandle->Write(Response->GetContent().GetData(), Response->GetContentLength());
			// Close the file
			delete FileHandle;

			OnResult.Broadcast(DownloadResult::SuccessDownloading);
			OnResultCpp.Broadcast(DownloadResult::SuccessDownloading);
		}
		else
		{
			OnResult.Broadcast(DownloadResult::SaveFailed);
			OnResultCpp.Broadcast(DownloadResult::SaveFailed);
		}
	}
	else
	{
		OnResult.Broadcast(DownloadResult::DownloadFailed);
		OnResultCpp.Broadcast(DownloadResult::DownloadFailed);
	}
}
