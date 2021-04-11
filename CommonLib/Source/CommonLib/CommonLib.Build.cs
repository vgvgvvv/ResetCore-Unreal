// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class CommonLib : ModuleRules
{
	public CommonLib(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;
		
		PublicIncludePaths.AddRange(
			new string[] {
#if UE_4_20_OR_LATER
				Path.Combine(ModuleDirectory, "Public"),
#else
				"CommonLib/Public",
#endif
			}
			);
				
		
		PrivateIncludePaths.AddRange(
			new string[] {
#if UE_4_20_OR_LATER
				Path.Combine(ModuleDirectory, "Private")
#else
				"CommonLib/Private",
#endif
			}
			);
			
		
		PublicDependencyModuleNames.AddRange(
			new string[]
			{
				"Core",
				"Http",
				// ... add other public dependencies that you statically link with here ...
			}
			);
			
		
		PrivateDependencyModuleNames.AddRange(
			new string[]
			{
				"CoreUObject",
				"Engine",
				"Slate",
				"SlateCore",
				"Networking",
				"Sockets",
				"Json",
				// ... add private dependencies that you statically link with here ...	
			}
			);
		
		
		DynamicallyLoadedModuleNames.AddRange(
			new string[]
			{
				// ... add any modules that your module loads dynamically here ...
			}
			);
	}
}
