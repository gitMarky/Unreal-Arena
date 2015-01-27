/*-- No Blood --*/

#include Library_ConfigurableRule

global func MOD_FixedEquipment(){ return ObjectCount( Rule_FixedEquipment ); }

public func RuleRequired() { return [ Rule_ClassSelection ]; }

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// properties

local Name = "$Name$";
local Description = "$Description$";
