#pragma once

#include "JsonObject.h"
#include "ObjectMacros.h"
#include "RouteProto.generated.h"


UENUM()
enum class ERouteType : uint8
{
	RouteServer = 0,
    LocalController = 1,
    MobileDevice = 2, 
    UnrealEngine = 3,
    WebService = 4,
    Max = WebService        
};

UENUM()
enum class ERouteProtoID  : uint8 {
	Regist = 0,
	NormalMessage = 1,
	Close = 2,        
	HeartBeat = 3,   
	Log = 4,		  
	ListDevice = 5,	  
	Max = ListDevice         
};

USTRUCT()
struct REAUTOMATIC_API FRouteRegistMessage
{
	GENERATED_BODY()

	UPROPERTY()
	FString RegistName;

	UPROPERTY()
	ERouteType RegistType;
};

USTRUCT()
struct REAUTOMATIC_API FRouteCloseMessage
{
	GENERATED_BODY()
	
	UPROPERTY()
	FString RegistName;

	UPROPERTY()
	ERouteType RegistType;
};

USTRUCT()
struct REAUTOMATIC_API FRawCommandMessage
{
	GENERATED_BODY()
	
	UPROPERTY()
	FString CmdId;

	TSharedPtr<FJsonValue> Content;
};


USTRUCT()
struct REAUTOMATIC_API FRouteSendMessage
{
	GENERATED_BODY()
	
	UPROPERTY()
	ERouteType FromRouteType;

	UPROPERTY()
	FString FromRouteName;
	
	UPROPERTY()
	ERouteType TargetRouteType;

	UPROPERTY()
	FString TargetRouteName;

	FRawCommandMessage Content;
	
};

USTRUCT()
struct REAUTOMATIC_API FRemoteControllerInfo
{
	GENERATED_BODY()

	UPROPERTY()
	FString ControllerName;

	UPROPERTY()
	ERouteType ControllerType;
};

USTRUCT()
struct REAUTOMATIC_API FCmdTypes
{
	GENERATED_BODY()
	
	using FCmdType=FString;

	static FCmdType Log;
	static FCmdType UE4Msg;
	static FCmdType UE4MsgResult;
	static FCmdType ListDevice;
	static FCmdType Shell;
	static FCmdType DownloadFile;
	static FCmdType RunLua;
	static FCmdType RunLuaFinish;
	static FCmdType SendFile;
	static FCmdType SendFileFinish;
};

