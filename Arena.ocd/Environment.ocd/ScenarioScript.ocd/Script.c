/**
 Default script.
 @author Marky
 @version 0.1
*/

local Name = "$Name$";
local Description = "$Description$";

func Initialize()
{
	CreateObject(Environment_RoundManager);
	CreateObject(Environment_ConfigurationUA);
	CreateObject(RoundTester);
}