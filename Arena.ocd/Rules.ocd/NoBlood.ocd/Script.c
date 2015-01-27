/*-- No Blood --*/
#include Library_ConfigurableRule


global func MOD_NoBlood(){ return ObjectCount( Rule_NoBlood ); }

public func RuleForbid(){ return [ Rule_MoreGore ];}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// properties

local Name = "$Name$";
local Description = "$Description$";
