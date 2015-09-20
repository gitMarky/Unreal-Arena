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
	var crew = GetHiRank(player);
	
	if (crew == nil)
	{
		crew = CreateObject(Clonk, 0, 0, player);
		crew->MakeCrewMember(player);
	}
	SetCursor(player, crew);
	
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
	var relaunch_location = possible_locations[Random(GetLength(possible_locations))];

    // spawn player from relaunch container
    
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
	 return crew;
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
