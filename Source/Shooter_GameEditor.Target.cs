// Shooter_Game, All rights reserved.

using UnrealBuildTool;
using System.Collections.Generic;
using EpicGames.Core;

public class Shooter_GameEditorTarget : TargetRules
{
    [CommandLine("-UnoptimizedCode")]
    public bool UnoptimizedCode = false;

    public Shooter_GameEditorTarget(TargetInfo Target) : base(Target)
    {
        Type = TargetType.Editor;
        DefaultBuildSettings = BuildSettingsVersion.V2;

        ExtraModuleNames.AddRange(new string[] { "Shooter_Game" });

        if (UnoptimizedCode)
        {
            ProjectDefinitions.Add("UNOPTIMIZED_CODE");
        }
    }
}
