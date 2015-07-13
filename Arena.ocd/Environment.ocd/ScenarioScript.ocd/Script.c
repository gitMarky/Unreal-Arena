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


protected func RelaunchPlayer(int player, int killer)
{
	var crew = CreateObject(Clonk, LandscapeWidth() / 2, 20, player);
	crew->MakeCrewMember(player);
	SetCursor(player, crew);

	var relaunch_x = crew->GetX();
	var relaunch_y = crew->GetY();

	CreateObject(RelaunchContainerEx,
	             relaunch_x,
	             relaunch_y,
	             crew->GetOwner())->StartRelaunch(crew);
}
