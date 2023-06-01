// Fill out your copyright notice in the Description page of Project Settings.

using UnrealBuildTool;

public class CombatGASCompanion : ModuleRules
{
	public CombatGASCompanion(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
	
		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore",
			"GASCompanion",
			"UMG", 
			"GameplayAbilities", 
			"GameplayTasks", 
			"GameplayTags",
			"EnhancedInput",
			/*AnimGraphRunTime Required to use UKismetAnimationLibraryFunctions */
			"AnimGraphRuntime"});

		PrivateDependencyModuleNames.AddRange(new string[] { "GASCompanion"});

		// Uncomment if you are using Slate UI
		// PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });
		
		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}
