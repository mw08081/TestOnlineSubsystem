// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class TestOnlineSubsystem : ModuleRules
{
	public TestOnlineSubsystem(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { 
			"Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput" ,

			"UMG",

            "OnlineSubsystem", "OnlineSubsystemSteam", "OnlineSubsystemUtils",

        });

        PublicIncludePaths.AddRange(new string[]
        {
            "TestOnlineSubsystem",
        });
    }
}
