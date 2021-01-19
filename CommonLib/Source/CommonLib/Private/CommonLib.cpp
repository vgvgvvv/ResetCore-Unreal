// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#include "CommonLib.h"

#define LOCTEXT_NAMESPACE "FCommonLibModule"

DEFINE_LOG_CATEGORY(ResetCore_CommonLib);

void FCommonLibModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
}

void FCommonLibModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FCommonLibModule, CommonLib)