/*-- InstaGib --*/

#include Library_ConfigurableRule


static const PROJECTILE_Color_InstaGib = -42386; // RGB(255,90,110);
static const PROJECTILE_Damage_InstaGib = 10000;

public func GameConfigConflictingRules()
{
	return [Rule_ClassSelection, 
	        Rule_SelectSpecial,
	        Rule_NoAmmo,
	        Rule_WeaponsStay,
	        Rule_FastBullets];
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
