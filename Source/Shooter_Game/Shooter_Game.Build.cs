// Shooter_Game, All rights reserved.

using UnrealBuildTool;

public class Shooter_Game : ModuleRules
{
    public Shooter_Game(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(new string[] {
            "Core",
            "CoreUObject",
            "Engine",
            "InputCore",
            "Niagara",
            "PhysicsCore",
            "GameplayTasks",
            "NavigationSystem",
            "GameplayCameras",
            "SlateCore"});

        PrivateDependencyModuleNames.AddRange(new string[] { });

        PublicIncludePaths.AddRange(new string[] {
            "Shoother_Game/Public/Player",
            "Shoother_Game/Public/Components",
            "Shoother_Game/Public/Dev",
            "Shoother_Game/Public/Weapons",
            "Shoother_Game/Public/UI",
            "Shoother_Game/Public/Misc",
            "Shoother_Game/Public/Animations",
            "Shoother_Game/Public/Weapons/Components",
            "Shoother_Game/Public/AI",
            "Shoother_Game/Public/AI/Tasks"});

        // Uncomment if you are using Slate UI
        // PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });

        // Uncomment if you are using online features
        // PrivateDependencyModuleNames.Add("OnlineSubsystem");

        // To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
    }
}
