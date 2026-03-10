// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class ProjectOverwatch : ModuleRules
{
	public ProjectOverwatch(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] {
			"Core",
			"CoreUObject",
			"Engine",
			"InputCore",
			"EnhancedInput",
			"AIModule",
			"StateTreeModule",
			"GameplayStateTreeModule",
			"UMG",
			"Slate",
			"Paper2D"
		});

		PrivateDependencyModuleNames.AddRange(new string[] 
		{
		});

		PublicIncludePaths.AddRange(new string[] {
			"ProjectOverwatch",
			"ProjectOverwatch/Variant_Horror",
			"ProjectOverwatch/Variant_Horror/UI",
			"ProjectOverwatch/Variant_Shooter",
			"ProjectOverwatch/Variant_Shooter/AI",
			"ProjectOverwatch/Variant_Shooter/UI",
			"ProjectOverwatch/Variant_Shooter/Weapons"
		});

		// Uncomment if you are using Slate UI
		// PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });

		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}
