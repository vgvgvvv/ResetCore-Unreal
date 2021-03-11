#pragma once
#include "JsonPackageHandler.h"

enum DownloadResult;

class FRouteMessageHandler : public FJsonPackageHandler
{
public:

	virtual void HandleJsonInfo(FSocketClient& from, TSharedPtr<FJsonObject> jsonObject) override;

private:

	void OnDownloadFileProgress(const int32 BytesSent, const int32 BytesReceived, const int32 ContentLength);
	void OnDownloadFileResult(TEnumAsByte<DownloadResult> Result);
};
