// (C) Gaslight Games Ltd, 2019-2020.  All rights reserved.

using UnrealBuildTool;
using System.Collections.Generic;

public class EOSBasicEditorTarget : TargetRules
{
	public EOSBasicEditorTarget(TargetInfo Target) : base(Target)
	{
        DefaultBuildSettings = BuildSettingsVersion.V2;

        Type = TargetType.Editor;

		ExtraModuleNames.AddRange( new string[] { "EOSBasic" } );
	}
}
