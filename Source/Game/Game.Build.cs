// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class Game : ModuleRules
{
	public Game(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "HeadMountedDisplay", "UMG", "InputCore", "NavigationSystem" ,"AIModule", "GamePlayTasks"});
		PrivateDependencyModuleNames.AddRange(new string[] { "GameLoadingScreen", "Slate", "SlateCore", "MoviePlayer" });
	}
}
