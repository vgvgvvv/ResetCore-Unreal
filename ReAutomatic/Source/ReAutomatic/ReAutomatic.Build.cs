// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

using System.IO;
using UnrealBuildTool;

public class ReAutomatic : ModuleRules
{
	public ReAutomatic(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicIncludePaths.AddRange(
			new string[] {
#if UE_4_20_OR_LATER
				Path.Combine(ModuleDirectory, "Public"),
#else
				"ReAutomatic/Public",
#endif
			}
			);


		PrivateIncludePaths.AddRange(
			new string[] {
#if UE_4_20_OR_LATER
				Path.Combine(ModuleDirectory, "Private"),
#else
				"ReAutomatic/Private",
#endif
			}
			);


		PublicDependencyModuleNames.AddRange(
			new string[]
			{
				"Core",
                "CommonLib",
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
				"Json",
				"JsonUtilities",
				"Networking",
				"Sockets",
				"UMG",
				"Http",
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
