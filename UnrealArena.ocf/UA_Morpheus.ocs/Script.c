
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

	// lift plates
	var lp1 = CreateObject(Liftplate_Small, 264, 695 -5, NO_OWNER);
	var lp2 = CreateObject(Liftplate_Small, 199, 775 -5, NO_OWNER);
	lp1->SetMoveDirection(COMD_Down);
	lp2->SetMoveDirection(COMD_Down);
	lp1->SetVertex(1, 1, -68);
	lp2->SetVertex(1, 1, -68);
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