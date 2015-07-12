
static const SPAWN_Ammo_BaseFront = "b";
static const SPAWN_Ammo_BaseBack = "d";

static const SPAWN_Ammo_BaseFloor1 = "f";

static const SPAWN_Ammo_BaseFloor2 = "h";

static const SPAWN_Ammo_BaseRoof = "j";
static const SPAWN_Extra_BaseRoof = "k";

static const SPAWN_Ammo_BaseTunnel = "m";

static const SPAWN_Superweapon = "n";


protected func Initialize()
{
	CreateSpawnPoints();
	CreateObject(Environment_ScenarioScript);
}

// Gamecall from LastManStanding goal, on respawning.
protected func OnPlayerRelaunch(int plr)
{
	var clonk = GetCrew(plr);
	var relaunch = CreateObject(RelaunchContainer, LandscapeWidth() / 2, LandscapeHeight() / 2, clonk->GetOwner());
	relaunch->StartRelaunch(clonk);
	return;
}

protected func CreateSpawnPoints()
{	
}