// Shooter_Game, All rights reserved.

using UnrealBuildTool;

public class Shooter_Game : ModuleRules
{
    public Shooter_Game(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "Niagara", "PhysicsCore", "GameplayTasks", "NavigationSystem",
                                                            "GameplayCameras", "Slate", "SlateCore" });
    }
}
