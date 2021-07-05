// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

using System.IO;
using UnrealBuildTool;

public class DotNetHoster : ModuleRules
{
	public DotNetHoster(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;
		
		PublicIncludePaths.AddRange(
			new string[] {
				"DotNetHoster/Public"
				// ... add public include paths required here ...
			}
			);
				
		
		PrivateIncludePaths.AddRange(
			new string[] {
				"DotNetHoster/Private",
				// ... add other private include paths required here ...
			}
			);
			
		
		PublicDependencyModuleNames.AddRange(
			new string[]
			{
				"Core",
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
				// ... add private dependencies that you statically link with here ...	
			}
			);
		
		
		DynamicallyLoadedModuleNames.AddRange(
			new string[]
			{
				// ... add any modules that your module loads dynamically here ...
			}
			);

		string LibDir = ModuleDirectory + "/Thirdpart/DotNet/";

		if (Target.Platform == UnrealTargetPlatform.Win32 || Target.Platform == UnrealTargetPlatform.Win64)
		{
			LibDir += "Windows/";
			LibDir += "packs/Microsoft.NetCore.App.Host.win-x64/3.1.9/runtimes/win-x64/native/";
		
			PublicLibraryPaths.AddRange(new string[]
			{
				LibDir				
			});
			PublicAdditionalLibraries.Add(LibDir + "nethost.lib");
		}else if (Target.Platform == UnrealTargetPlatform.Mac)
		{
			LibDir += "Mac/";
		}else if (Target.Platform == UnrealTargetPlatform.Linux)
		{
			LibDir += "Linux/";
		}

		
	}
}
