#pragma once
#include "JsonObject.h"
#include "NetPackage.h"

enum class ERouteType : uint8;
struct FRawCommandMessage;
enum class ERouteProtoID : uint8;
class FNetJsonSerializer;

class FRouteMessagePackage
{
public:
	static FNetPackage MakeFromJsonObject(TSharedPtr<FJsonObject> JsonObject);
	static FNetPackage MakeFromProtoIDMessage(ERouteProtoID ProtoID, TSharedPtr<FJsonObject> Content);
	static FNetPackage MakeFromRouteMessage(const FRawCommandMessage& CommandMessage, const FString& TargetName, const ERouteType TargetType);
};
