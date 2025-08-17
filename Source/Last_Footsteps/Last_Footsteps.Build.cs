// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class Last_Footsteps : ModuleRules
{
	public Last_Footsteps(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
	
		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "NavigationSystem", "AIModule", "EnhancedInput", "UMG", "Niagara", "CableComponent", "AdvancedWidgets" });

		PrivateDependencyModuleNames.AddRange(new string[] {  });

        //Uncomment if you are using Slate UI
        PrivateIncludePaths.AddRange(new string[] {
			//"Slate", "SlateCore"
			"Last_Footsteps",
            "Last_Footsteps/CHR",
            "Last_Footsteps/OBS",
            "Last_Footsteps/ENM",
            "Last_Footsteps/ITEM",
            "Last_Footsteps/UI"
        });

        //PrivateDependencyModuleNames.AddRange(new string[] {  });

        // Uncomment if you are using Slate UI
        PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore", "UMG" });


        // Uncomment if you are using online features
        // PrivateDependencyModuleNames.Add("OnlineSubsystem");

        // To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
    }
}
