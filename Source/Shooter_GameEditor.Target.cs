// Shooter_Game, All rights reserved.

using UnrealBuildTool;
using System.Collections.Generic;

public class Shooter_GameEditorTarget : TargetRules
{
	public Shooter_GameEditorTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Editor;
		DefaultBuildSettings = BuildSettingsVersion.V2;

		ExtraModuleNames.AddRange( new string[] { "Shooter_Game" } );
	}
}
