#include Environment_ScenarioScript

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
	
	_inherited(...);
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
	var timer_weapons = 10 * 36;
	var timer_items = 10 * 36;

	
	// spawn point templates
	var weapon_asmd = CreateSpawnPoint(0, 0)->SetRespawnTimer(timer_weapons)->SpawnItem(SPAWN_Weapon_ASMD)->SetDescription("$WeaponBaseFront$");
	var ammo_asmd = CreateSpawnPoint(0, 0)->SetRespawnTimer(timer_items)->SpawnItem(SPAWN_Ammo_ASMD)->SetDescription("$AmmoBaseFront$");

	var weapon_linkgun = CreateSpawnPoint(0, 0)->SetRespawnTimer(timer_weapons)->SpawnItem(SPAWN_Weapon_PulseRifle)->SetDescription("$WeaponBaseBack$");
	var ammo_linkgun = CreateSpawnPoint(0, 0)->SetRespawnTimer(timer_items)->SpawnItem(SPAWN_Ammo_PulseRifle)->SetDescription("$AmmoBaseBack$");

	var weapon_rocket = CreateSpawnPoint(0, 0)->SetRespawnTimer(timer_weapons)->SpawnItem(SPAWN_Weapon_RocketLauncher)->SetDescription("$WeaponBaseFloor1$");
	var ammo_rocket = CreateSpawnPoint(0, 0)->SetRespawnTimer(timer_items)->SpawnItem(SPAWN_Ammo_RocketLauncher)->SetDescription("$AmmoBaseFloor1$");

	var weapon_sniper = CreateSpawnPoint(0, 0)->SetRespawnTimer(timer_weapons)->SpawnItem(SPAWN_Weapon_SniperRifle)->SetDescription("$WeaponBaseFloor2$");
	var ammo_sniper = CreateSpawnPoint(0, 0)->SetRespawnTimer(timer_items)->SpawnItem(SPAWN_Ammo_SniperRifle)->SetDescription("$AmmoBaseFloor2$");

	var weapon_sniper2 = CreateSpawnPoint(0, 0)->SetRespawnTimer(timer_weapons)->SpawnItem(SPAWN_Weapon_SniperRifle)->SetDescription("$WeaponBaseRoof$");
	var ammo_sniper2 = CreateSpawnPoint(0, 0)->SetRespawnTimer(timer_items)->SpawnItem(SPAWN_Ammo_SniperRifle)->SetDescription("$AmmoBaseRoof$");

	var weapon_ripper = CreateSpawnPoint(0, 0)->SetRespawnTimer(timer_weapons)->SpawnItem(SPAWN_Weapon_Ripper)->SetDescription("$WeaponBaseTunnel$");
	var ammo_ripper = CreateSpawnPoint(0, 0)->SetRespawnTimer(timer_items)->SpawnItem(SPAWN_Ammo_Ripper)->SetDescription("$AmmoBaseTunnel$");

	var weapon_redeemer = CreateSpawnPoint(0, 0)->SetRespawnTimer(timer_weapons)->SpawnItem(SPAWN_Superweapon)->SetDescription("$WeaponSuper$");
	var armor = CreateSpawnPoint(0, 0)->SetRespawnTimer(timer_items)->SpawnItem(SPAWN_Item_Armor)->SetDescription("$ExtraBaseRoof$");

	var vial = CreateSpawnPoint(0, 0)->SetRespawnTimer(timer_items)->SpawnItem(SPAWN_Item_HealthVial)->SetDescription("$HealthSmall$");
	var health_base = CreateSpawnPoint(0, 0)->SetRespawnTimer(timer_items)->SpawnItem(SPAWN_Item_HealthPack)->SetDescription("$HealthBase$");
	var health_center = CreateSpawnPoint(0, 0)->SetRespawnTimer(timer_items)->SpawnItem(SPAWN_Item_HealthPack)->SetDescription("$HealthCenter$");
	var keg_o_health = CreateSpawnPoint(0, 0)->SetRespawnTimer(timer_items)->SpawnItem(SPAWN_Item_HealthKeg)->SetDescription("$HealthSuper$");


	// ASMD
	CopySpawnPoint(weapon_asmd, 411, 581);
	CopySpawnPoint(ammo_asmd, 411-20, 581);
	CopySpawnPoint(ammo_asmd, 411+20, 581);
	
	CopySpawnPoint(weapon_asmd, 411, 581, true);
	CopySpawnPoint(ammo_asmd, 411-20, 581, true);
	CopySpawnPoint(ammo_asmd, 411+20, 581, true);

	// Linkgun
	CopySpawnPoint(weapon_linkgun, 284, 581);
	CopySpawnPoint(ammo_linkgun, 284+20, 581);
	CopySpawnPoint(ammo_linkgun, 284-20, 581);

	CopySpawnPoint(weapon_linkgun, 284, 581, true);
	CopySpawnPoint(ammo_linkgun, 284+20, 581, true);
	CopySpawnPoint(ammo_linkgun, 284-20, 581, true);
	
	// Rocket Launcher
	CopySpawnPoint(weapon_rocket, 272, 490);
	CopySpawnPoint(ammo_rocket, 272-20, 490);
	CopySpawnPoint(ammo_rocket, 272+20, 490);

	CopySpawnPoint(weapon_rocket, 272, 490, true);
	CopySpawnPoint(ammo_rocket, 272-20, 490, true);
	CopySpawnPoint(ammo_rocket, 272+20, 490, true);
	
	// Sniper
	CopySpawnPoint(weapon_sniper, 400, 369);
	CopySpawnPoint(ammo_sniper, 400-20, 369);
	CopySpawnPoint(ammo_sniper, 400+20, 369);

	CopySpawnPoint(weapon_sniper, 400, 369, true);
	CopySpawnPoint(ammo_sniper, 400-20, 369, true);
	CopySpawnPoint(ammo_sniper, 400+20, 369, true);

	// base - roof
	CopySpawnPoint(weapon_sniper2, 400, 195);
	CopySpawnPoint(ammo_sniper2, 400-20, 195);
	CopySpawnPoint(ammo_sniper2, 400-30, 195);

	CopySpawnPoint(weapon_sniper2, 400, 195, true);
	CopySpawnPoint(ammo_sniper2, 400-20, 195, true);
	CopySpawnPoint(ammo_sniper2, 400-30, 195, true);

	// Ripper
	CopySpawnPoint(weapon_ripper, 550, 665);
	CopySpawnPoint(ammo_ripper, 550-20, 665);
	CopySpawnPoint(ammo_ripper, 550+20, 665);

	CopySpawnPoint(weapon_ripper, 550, 665, true);
	CopySpawnPoint(ammo_ripper, 550-20, 665, true);
	CopySpawnPoint(ammo_ripper, 550+20, 665, true);
	
	// Redeemer
	CopySpawnPoint(weapon_redeemer, 635, 415);
	CopySpawnPoint(weapon_redeemer, 635, 415, true);

	// Armor
	CopySpawnPoint(armor, 290, 195);
	CopySpawnPoint(armor, 290, 195, true);

	// Health Vial - small health
	CopySpawnPoint(vial, 285, 650);
	CopySpawnPoint(vial, 315, 650);
	CopySpawnPoint(vial, 345, 650);
	CopySpawnPoint(vial, 375, 650);
	CopySpawnPoint(vial, 405, 650);

 	CopySpawnPoint(vial, 285, 650, true);
 	CopySpawnPoint(vial, 315, 650, true);
 	CopySpawnPoint(vial, 345, 650, true);
 	CopySpawnPoint(vial, 375, 650, true);
 	CopySpawnPoint(vial, 405, 650, true);

	// Medipack - medium health
	CopySpawnPoint(health_base, 347-25, 581);
	CopySpawnPoint(health_base, 347+25, 581);

	CopySpawnPoint(health_base, 347-25, 581, true);  
	CopySpawnPoint(health_base, 347+25, 581, true);  

	CopySpawnPoint(health_center, 932, 660);
	CopySpawnPoint(health_center, 972, 660);

	// Keg'o'Health - large health
	CopySpawnPoint(keg_o_health, 952, 460);
	
	//-----------------------------------------
	// remove the templates again
	
	weapon_asmd->RemoveObject();
//	ammo_asmd->RemoveObject();

	weapon_linkgun->RemoveObject();
//	ammo_linkgun->RemoveObject();

	weapon_rocket->RemoveObject();
//	ammo_rocket->RemoveObject();

	weapon_sniper->RemoveObject();
//	ammo_sniper->RemoveObject();

	weapon_sniper2->RemoveObject();
//	ammo_sniper2->RemoveObject();

	weapon_ripper->RemoveObject();
//	ammo_ripper->RemoveObject();

	weapon_redeemer->RemoveObject();
	armor->RemoveObject();

	vial->RemoveObject();
	health_base->RemoveObject();
	health_center->RemoveObject();
	keg_o_health->RemoveObject();	
}
