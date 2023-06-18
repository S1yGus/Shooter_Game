// Shooter_Game, All rights reserved.

using UnrealBuildTool;
using System.Collections.Generic;
using EpicGames.Core;

public class Shooter_GameTarget : TargetRules
{
    [CommandLine("-UnoptimizedCode")]
    public bool UnoptimizedCode = false;

    public Shooter_GameTarget(TargetInfo Target) : base(Target)
    {
        Type = TargetType.Game;
        DefaultBuildSettings = BuildSettingsVersion.V2;

        ExtraModuleNames.AddRange(new string[] { "Shooter_Game" });

        if (UnoptimizedCode)
        {
            ProjectDefinitions.Add("UNOPTIMIZED_CODE");
        }
    }
}
