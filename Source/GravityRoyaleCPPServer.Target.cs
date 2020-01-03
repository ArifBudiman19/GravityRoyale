// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;
using System.Collections.Generic;

public class GravityRoyaleCPPServerTarget : TargetRules
{
	public GravityRoyaleCPPServerTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Server;
		ExtraModuleNames.Add("GravityRoyaleCPP");
	}
}
