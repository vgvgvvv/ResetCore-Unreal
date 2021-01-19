#pragma once

struct FNetPackage;
class FSocketClient;

class INetPackageHandler
{
public:
	virtual ~INetPackageHandler() = default;
	virtual void HandlePackage(FSocketClient& from, FNetPackage& package) = 0;
};
