/**
 Default script.
 @author Marky
 @version 0.1
*/

#include Library_ScenarioScript_PlayerRespawn

local Name = "$Name$";
local Description = "$Description$";

func Initialize()
{
	_inherited(...);

	CreateObject(Environment_RoundManager);
	CreateObject(Environment_ConfigurationUA);
}


public func GetPlayerCrewID(int player)
{
	return UA_Clonk;
}


private func StartingEquipment(object crew)
{
	if (IsInstaGibConfigured())
	{
		crew->CreateContents(Weapon_UT99_EnergyRifle);
	}
	else
	{
	    // determine the 
    	var id_primary_weapon = GameConfiguration()->GetSpawnPointItem(SPAWN_Weapon_Chainsaw);
		var id_secondary_weapon = GameConfiguration()->GetSpawnPointItem(SPAWN_Weapon_Enforcer);

		if (nil != id_primary_weapon) crew->CreateContents(id_primary_weapon);
		if (nil != id_secondary_weapon) crew->CreateContents(id_secondary_weapon);
	}
}


protected func RelaunchLocations(){ return [{ x = LandscapeWidth() / 2, y = 20, team = -1}];}
