// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class Luau : ModuleRules
{
	public Luau(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;
		CppStandard = CppStandardVersion.Cpp17;
		ShadowVariableWarningLevel = WarningLevel.Off;
		UnsafeTypeCastWarningLevel = WarningLevel.Off;
		
		PublicIncludePaths.AddRange(
			new string[] {
				
				// ... add public include paths required here ...
			}
			);
				
		
		PrivateIncludePaths.AddRange(
			new string[] {
				"Luau/Luau/Analysis/include",
				"Luau/Luau/Ast/include",
				"Luau/Luau/CodeGen/include",
				"Luau/Luau/Common/include",
				"Luau/Luau/Compiler/include",
				"Luau/Luau/VM/include"
				// ... add other private include paths required here ...
			}
			);
			
		
		PublicDependencyModuleNames.AddRange(
			new string[]
			{
				"Core"
			}
			);
			
		
		PrivateDependencyModuleNames.AddRange(
			new string[]
			{
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
