/*-- schnelle Projektile --*/

#include Library_ConfigurableRule

public func RuleForbid(){ return [ IGIB ];}

global func MOD_FastBullets()
{
	return ObjectCount(Rule_FastBullets);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// properties

local Name = "$Name$";
local Description = "$Description$";
