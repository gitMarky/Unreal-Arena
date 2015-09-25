/*-- InstaGib --*/

#include Library_ConfigurableRule


static const PROJECTILE_Color_InstaGib = -42386; // RGB(255,90,110);
static const PROJECTILE_Damage_InstaGib = 10000;

public func GameConfigConflictingRules()
{
	return [];
}

global func IsInstaGibConfigured()
{
	return FindObject(Find_ID(Rule_InstaGib));
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// properties

local Name = "$Name$";
local Description = "$Description$";
