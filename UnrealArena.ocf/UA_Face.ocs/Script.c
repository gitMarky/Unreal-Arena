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
	CreateJumpPads();
	CreateWaypoints();
	
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


private func CreateJumpPads()
{
	// Jump pads
	
	var blue = RGB(100, 100, 200);
	var red = RGB(200, 100, 100);
	var grey = RGB(150, 150, 150);

	var jumpPads = [{x = 122, y = 474, strength =  88, angle =   37, base =    0, color =  blue},
			        {x = 132, y = 507, strength = 100, angle =  115, base =  130, color =  blue},
			        {x = 155, y = 575, strength = 130, angle =   10, base =    0, color =  blue},
			        {x = 125, y = 575, strength =  75, angle =   28, base =    0, color =  blue},
			        {x = 144, y = 638, strength =  70, angle =   80, base =   90, color =  blue},
			        {x = 147, y = 674, strength =  70, angle =   30, base =    0, color =  blue},
			        // mirrored
			        {x = 1783, y = 474, strength =  88, angle =  -37, base =    0, color =  red},
			        {x = 1773, y = 507, strength = 100, angle = -115, base = -130, color =  red},
			        {x = 1750, y = 575, strength = 130, angle =  -10, base =    0, color =  red},
			        {x = 1780, y = 575, strength =  75, angle =  -28, base =    0, color =  red},
			        {x = 1761, y = 638, strength =  70, angle =  -80, base =  -90, color =  red},
			        {x = 1758, y = 674, strength =  70, angle =  -30, base =    0, color =  red}];

	for (var padData in jumpPads)
	{
		CreateObject(Arena_JumpPad, padData.x, padData.y, NO_OWNER)->SetStrength(padData.strength)
		                                                             ->SetPadR(padData.angle)
		                                                             ->SetBaseR(padData.base)
		                                                             ->SetPadColor(padData.color)
		                                                             ->SetEffectColor(padData.color)
		                                                             ->SetBaseColor(grey)
		                                                             ->SetBaseGraphics("BaseSmall");
	}

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


public func CreateWaypoints()
{
	var shift = -86;

	var wp_red_ground_front = CreateObject(Map_Waypoint, 1307, 575);
	var wp_red_ground_entrance = CreateObject(Map_Waypoint, 1439, 581);
	var wp3 = CreateObject(Map_Waypoint, 1632, 485);
	var wp_red_ground_back = CreateObject(Map_Waypoint, 1651, 581);
	var wp_blue_ground_front = CreateObject(Map_Waypoint, 598, 575);
	var wp_blue_ground_entrance = CreateObject(Map_Waypoint, 466, 581);
	var wp7 = CreateObject(Map_Waypoint, 273, 485);
	var wp_blue_ground_back = CreateObject(Map_Waypoint, 254, 581);
	var wp9 = CreateObject(Map_Waypoint, 871, 529);
	var wp10 = CreateObject(Map_Waypoint, 1034, 529);
	var wp11 = CreateObject(Map_Waypoint, 971, 579);
	var wp12 = CreateObject(Map_Waypoint, 929, 579);
	var wp13 = CreateObject(Map_Waypoint, 1082, 619);
	var wp14 = CreateObject(Map_Waypoint, 953, 655);
	var wp15 = CreateObject(Map_Waypoint, 822, 619);
	var wp16 = CreateObject(Map_Waypoint, 1355, 665);
	var wp17 = CreateObject(Map_Waypoint, 552, 665);
	var wp18 = CreateObject(Map_Waypoint, 253, 649);
	var wp19 = CreateObject(Map_Waypoint, 1655, 650);

//	var wp_blue_jumppad = CreateObject(Map_Waypoint, 155,  665);
//	var wp_red_jumppad = CreateObject(Map_Waypoint, LandscapeWidth()-155,  665);
	var wp_blue_jumppad = CreateObject(Map_Waypoint, 190,  665);
	var wp_red_jumppad = CreateObject(Map_Waypoint, LandscapeWidth()-190,  665);

	var wp_blue_teleporter_sniper = CreateObject(Map_Waypoint, 398, 368);
	var wp_blue_teleporter_roof = CreateObject(Map_Waypoint, 330, 212);
	var wpt3 = CreateObject(Map_Waypoint, 290, 185);
	var wpt4 = CreateObject(Map_Waypoint, 422, 367);
	var wp_blue_teleporter_base = CreateObject(Map_Waypoint, 307, 581);
	var wp_blue_sniper_roof = CreateObject(Map_Waypoint, 406, 185);
	var wp_blue_sniper = CreateObject(Map_Waypoint, 433+5, 368);
	var wp_blue_teleporter_level = CreateObject(Map_Waypoint, 420, 485);
	var wpt16 = CreateObject(Map_Waypoint, 442+13, 485);
	var wp_red_teleporter_base = CreateObject(Map_Waypoint, 1684+shift, 581); // wp_blue_teleporter_base
	var wp_red_teleporter_sniper = CreateObject(Map_Waypoint, 1605+shift, 368); // wp_blue_teleporter_sniper
	var wp_red_teleporter_roof = CreateObject(Map_Waypoint, 1673+shift, 212); //wp_blue_teleporter_roof
	var wp_red_teleporter_level = CreateObject(Map_Waypoint, 1577+shift, 485); // wp_blue_teleporter_level
	var wpt22 = CreateObject(Map_Waypoint, 1554-13+shift, 485); // wpt16
	var wp_red_sniper = CreateObject(Map_Waypoint, 1562-5+shift, 368); // wp_blue_sniper
	var wpt24 = CreateObject(Map_Waypoint, 1570+shift, 368); //wpt24
	var wp_red_sniper_roof = CreateObject(Map_Waypoint, LandscapeWidth()-406, 185); // wp_blue_sniper_roof
	var wpt26 = CreateObject(Map_Waypoint, LandscapeWidth()-290, 185); // wpt3

	wp_red_ground_front->SetName("wp_red_ground_front");
	wp_red_ground_entrance->SetName("wp_red_ground_entrance");
	wp3->SetName("wp3");
	wp_red_ground_back->SetName("wp_red_ground_back");
	wp_blue_ground_front->SetName("wp_blue_ground_front");
	wp_blue_ground_entrance->SetName("wp_blue_ground_entrance");
	wp7->SetName("wp7");
	wp_blue_ground_back->SetName("wp_blue_ground_back");
	wp9->SetName("wp9");
	wp10->SetName("wp10");
	wp11->SetName("wp11");
	wp12->SetName("wp12");
	wp13->SetName("wp13");
	wp14->SetName("wp14");
	wp15->SetName("wp15");
	wp16->SetName("wp16");
	wp17->SetName("wp17");
	wp18->SetName("wp18");
	wp19->SetName("wp19");
	wp_blue_teleporter_sniper->SetName("wp_blue_teleporter_sniper");
	wp_blue_teleporter_roof->SetName("wp_blue_teleporter_roof");
	wp_blue_teleporter_base->SetName("wp_blue_teleporter_base");
	wp_blue_sniper_roof->SetName("wp_blue_sniper_roof");
	wp_blue_teleporter_level->SetName("wp_blue_teleporter_level");
	wp_red_teleporter_base->SetName("wp_red_teleporter_base");
	wp_red_teleporter_sniper->SetName("wp_red_teleporter_sniper");
	wp_red_teleporter_roof->SetName("wp_red_teleporter_roof");
	wp_red_teleporter_level->SetName("wp_red_teleporter_level");
	wp_red_sniper_roof->SetName("wp_red_sniper_roof");
	wpt3->SetName("wpt3");
	wpt4->SetName("wpt4");
	wp_blue_sniper->SetName("wp_blue_sniper");
	wpt16->SetName("wpt16");
	wpt22->SetName("wpt22");
	wp_red_sniper->SetName("wp_red_sniper");
	wpt24->SetName("wpt24");
	wpt26->SetName("wpt26");



	wp_red_ground_front->AddPath(wp10); //Path_MoveTo, 1
	wp_red_ground_front->AddPath(wp_red_ground_entrance); //Path_MoveTo, 1
	wp_red_ground_entrance->AddPath(wp_red_ground_front); //Path_MoveTo, 1
	wp_red_ground_back->AddPath(wp19); //Path_MoveTo, -1
	wp_blue_ground_front->AddPath(wp9); //Path_MoveTo, 1
	wp_blue_ground_front->AddPath(wp_blue_ground_entrance); //Path_MoveTo, 1
	wp_blue_ground_entrance->AddPath(wp_blue_ground_front); //Path_MoveTo, 1
	wp_blue_ground_back->AddPath(wp18); //Path_MoveTo, -1
	wp9->AddPath(wp10); //Path_Jump, 1
	wp9->AddPath(wp11); //Path_MoveTo, 1
	wp9->AddPath(wp_blue_ground_front); //Path_MoveTo, -1
	wp10->AddPath(wp9); //Path_Jump, -1
	wp10->AddPath(wp12); //Path_MoveTo, -1
	wp10->AddPath(wp_red_ground_front); //Path_MoveTo, 1
	wp11->AddPath(wp9); //Path_Jump, -1
	wp11->AddPath(wp12); //Path_MoveTo, -1
	wp11->AddPath(wp13); //Path_MoveTo, 1
	wp12->AddPath(wp10); //Path_Jump, 1
	wp12->AddPath(wp11); //Path_MoveTo, 1
	wp12->AddPath(wp15); //Path_MoveTo, -1
	wp13->AddPath(wp11); //Path_Jump, -1
	wp13->AddPath(wp14); //Path_MoveTo, -1
	wp13->AddPath(wp16); //Path_MoveTo, 1
	wp14->AddPath(wp15); //Path_MoveTo, -1
	wp14->AddPath(wp13); //Path_MoveTo, 1
	wp15->AddPath(wp12); //Path_Jump, 1
	wp15->AddPath(wp14); //Path_MoveTo, 1
	wp15->AddPath(wp17); //Path_MoveTo, -1
	wp16->AddPath(wp19); //Path_MoveTo, 1
	wp16->AddPath(wp13); //Path_MoveTo, -1
	wp17->AddPath(wp15); //Path_MoveTo, 1
	wp17->AddPath(wp18); //Path_MoveTo, -1
	wp18->AddPath(wp17); //Path_MoveTo, 1
	wp19->AddPath(wp16); //Path_MoveTo, -1





	wp_blue_teleporter_sniper->AddPath(wp_blue_teleporter_base); //Path_MoveTo, -1
	wp_blue_teleporter_sniper->AddPath(wpt4); //Path_MoveTo, 1
	wp_blue_teleporter_roof->AddPath(wp_blue_teleporter_base); //Path_MoveTo, -1
	wp_blue_teleporter_roof->AddPath(wpt3); //Path_MoveTo, 1
	wpt3->AddPath(wp_blue_teleporter_roof); //Path_MoveTo, -1
	wpt3->AddPath(wp_blue_sniper_roof); //Path_MoveTo, 1
	wpt4->AddPath(wp_blue_teleporter_sniper); //Path_MoveTo, -1
	wpt4->AddPath(wp_blue_sniper); //Path_MoveTo, 1
	wp_blue_teleporter_base->AddPath(wp_blue_teleporter_sniper); //Path_MoveTo, 1
	wp_blue_teleporter_base->AddPath(wp_blue_teleporter_roof); //Path_MoveTo, 1
	wp_blue_teleporter_base->AddPath(wp_blue_teleporter_level); //Path_MoveTo, 1




	wp_blue_teleporter_base->AddPath(wp_blue_ground_back); //Path_MoveTo, 1
	wp_blue_teleporter_base->AddPath(wp_blue_ground_entrance); //Path_MoveTo, 1
	wp_blue_ground_back->AddPath(wp_blue_teleporter_base); //Path_MoveTo, 1
	wp_blue_ground_entrance->AddPath(wp_blue_teleporter_base); //Path_MoveTo, 1
	wp_blue_sniper_roof->AddPath(wpt3); //Path_MoveTo, -1
	wp_blue_sniper_roof->AddPath(wp_blue_ground_front); //Path_MoveTo, 1
	wp_blue_sniper->AddPath(wpt4); //Path_MoveTo, -1

	wp_blue_sniper->AddPath(wp_blue_ground_front); //Path_MoveTo, 1
	wp_blue_teleporter_level->AddPath(wp_blue_teleporter_base); //Path_MoveTo, -1
	wp_blue_teleporter_level->AddPath(wpt16); //Path_MoveTo, 1
	wp_blue_teleporter_level->AddPath(wp7); //Path_MoveTo, 1
	wp7->AddPath(wp_blue_teleporter_level); //Path_MoveTo, 1
	wpt16->AddPath(wp_blue_teleporter_level); //Path_MoveTo, -1
	wpt16->AddPath(wp_blue_ground_front); //Path_MoveTo, 1
	wp_red_teleporter_base->AddPath(wp_red_teleporter_roof); //Path_MoveTo, -1
	wp_red_teleporter_base->AddPath(wp_red_teleporter_sniper); //Path_MoveTo, -1
	wp_red_teleporter_base->AddPath(wp_red_teleporter_level); //Path_MoveTo, -1
	wp_red_teleporter_base->AddPath(wp_red_ground_back); //Path_MoveTo, 1
	wp_red_teleporter_base->AddPath(wp_red_ground_entrance); //Path_MoveTo, 1

	wp_red_ground_back->AddPath(wp_red_teleporter_base); //Path_MoveTo, 1
	wp_red_ground_entrance->AddPath(wp_red_teleporter_base); //Path_MoveTo, 1
	wp_red_teleporter_sniper->AddPath(wp_red_teleporter_base); //Path_MoveTo, 1
	wp_red_teleporter_sniper->AddPath(wpt24); //Path_MoveTo, -1
	wp_red_teleporter_roof->AddPath(wp_red_teleporter_base); //Path_MoveTo, 1
	wp_red_teleporter_roof->AddPath(wpt26); //Path_MoveTo, -1
	wp_red_teleporter_level->AddPath(wp_red_teleporter_base); //Path_MoveTo, 1
	wp_red_teleporter_level->AddPath(wpt22); //Path_MoveTo, -1
	wp_red_teleporter_level->AddPath(wp3); //Path_MoveTo, 1
	wp3->AddPath(wp_red_teleporter_level); //Path_MoveTo, 1
	wpt22->AddPath(wp_red_teleporter_level); //Path_MoveTo, 1
	wpt22->AddPath(wp_red_ground_front); //Path_MoveTo, 1
	wp_red_sniper->AddPath(wpt24); //Path_MoveTo, 1

	wp_red_sniper->AddPath(wp_red_ground_front); //Path_MoveTo, 1
	wpt24->AddPath(wp_red_teleporter_sniper); //Path_MoveTo, 1
	wpt24->AddPath(wp_red_sniper); //Path_MoveTo, -1
	wp_red_sniper_roof->AddPath(wpt26); //Path_MoveTo, 1
	wp_red_sniper_roof->AddPath(wp_red_ground_front); //Path_MoveTo, 1
	wpt26->AddPath(wp_red_teleporter_roof); //Path_MoveTo, 1
	wpt26->AddPath(wp_red_sniper_roof); //Path_MoveTo, -1

	wp18->AddPath(wp_blue_jumppad); //Path_MoveTo, 1
	wp19->AddPath(wp_red_jumppad); //Path_MoveTo, 1
	wp_blue_jumppad->AddPath(wp_blue_teleporter_base); //Path_MoveTo, 1
	wp_red_jumppad->AddPath(wp_red_teleporter_base); //Path_MoveTo, 1

/*
	wp_blue_ground_front->SetInfo(WPInfo_Defend, 1);
	wp_blue_ground_entrance->SetInfo(WPInfo_Defend, 1);
	wp_blue_ground_back->SetInfo(WPInfo_Defend, 1);
	wp_blue_teleporter_base->SetInfo(WPInfo_Defend,1);
	wp7->SetInfo(WPInfo_Defend, 1);
	wp_blue_sniper->SetInfo(WPInfo_Sniper,1);
	wp_blue_sniper_roof->SetInfo(WPInfo_Sniper,1);

	wp_red_ground_front->SetInfo(WPInfo_Defend, 2);
	wp_red_ground_entrance->SetInfo(WPInfo_Defend, 2);
	wp_red_ground_back->SetInfo(WPInfo_Defend, 2);
	wp_red_teleporter_base->SetInfo(WPInfo_Defend,2);
	wp3->SetInfo(WPInfo_Defend, 2);
	wp_red_sniper->SetInfo(WPInfo_Sniper,2);
	wp_red_sniper_roof->SetInfo(WPInfo_Sniper,2);



	wp_blue_teleporter_base->SetArriveCommand(0, 0, "Call", b_ent, 0, 0, 0, "ContainedRight");
	wp_blue_teleporter_base->SetArriveCommand(1, 0, "Call", b_ent, 0, 0, 0, "ContainedUp");
	wp_blue_teleporter_base->SetArriveCommand(2, 0, "Call", b_ent, 0, 0, 0, "ContainedLeft");
	wp_blue_teleporter_roof->SetArriveCommand(0, 0, "Enter", b_ex1, 0, 0, 0, 0);
	wp_blue_teleporter_sniper->SetArriveCommand(0, 0, "Enter", b_ex2, 0, 0, 0, 0);
	wp_blue_teleporter_level->SetArriveCommand(0, 0, "Enter", b_ex3, 0, 0, 0, 0);

	wp_red_teleporter_base->SetArriveCommand(0, 0, "Call", r_ent, 0, 0, 0, "ContainedUp");
	wp_red_teleporter_base->SetArriveCommand(1, 0, "Call", r_ent, 0, 0, 0, "ContainedLeft");
	wp_red_teleporter_base->SetArriveCommand(2, 0, "Call", r_ent, 0, 0, 0, "ContainedRight");
	wp_red_teleporter_roof->SetArriveCommand(0, 0, "Enter", r_ex1, 0, 0, 0, 0);
	wp_red_teleporter_sniper->SetArriveCommand(0, 0, "Enter", r_ex2, 0, 0, 0, 0);
	wp_red_teleporter_level->SetArriveCommand(0, 0, "Enter", r_ex3, 0, 0, 0, 0);

	wp_blue_jumppad ->SetArriveCommand(0, 0, "MoveTo", 0, 155, 665);
	wp_red_jumppad->SetArriveCommand(0, 0, "MoveTo", 0, LandscapeWidth()-155, 665);

	wp_blue_teleporter_base->SetPathLength( wp_blue_teleporter_base->GetPathID(wp_blue_teleporter_sniper), 1);
	wp_blue_teleporter_base->SetPathLength( wp_blue_teleporter_base->GetPathID(wp_blue_teleporter_roof), 1);
	wp_blue_teleporter_base->SetPathLength( wp_blue_teleporter_base->GetPathID(wp_blue_teleporter_level), 1);

	wp_blue_teleporter_sniper->SetPathLength( wp_blue_teleporter_sniper->GetPathID(wp_blue_teleporter_base), 1);
	wp_blue_teleporter_roof->SetPathLength( wp_blue_teleporter_roof->GetPathID(wp_blue_teleporter_base), 1);
	wp_blue_teleporter_level->SetPathLength( wp_blue_teleporter_level->GetPathID(wp_blue_teleporter_base), 1);


	wp_red_teleporter_base->SetPathLength( wp_red_teleporter_base->GetPathID(wp_red_teleporter_sniper), 1);
	wp_red_teleporter_base->SetPathLength( wp_red_teleporter_base->GetPathID(wp_red_teleporter_roof), 1);
	wp_red_teleporter_base->SetPathLength( wp_red_teleporter_base->GetPathID(wp_red_teleporter_level), 1);

	wp_red_teleporter_sniper->SetPathLength( wp_red_teleporter_sniper->GetPathID(wp_red_teleporter_base), 1);
	wp_red_teleporter_roof->SetPathLength( wp_red_teleporter_roof->GetPathID(wp_red_teleporter_base), 1);
	wp_red_teleporter_level->SetPathLength( wp_red_teleporter_level->GetPathID(wp_red_teleporter_base), 1);

	wp_red_jumppad->SetPathLength(0, 1);
	wp_blue_jumppad->SetPathLength(0, 1);


	wp9->SetPathLength( wp9->GetPathID(wp10), 35);
	wp10->SetPathLength( wp10->GetPathID(wp9), 35);

	wp12->SetPathLength( wp12->GetPathID(wp10), 10);
	wp10->SetPathLength( wp10->GetPathID(wp12), 10);
	wp12->SetPathLength( wp12->GetPathID(wp15), 10);
	wp15->SetPathLength( wp15->GetPathID(wp12), 10);
	wp9->SetPathLength( wp9->GetPathID(wp11), 10);
	wp11->SetPathLength( wp11->GetPathID(wp9), 10);
	wp13->SetPathLength( wp13->GetPathID(wp11), 10);
	wp11->SetPathLength( wp11->GetPathID(wp13), 10);
	wp13->SetPathLength( wp13->GetPathID(wp14), 10);
	wp14->SetPathLength( wp14->GetPathID(wp13), 10);
	wp15->SetPathLength( wp15->GetPathID(wp14), 10);
	wp14->SetPathLength( wp14->GetPathID(wp15), 10);


	wp18->SetPathLength( wp18->GetPathID(wp17), 100);
	wp17->SetPathLength( wp17->GetPathID(wp18), 50);

	wp19->SetPathLength( wp19->GetPathID(wp16), 100);
	wp16->SetPathLength( wp16->GetPathID(wp19), 50);*/
}
