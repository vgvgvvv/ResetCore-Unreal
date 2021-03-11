#include "API/AutomaticPlatformHelper.h"

#include <string>

FString UAutomaticPlatformHelper::GetDeviceName()
{
	#if PLATFORM_ANDROID

	return FAndroidMisc::GetDeviceModel();
	
	#else

	char *temp = 0;
	std::string computerName;
    
	#if defined(WIN32) || defined(_WIN32) || defined(_WIN64)
	temp = getenv("COMPUTERNAME");
	if (temp != 0) {
		computerName = temp;
		temp = 0;
	}
	#else
	temp = getenv("HOSTNAME");
	if (temp != 0) {
		computerName = temp;
		temp = 0;
	} else {
		temp = new char[512];
		if (gethostname(temp, 512) == 0) { // success = 0, failure = -1
			computerName = temp;
		}
		delete []temp;
		temp = 0;
	}
	#endif
	return FString(computerName.c_str());

	#endif

	
}
