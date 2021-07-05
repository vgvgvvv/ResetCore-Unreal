#pragma once
#include <string>

class DOTNETHOSTER_API FLog
{
public:
	static void Log(const std::string& Info);
	static void Warn(const std::string& Info);
	static void Error(const std::string& Info);
};
