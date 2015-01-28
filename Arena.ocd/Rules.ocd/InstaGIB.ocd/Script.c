/*-- InstaGib --*/

#include Library_ConfigurableRule

public func GameConfigConflictingRules()
{
	return [Rule_ClassSelection, 
	        Rule_SelectSpecial,
	        Rule_NoAmmo,
	        Rule_WeaponsStay,
	        Rule_FastBullets];
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// properties

local Name = "$Name$";
local Description = "$Description$";
