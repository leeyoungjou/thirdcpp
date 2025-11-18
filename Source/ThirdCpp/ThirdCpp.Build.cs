// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class ThirdCpp : ModuleRules
{
	public ThirdCpp(ReadOnlyTargetRules Target) : base(Target)
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
			"Slate"
		});

		PrivateDependencyModuleNames.AddRange(new string[] { });

		PublicIncludePaths.AddRange(new string[] {
			"ThirdCpp",
			"ThirdCpp/Variant_Platforming",
			"ThirdCpp/Variant_Platforming/Animation",
			"ThirdCpp/Variant_Combat",
			"ThirdCpp/Variant_Combat/AI",
			"ThirdCpp/Variant_Combat/Animation",
			"ThirdCpp/Variant_Combat/Gameplay",
			"ThirdCpp/Variant_Combat/Interfaces",
			"ThirdCpp/Variant_Combat/UI",
			"ThirdCpp/Variant_SideScrolling",
			"ThirdCpp/Variant_SideScrolling/AI",
			"ThirdCpp/Variant_SideScrolling/Gameplay",
			"ThirdCpp/Variant_SideScrolling/Interfaces",
			"ThirdCpp/Variant_SideScrolling/UI"
		});

		// Uncomment if you are using Slate UI
		// PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });

		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}
