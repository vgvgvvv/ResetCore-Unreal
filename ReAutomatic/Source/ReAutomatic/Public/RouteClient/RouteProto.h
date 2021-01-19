#pragma once
#include "ObjectMacros.h"
#include "UnrealString.h"
#include "RouteProto.generated.h"

UENUM()
enum class REAUTOMATIC_API ERouteType
{
	RouteType_RouteServer = 0,
    RouteType_LocalController = 1,
    RouteType_MobileDevice = 2, 
    RouteType_UnrealEngine = 3,
    RouteType_WebService = 4,
    RouteType_Max = RouteType_WebService        
};

UENUM()
enum class REAUTOMATIC_API ERouteProtoID
{
	RouteProtoID_Regist = 0,
	RouteProtoID_NormalMessage = 1,
	RouteProtoID_Close = 2,        
	RouteProtoID_HeartBeat = 3,   
	RouteProtoID_Log = 4,		  
	RouteProtoID_ListDevice = 5,	  
	RouteProtoID_Max = RouteProtoID_ListDevice         
};

USTRUCT()
struct FRouteRegistMessage
{
	GENERATED_BODY()

	UPROPERTY()
	FString RegistName;

	UPROPERTY()
	ERouteType RegistType;
};

USTRUCT()
struct FRouteCloseMessage
{
	GENERATED_BODY()
	
	UPROPERTY()
	FString RegistName;

	UPROPERTY()
	ERouteType RegistType;
};

USTRUCT()
struct FRouteSendMessage
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
	
};

USTRUCT()
struct FCmdTypes
{
	GENERATED_BODY()
	
	using FCmdType=FString;

	UPROPERTY()
	static FCmdType Log = "Log";

	UPROPERTY()
	static FCmdType UE4Msg = "UE4Msg";

	UPROPERTY()
	static FCmdType ListDevice = "ListDevice";

	UPROPERTY()
	static FCmdType Shell = "Shell";

	UPROPERTY()
	static FCmdType DownloadFile = "DownloadFile";
	
};

USTRUCT()
struct FRawCommandMessage
{
	GENERATED_BODY()
	
	UPROPERTY()
	FCmdTypes::FCmdType CmdId;
	
};
