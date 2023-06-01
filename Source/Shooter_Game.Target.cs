// Shooter_Game, All rights reserved.

using UnrealBuildTool;
using System.Collections.Generic;

public class Shooter_GameTarget : TargetRules
{
    public Shooter_GameTarget(TargetInfo Target) : base(Target)
    {
        Type = TargetType.Game;
        DefaultBuildSettings = BuildSettingsVersion.V2;

        ExtraModuleNames.AddRange(new string[] { "Shooter_Game" });
    }
}
