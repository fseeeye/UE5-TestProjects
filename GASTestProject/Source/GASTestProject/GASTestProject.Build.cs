// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class GASTestProject : ModuleRules
{
	public GASTestProject(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "HeadMountedDisplay",
			"GameplayAbilities", // Register GAS
			"GameplayTags",
			"GameplayTasks" // for Gameplay Task
		});
	}
}
