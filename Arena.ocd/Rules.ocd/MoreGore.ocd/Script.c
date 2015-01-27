/*-- More Gore --*/
#include Library_ConfigurableRule

global func MOD_MoreGore(){ return ObjectCount( Rule_MoreGore ); }

public func RuleForbid(){ return [ ID_Rule_NoBlood ]; }

/* Zeugs, das herumliegt, soll verschwinden */

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// properties

local Name = "$Name$";
local Description = "$Description$";
