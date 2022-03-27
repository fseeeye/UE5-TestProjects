// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class EditorSpawnAndSim : ModuleRules
{
	public EditorSpawnAndSim(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "HeadMountedDisplay" });

		if (Target.bBuildEditor)
        {
			PrivateDependencyModuleNames.AddRange(new string[] { "UnrealEd", "LevelEditor" });
		}
	}
}
