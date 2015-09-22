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

/**
 Callback from configuration menu object.
 */
protected func OnConfigurationEnd()
{
	// the players have been released from their containers by
	// the round countdown. Put them in relaunch containers
	// at the starting position
	for (var i = 0; i < GetPlayerCount(); i++)
	{
		SpawnPlayer(GetPlayerByIndex(i));
	}

	// create a round countdown
	if (!FindObject(Find_ID(Countdown))) CreateObject(Countdown, 0, 0, NO_OWNER);
}

/**
 Callback from the round manager.
 */
protected func OnRoundStart(int round)
{
    // release all players from their relaunch containers, instantly.
	ReleasePlayers(true);
}


/**
 Callback from the thing that relaunches the players
 */
protected func RelaunchPlayer(int player, int killer)
{
	var crew = SpawnPlayer(player);
	ReleaseCrew(crew);
}

/**
 Puts a player in a relaunch container, without releasing him.
 */
protected func SpawnPlayer(int player)
{
	var crew = SpawnPlayerCrew(player);
	var relaunch_location = GetRelaunchLocation(player);
    ContainPlayer(relaunch_location, crew);
    CreateStartingEquipment(crew);
    return crew;
}


private func SpawnPlayerCrew(int player)
{
    var crew = GetHiRank(player);

	if (crew == nil)
	{
		crew = CreateObject(Clonk, 0, 0, player);
		crew->MakeCrewMember(player);
	}

	SetCursor(player, crew);
	return crew;
}


private func CreateStartingEquipment(object crew)
{
    RemoveEquipment(crew);
	StartingEquipment(crew);
}


private func RemoveEquipment(object crew)
{
	// remove previous contents
	for (var contents = crew->Contents(); contents != nil; contents = crew->Contents())
	{
		contents->RemoveObject();
	}
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
    	var id_primary_weapon = GameConfiguration()->GetSpawnPointItem("weapon_slot_01");
		var id_secondary_weapon = GameConfiguration()->GetSpawnPointItem("weapon_slot_02");

		if (nil != id_primary_weapon) crew->CreateContents(id_primary_weapon);
		if (nil != id_secondary_weapon) crew->CreateContents(id_secondary_weapon);
	}
}

private func GetRelaunchLocation(int player)
{
	// sort the possible locations
	var possible_locations = [];

	for (var location in RelaunchLocations())
	{
		if (location.team == -1 || location.team == GetPlayerTeam(player))
		{
			PushBack(possible_locations, location);
		}
	}

	// determine a random location
	return possible_locations[Random(GetLength(possible_locations))];
}


private func ContainPlayer(proplist relaunch_location, object crew)
{
    // from existing one?
	var relaunch_container = crew->Contained();
	
	// delete foreign invalid containers...
	if (relaunch_container != nil && relaunch_container->GetID() != RelaunchContainerEx)
	{
		relaunch_container->RemoveObject(true);
	}

	if (relaunch_container == nil)
	{
		relaunch_container = CreateObject(RelaunchContainerEx,
							              relaunch_location.x,
							              relaunch_location.y,
							              crew->GetOwner());
     }
     else
     {
     	relaunch_container->SetPosition(relaunch_location.x, relaunch_location.y);
     }

	 relaunch_container->PrepareRelaunch(crew);
}


/**
 Releases all players from their relaunch containers.
 @par instant If {@c true}, then the relaunch container exits the player immediately.
 @version 0.1.0
 */
public func ReleasePlayers(bool instant)
{
	for (var i = 0; i < GetPlayerCount(); i++)
	{
		ReleasePlayer(GetPlayerByIndex(i), instant);
	}
}


/**
 Releases a single player from his relaunch container.
 @par instant If {@c true}, then the relaunch container exits the player immediately.
 @version 0.1.0
 */
public func ReleasePlayer(int player, bool instant)
{
	for (var i = 0; i < GetCrewCount(player); i++)
	{
		ReleaseCrew(GetCrew(player, i), instant);
	}
}


/**
 Releases the crew member from their relaunch container.
 @par instant If {@c true}, then the relaunch container exits the player immediately.
 @version 0.1.0
 */
public func ReleaseCrew(object crew, bool instant)
{
	var container = crew->Contained();
	
	if ((container != nil) && (container->GetID() == RelaunchContainerEx))
	{
		if (instant)
		{
			container->InstantRelaunch();
		}
		else
		{
			container->StartRelaunch(crew);
		}
	}
}


protected func RelaunchLocations(){ return [{ x = LandscapeWidth() / 2, y = 20, team = -1}];}
