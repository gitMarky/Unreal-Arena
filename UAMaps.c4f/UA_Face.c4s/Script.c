#strict
#include _LSS

static Flags;
static AlertLights;

public func Initialize()
{
	// Waffenset
	gIDWeaponSet = W_U_; // Standard Unreal-Set
	gIDWeaponSet->~ChooserFinished();
	  Flags = CreateArray(3);

	// Deko
	CreateMapObjects();
	CreateWaypoints();
}

public func CreateWaypoints()
{

	var shift = -86;

	var wp_red_ground_front = CreateWP(1307,575);
	var wp_red_ground_entrance = CreateWP(1439,581);
	var wp3 = CreateWP(1632,485);
	var wp_red_ground_back = CreateWP(1651,581);
	var wp_blue_ground_front = CreateWP(598,575);
	var wp_blue_ground_entrance = CreateWP(466,581);
	var wp7 = CreateWP(273,485);
	var wp_blue_ground_back = CreateWP(254,581);
	var wp9 = CreateWP(871,529);
	var wp10 = CreateWP(1034,529);
	var wp11 = CreateWP(971,579);
	var wp12 = CreateWP(929,579);
	var wp13 = CreateWP(1082,619);
	var wp14 = CreateWP(953,655);
	var wp15 = CreateWP(822,619);
	var wp16 = CreateWP(1355,665);
	var wp17 = CreateWP(552,665);
	var wp18 = CreateWP(253,649);
	var wp19 = CreateWP(1655,650);

//	var wp_blue_jumppad = CreateWP(155, 665);
//	var wp_red_jumppad = CreateWP(LandscapeWidth()-155, 665);
	var wp_blue_jumppad = CreateWP(190, 665);
	var wp_red_jumppad = CreateWP(LandscapeWidth()-190, 665);

	var wp_blue_teleporter_sniper = CreateWP(398,368);
	var wp_blue_teleporter_roof = CreateWP(330,212);
	var wpt3 = CreateWP(290,185);
	var wpt4 = CreateWP(422,367);
	var wp_blue_teleporter_base = CreateWP(307,581);
	var wp_blue_sniper_roof = CreateWP(406,185);
	var wp_blue_sniper = CreateWP(433+5,368);
	var wp_blue_teleporter_level = CreateWP(420,485);
	var wpt16 = CreateWP(442+13,485);
	var wp_red_teleporter_base = CreateWP(1684+shift,581); // wp_blue_teleporter_base
	var wp_red_teleporter_sniper = CreateWP(1605+shift,368); // wp_blue_teleporter_sniper
	var wp_red_teleporter_roof = CreateWP(1673+shift,212); //wp_blue_teleporter_roof
	var wp_red_teleporter_level = CreateWP(1577+shift,485); // wp_blue_teleporter_level
	var wpt22 = CreateWP(1554-13+shift,485); // wpt16
	var wp_red_sniper = CreateWP(1562-5+shift,368); // wp_blue_sniper
	var wpt24 = CreateWP(1570+shift,368); //wpt24
	var wp_red_sniper_roof = CreateWP(LandscapeWidth()-406,185); // wp_blue_sniper_roof
	var wpt26 = CreateWP(LandscapeWidth()-290,185); // wpt3

	wp_red_ground_front -> SetName("wp_red_ground_front");
	wp_red_ground_entrance -> SetName("wp_red_ground_entrance");
	wp3 -> SetName("wp3");
	wp_red_ground_back -> SetName("wp_red_ground_back");
	wp_blue_ground_front -> SetName("wp_blue_ground_front");
	wp_blue_ground_entrance -> SetName("wp_blue_ground_entrance");
	wp7 -> SetName("wp7");
	wp_blue_ground_back -> SetName("wp_blue_ground_back");
	wp9 -> SetName("wp9");
	wp10 -> SetName("wp10");
	wp11 -> SetName("wp11");
	wp12 -> SetName("wp12");
	wp13 -> SetName("wp13");
	wp14 -> SetName("wp14");
	wp15 -> SetName("wp15");
	wp16 -> SetName("wp16");
	wp17 -> SetName("wp17");
	wp18 -> SetName("wp18");
	wp19 -> SetName("wp19");
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



	wp_red_ground_front -> AddPath(wp10, Path_MoveTo, 1);
	wp_red_ground_front -> AddPath(wp_red_ground_entrance, Path_MoveTo, 1);
	wp_red_ground_entrance -> AddPath(wp_red_ground_front, Path_MoveTo, 1);
	wp_red_ground_back -> AddPath(wp19, Path_MoveTo, -1);
	wp_blue_ground_front -> AddPath(wp9, Path_MoveTo, 1);
	wp_blue_ground_front -> AddPath(wp_blue_ground_entrance, Path_MoveTo, 1);
	wp_blue_ground_entrance -> AddPath(wp_blue_ground_front, Path_MoveTo, 1);
	wp_blue_ground_back -> AddPath(wp18, Path_MoveTo, -1);
	wp9 -> AddPath(wp10, Path_Jump, 1);
	wp9 -> AddPath(wp11, Path_MoveTo, 1);
	wp9 -> AddPath(wp_blue_ground_front, Path_MoveTo, -1);
	wp10 -> AddPath(wp9, Path_Jump, -1);
	wp10 -> AddPath(wp12, Path_MoveTo, -1);
	wp10 -> AddPath(wp_red_ground_front, Path_MoveTo, 1);
	wp11 -> AddPath(wp9, Path_Jump, -1);
	wp11 -> AddPath(wp12, Path_MoveTo, -1);
	wp11 -> AddPath(wp13, Path_MoveTo, 1);
	wp12 -> AddPath(wp10, Path_Jump, 1);
	wp12 -> AddPath(wp11, Path_MoveTo, 1);
	wp12 -> AddPath(wp15, Path_MoveTo, -1);
	wp13 -> AddPath(wp11, Path_Jump, -1);
	wp13 -> AddPath(wp14, Path_MoveTo, -1);
	wp13 -> AddPath(wp16, Path_MoveTo, 1);
	wp14 -> AddPath(wp15, Path_MoveTo, -1);
	wp14 -> AddPath(wp13, Path_MoveTo, 1);
	wp15 -> AddPath(wp12, Path_Jump, 1);
	wp15 -> AddPath(wp14, Path_MoveTo, 1);
	wp15 -> AddPath(wp17, Path_MoveTo, -1);
	wp16 -> AddPath(wp19, Path_MoveTo, 1);
	wp16 -> AddPath(wp13, Path_MoveTo, -1);
	wp17 -> AddPath(wp15, Path_MoveTo, 1);
	wp17 -> AddPath(wp18, Path_MoveTo, -1);
	wp18 -> AddPath(wp17, Path_MoveTo, 1);
	wp19 -> AddPath(wp16, Path_MoveTo, -1);





	wp_blue_teleporter_sniper -> AddPath(wp_blue_teleporter_base, Path_MoveTo, -1);
	wp_blue_teleporter_sniper -> AddPath(wpt4, Path_MoveTo, 1);
	wp_blue_teleporter_roof -> AddPath(wp_blue_teleporter_base, Path_MoveTo, -1);
	wp_blue_teleporter_roof -> AddPath(wpt3, Path_MoveTo, 1);
	wpt3 -> AddPath(wp_blue_teleporter_roof, Path_MoveTo, -1);
	wpt3 -> AddPath(wp_blue_sniper_roof, Path_MoveTo, 1);
	wpt4 -> AddPath(wp_blue_teleporter_sniper, Path_MoveTo, -1);
	wpt4 -> AddPath(wp_blue_sniper, Path_MoveTo, 1);
	wp_blue_teleporter_base -> AddPath(wp_blue_teleporter_sniper, Path_MoveTo, 1);
	wp_blue_teleporter_base -> AddPath(wp_blue_teleporter_roof, Path_MoveTo, 1);
	wp_blue_teleporter_base -> AddPath(wp_blue_teleporter_level, Path_MoveTo, 1);




	wp_blue_teleporter_base -> AddPath(wp_blue_ground_back, Path_MoveTo, 1);
	wp_blue_teleporter_base -> AddPath(wp_blue_ground_entrance, Path_MoveTo, 1);
	wp_blue_ground_back -> AddPath(wp_blue_teleporter_base, Path_MoveTo, 1);
	wp_blue_ground_entrance -> AddPath(wp_blue_teleporter_base, Path_MoveTo, 1);
	wp_blue_sniper_roof -> AddPath(wpt3, Path_MoveTo, -1);
	wp_blue_sniper_roof -> AddPath(wp_blue_ground_front, Path_MoveTo, 1);
	wp_blue_sniper -> AddPath(wpt4, Path_MoveTo, -1);

	wp_blue_sniper -> AddPath(wp_blue_ground_front, Path_MoveTo, 1);
	wp_blue_teleporter_level -> AddPath(wp_blue_teleporter_base, Path_MoveTo, -1);
	wp_blue_teleporter_level -> AddPath(wpt16, Path_MoveTo, 1);
	wp_blue_teleporter_level -> AddPath(wp7, Path_MoveTo, 1);
	wp7 -> AddPath(wp_blue_teleporter_level, Path_MoveTo, 1);
	wpt16 -> AddPath(wp_blue_teleporter_level, Path_MoveTo, -1);
	wpt16 -> AddPath(wp_blue_ground_front, Path_MoveTo, 1);
	wp_red_teleporter_base -> AddPath(wp_red_teleporter_roof, Path_MoveTo, -1);
	wp_red_teleporter_base -> AddPath(wp_red_teleporter_sniper, Path_MoveTo, -1);
	wp_red_teleporter_base -> AddPath(wp_red_teleporter_level, Path_MoveTo, -1);
	wp_red_teleporter_base -> AddPath(wp_red_ground_back, Path_MoveTo, 1);
	wp_red_teleporter_base -> AddPath(wp_red_ground_entrance, Path_MoveTo, 1);

	wp_red_ground_back -> AddPath(wp_red_teleporter_base, Path_MoveTo, 1);
	wp_red_ground_entrance -> AddPath(wp_red_teleporter_base, Path_MoveTo, 1);
	wp_red_teleporter_sniper -> AddPath(wp_red_teleporter_base, Path_MoveTo, 1);
	wp_red_teleporter_sniper -> AddPath(wpt24, Path_MoveTo, -1);
	wp_red_teleporter_roof -> AddPath(wp_red_teleporter_base, Path_MoveTo, 1);
	wp_red_teleporter_roof -> AddPath(wpt26, Path_MoveTo, -1);
	wp_red_teleporter_level -> AddPath(wp_red_teleporter_base, Path_MoveTo, 1);
	wp_red_teleporter_level -> AddPath(wpt22, Path_MoveTo, -1);
	wp_red_teleporter_level -> AddPath(wp3, Path_MoveTo, 1);
	wp3 -> AddPath(wp_red_teleporter_level, Path_MoveTo, 1);
	wpt22 -> AddPath(wp_red_teleporter_level, Path_MoveTo, 1);
	wpt22 -> AddPath(wp_red_ground_front, Path_MoveTo, 1);
	wp_red_sniper -> AddPath(wpt24, Path_MoveTo, 1);

	wp_red_sniper -> AddPath(wp_red_ground_front, Path_MoveTo, 1);
	wpt24 -> AddPath(wp_red_teleporter_sniper, Path_MoveTo, 1);
	wpt24 -> AddPath(wp_red_sniper, Path_MoveTo, -1);
	wp_red_sniper_roof -> AddPath(wpt26, Path_MoveTo, 1);
	wp_red_sniper_roof -> AddPath(wp_red_ground_front, Path_MoveTo, 1);
	wpt26 -> AddPath(wp_red_teleporter_roof, Path_MoveTo, 1);
	wpt26 -> AddPath(wp_red_sniper_roof, Path_MoveTo, -1);

	wp18 -> AddPath(wp_blue_jumppad, Path_MoveTo, 1);
	wp19 -> AddPath(wp_red_jumppad, Path_MoveTo, 1);
	wp_blue_jumppad -> AddPath(wp_blue_teleporter_base, Path_MoveTo, 1);
	wp_red_jumppad -> AddPath(wp_red_teleporter_base, Path_MoveTo, 1);


	wp_blue_ground_front -> SetInfo(WPInfo_Defend, 1);
	wp_blue_ground_entrance -> SetInfo(WPInfo_Defend, 1);
	wp_blue_ground_back -> SetInfo(WPInfo_Defend, 1);
	wp_blue_teleporter_base -> SetInfo(WPInfo_Defend,1);
	wp7 -> SetInfo(WPInfo_Defend, 1);
	wp_blue_sniper -> SetInfo(WPInfo_Sniper,1);
	wp_blue_sniper_roof -> SetInfo(WPInfo_Sniper,1);

	wp_red_ground_front -> SetInfo(WPInfo_Defend, 2);
	wp_red_ground_entrance -> SetInfo(WPInfo_Defend, 2);
	wp_red_ground_back -> SetInfo(WPInfo_Defend, 2);
	wp_red_teleporter_base -> SetInfo(WPInfo_Defend,2);
	wp3 -> SetInfo(WPInfo_Defend, 2);
	wp_red_sniper -> SetInfo(WPInfo_Sniper,2);
	wp_red_sniper_roof -> SetInfo(WPInfo_Sniper,2);



	wp_blue_teleporter_base -> SetArriveCommand(0, 0, "Call", b_ent, 0, 0, 0, "ContainedRight");
	wp_blue_teleporter_base -> SetArriveCommand(1, 0, "Call", b_ent, 0, 0, 0, "ContainedUp");
	wp_blue_teleporter_base -> SetArriveCommand(2, 0, "Call", b_ent, 0, 0, 0, "ContainedLeft");
	wp_blue_teleporter_roof -> SetArriveCommand(0, 0, "Enter", b_ex1, 0, 0, 0, 0);
	wp_blue_teleporter_sniper -> SetArriveCommand(0, 0, "Enter", b_ex2, 0, 0, 0, 0);
	wp_blue_teleporter_level -> SetArriveCommand(0, 0, "Enter", b_ex3, 0, 0, 0, 0);

	wp_red_teleporter_base -> SetArriveCommand(0, 0, "Call", r_ent, 0, 0, 0, "ContainedUp");
	wp_red_teleporter_base -> SetArriveCommand(1, 0, "Call", r_ent, 0, 0, 0, "ContainedLeft");
	wp_red_teleporter_base -> SetArriveCommand(2, 0, "Call", r_ent, 0, 0, 0, "ContainedRight");
	wp_red_teleporter_roof -> SetArriveCommand(0, 0, "Enter", r_ex1, 0, 0, 0, 0);
	wp_red_teleporter_sniper -> SetArriveCommand(0, 0, "Enter", r_ex2, 0, 0, 0, 0);
	wp_red_teleporter_level -> SetArriveCommand(0, 0, "Enter", r_ex3, 0, 0, 0, 0);

	wp_blue_jumppad ->SetArriveCommand(0, 0, "MoveTo", 0, 155, 665);
	wp_red_jumppad -> SetArriveCommand(0, 0, "MoveTo", 0, LandscapeWidth()-155, 665);

	wp_blue_teleporter_base -> SetPathLength( wp_blue_teleporter_base -> GetPathID(wp_blue_teleporter_sniper), 1);
	wp_blue_teleporter_base -> SetPathLength( wp_blue_teleporter_base -> GetPathID(wp_blue_teleporter_roof), 1);
	wp_blue_teleporter_base -> SetPathLength( wp_blue_teleporter_base -> GetPathID(wp_blue_teleporter_level), 1);

	wp_blue_teleporter_sniper -> SetPathLength( wp_blue_teleporter_sniper -> GetPathID(wp_blue_teleporter_base), 1);
	wp_blue_teleporter_roof -> SetPathLength( wp_blue_teleporter_roof -> GetPathID(wp_blue_teleporter_base), 1);
	wp_blue_teleporter_level -> SetPathLength( wp_blue_teleporter_level -> GetPathID(wp_blue_teleporter_base), 1);


	wp_red_teleporter_base -> SetPathLength( wp_red_teleporter_base -> GetPathID(wp_red_teleporter_sniper), 1);
	wp_red_teleporter_base -> SetPathLength( wp_red_teleporter_base -> GetPathID(wp_red_teleporter_roof), 1);
	wp_red_teleporter_base -> SetPathLength( wp_red_teleporter_base -> GetPathID(wp_red_teleporter_level), 1);

	wp_red_teleporter_sniper -> SetPathLength( wp_red_teleporter_sniper -> GetPathID(wp_red_teleporter_base), 1);
	wp_red_teleporter_roof -> SetPathLength( wp_red_teleporter_roof -> GetPathID(wp_red_teleporter_base), 1);
	wp_red_teleporter_level -> SetPathLength( wp_red_teleporter_level -> GetPathID(wp_red_teleporter_base), 1);

	wp_red_jumppad -> SetPathLength(0, 1);
	wp_blue_jumppad -> SetPathLength(0, 1);


	wp9 -> SetPathLength( wp9->GetPathID(wp10), 35);
	wp10 -> SetPathLength( wp10->GetPathID(wp9), 35);

	wp12 -> SetPathLength( wp12->GetPathID(wp10), 10);
	wp10 -> SetPathLength( wp10->GetPathID(wp12), 10);
	wp12 -> SetPathLength( wp12->GetPathID(wp15), 10);
	wp15 -> SetPathLength( wp15->GetPathID(wp12), 10);
	wp9 -> SetPathLength( wp9->GetPathID(wp11), 10);
	wp11 -> SetPathLength( wp11->GetPathID(wp9), 10);
	wp13 -> SetPathLength( wp13->GetPathID(wp11), 10);
	wp11 -> SetPathLength( wp11->GetPathID(wp13), 10);
	wp13 -> SetPathLength( wp13->GetPathID(wp14), 10);
	wp14 -> SetPathLength( wp14->GetPathID(wp13), 10);
	wp15 -> SetPathLength( wp15->GetPathID(wp14), 10);
	wp14 -> SetPathLength( wp14->GetPathID(wp15), 10);


	wp18 -> SetPathLength( wp18->GetPathID(wp17), 100);
	wp17 -> SetPathLength( wp17->GetPathID(wp18), 50);

	wp19 -> SetPathLength( wp19->GetPathID(wp16), 100);
	wp16 -> SetPathLength( wp16->GetPathID(wp19), 50);

	CreateTeleportPoints();
}

public func CreateTeleportPoints()
{

		//WaypointsVisible(true);

}

public func CreateMapObjects()
{

	var w = LandscapeWidth();

	// ASMD
	PlaceSpawnpointEx("gWeaponPos04", 411    ,581);
	PlaceSpawnpointEx("gAmmoPos04", 411+20   ,581);
	PlaceSpawnpointEx("gAmmoPos04", 411-20   ,581);

	PlaceSpawnpointEx("gWeaponPos04",-411 + w,581);
	PlaceSpawnpointEx("gAmmoPos04",-411+20+ w,581);
	PlaceSpawnpointEx("gAmmoPos04",-411-20+ w,581);

	// Linkgun
	PlaceSpawnpointEx("gWeaponPos05", 284    ,581);
	PlaceSpawnpointEx("gAmmoPos05", 284+20   ,581);
	PlaceSpawnpointEx("gAmmoPos05", 284-20   ,581);

	PlaceSpawnpointEx("gWeaponPos05",-284 + w,581);
	PlaceSpawnpointEx("gAmmoPos05",-284+20+ w,581);
	PlaceSpawnpointEx("gAmmoPos05",-284-20+ w,581);

	// Rocket Launcher
	PlaceSpawnpointEx("gWeaponPos09", 272    ,490);
	PlaceSpawnpointEx("gAmmoPos09", 272-20   ,490);
	PlaceSpawnpointEx("gAmmoPos09", 272+20   ,490);

	PlaceSpawnpointEx("gWeaponPos09",-272 + w,490);
	PlaceSpawnpointEx("gAmmoPos09",-272-20+ w,490);
	PlaceSpawnpointEx("gAmmoPos09",-272+20+ w,490);


	// Sniper
	PlaceSpawnpointEx("gWeaponPos10", 400    ,369);
	PlaceSpawnpointEx("gAmmoPos10", 400-20   ,369);
	PlaceSpawnpointEx("gAmmoPos10", 400+20   ,369);

	PlaceSpawnpointEx("gWeaponPos10",-400 + w,369);
	PlaceSpawnpointEx("gAmmoPos10",-400-20+ w,369);
	PlaceSpawnpointEx("gAmmoPos10",-400+20+ w,369);

	PlaceSpawnpointEx("gWeaponPos10", 400    ,195);
	PlaceSpawnpointEx("gAmmoPos10", 400-20   ,195);
	PlaceSpawnpointEx("gAmmoPos10", 400-30   ,195);

	PlaceSpawnpointEx("gWeaponPos10",-400 + w,195);
	PlaceSpawnpointEx("gAmmoPos10",-400+20+ w,195);
	PlaceSpawnpointEx("gAmmoPos10",-400+30+ w,195);

	// Ripper
	PlaceSpawnpointEx("gWeaponPos06", 550    ,665);
	PlaceSpawnpointEx("gAmmoPos06", 550-20   ,665);
	PlaceSpawnpointEx("gAmmoPos06", 550+20   ,665);

	PlaceSpawnpointEx("gWeaponPos06",-550 + w,665);
	PlaceSpawnpointEx("gAmmoPos06",-550-20+ w,665);
	PlaceSpawnpointEx("gAmmoPos06",-550+20+ w,665);

	// Redeemer
	PlaceSpawnpointEx("gWeaponPos11", 635, 415);
	PlaceSpawnpointEx("gWeaponPos11",-635+w, 415);

	// Armor
	PlaceSpawnpointEx("gItemPos05", 290    ,195);
	PlaceSpawnpointEx("gItemPos05",-290 + w,195);

	// Health Vial
	PlaceSpawnpointEx("gItemPos01", 285,650);
	PlaceSpawnpointEx("gItemPos01", 315,650);
	PlaceSpawnpointEx("gItemPos01", 345,650);
	PlaceSpawnpointEx("gItemPos01", 375,650);
	PlaceSpawnpointEx("gItemPos01", 405,650);

	PlaceSpawnpointEx("gItemPos01",-285+w,650);
	PlaceSpawnpointEx("gItemPos01",-315+w,650);
	PlaceSpawnpointEx("gItemPos01",-345+w,650);
	PlaceSpawnpointEx("gItemPos01",-375+w,650);
	PlaceSpawnpointEx("gItemPos01",-405+w,650);

	// Medipack
	PlaceSpawnpointEx("gItemPos02", 347-25    ,581);
	PlaceSpawnpointEx("gItemPos02", 347+25    ,581);

	PlaceSpawnpointEx("gItemPos02", 932, 660);
	PlaceSpawnpointEx("gItemPos02", 972, 660);

	PlaceSpawnpointEx("gItemPos02",-347-25 + w,581);
	PlaceSpawnpointEx("gItemPos02",-347+25 + w,581);

	// Keg'o'Health
	PlaceSpawnpointEx("gItemPos03",952,460);


	// Special
	//PlaceSpawnpoint( 342,431,"WeaponPos13");
	//PlaceSpawnpoint(1642,431,"WeaponPos13");

	// Wegpunkte

	SetPosition( 630    ,586,CreateObject(WZ6A,0,0,-1));
	SetPosition(-630 + w,596,CreateObject(WZ6A,0,0,-1));


	// Teleporter

	var ent, ex1, ex2, ex3;
	static b_ent, b_ex1, b_ex2, b_ex3;
	static r_ent, r_ex1, r_ex2, r_ex3;

	ent = CreateObject(S__W,0,0,-1);
	ex1 = CreateObject(S__W,0,0,-1);
	ex2 = CreateObject(S__W,0,0,-1);
	ex3 = CreateObject(S__W,0,0,-1);

	SetPosition( 307, 581, ent );
	SetPosition( 323, 212, ex1 );
	SetPosition( 392, 368, ex2 );
	SetPosition( 417, 485, ex3 );

	b_ent = ent; b_ex1 = ex1; b_ex2 = ex2; b_ex3 = ex3;

	ent->~SetTo(1,RGB(0,0,232),ex3,ex2,ex1);

	ent = CreateObject(S__W,0,0,-1);
	ex1 = CreateObject(S__W,0,0,-1);
	ex2 = CreateObject(S__W,0,0,-1);
	ex3 = CreateObject(S__W,0,0,-1);

	SetPosition(-307+w, 581, ent );
	SetPosition(-323+w, 212, ex1 );
	SetPosition(-392+w, 368, ex2 );
	SetPosition(-417+w, 485, ex3 );

	r_ent = ent; r_ex1 = ex1; r_ex2 = ex2; r_ex3 = ex3;

	ent->~SetTo(2,RGB(244,0,0),ex2,ex3,ex1);

	// Himmel: Sterne und Galaxien

	var amount_stars, amount_galaxies;

	var i, x;



	// Jump pads

	var jumpPads = [[ 122, 474,  88,  37,   0],
			        [ 132, 507, 100, 115, 130],
			        [ 155, 575, 130,  10,   0],
			        [ 125, 575,  75,  28,   0],
			        [ 144, 638,  70,  80,  90],
			        [ 147, 674,  70,  30,   0],
			        // mirrored
			        [1783, 474,  88, -37,   0],
			        [1773, 507, 100,-115,-130],
			        [1750, 575, 130, -10,   0],
			        [1780, 575,  75, -28,   0],
			        [1761, 638,  70, -80, -90],
			        [1758, 674,  70, -30,   0]];

	for (var padData in jumpPads)
	{
		var pad = CreateObject(JMPD, 0, 0, -1);
		pad->SetPosition(padData[0], padData[1]);
		pad->Set(padData[2], padData[3], padData[4]);
	}

	// color jump pads

	var jumpPadsBlue = FindObjects(Find_ID(JPTP), Find_InRect(0,0,200,LandscapeHeight()));
	var jumpPadsRed = FindObjects(Find_ID(JPTP), Find_InRect(LandscapeWidth()-200,0,200,LandscapeHeight()));

	for (var pad in jumpPadsBlue) SetClrModulation(RGB(100,100,200), pad);
	for (var pad in jumpPadsRed)  SetClrModulation(RGB(200,100,100), pad);

	return;

	// Himmel: Sonne
	SetPosition( 676,LandscapeHeight()/2,CreateObject(_OAF,0,0,-1));

	CreateObject(_OAS, 0, 0, -1);


	// vents

	PlaceDeco(VENT, 348,   535, 30, 0, RGB(150, 150, 150));
	PlaceDeco(VENT,-348+w, 535, 30, 0, RGB(180, 150, 150));

	// flags
	PlaceDeco(_ODF, 318, 426, 0, 0, RGB( 20, 20, 150));
	PlaceDeco(_ODF, 378, 426, 0, 0, RGB( 20, 20, 150));

	PlaceDeco(_ODF, -318+w, 426, 0, 0,  RGB( 150, 20, 20));
	PlaceDeco(_ODF, -378+w, 426, 0, 0, RGB( 150, 20, 20));

	// Lights
	var colorLightBlue = RGBa(100, 100, 255, 70);
	var colorLightRed  = RGBa(255, 100, 100, 70);
	var colorLightDefault = RGBa(255, 255, 200, 50);

	var ceilingLights = [[347, 505, colorLightBlue],
	                     [270, 615, colorLightBlue],
	                     [315, 615, colorLightBlue],
	                     [380, 615, colorLightBlue],
	                     [425, 615, colorLightBlue],
	                     // center
	                     [932, 615, colorLightDefault],
	                     [972, 615, colorLightDefault],
	                     // mirrored
	                     [-347+w, 505, colorLightRed],
	                     [-270+w, 615, colorLightRed],
	                     [-315+w, 615, colorLightRed],
	                     [-380+w, 615, colorLightRed],
	                     [-425+w, 615, colorLightRed]];

	for (var light in ceilingLights) PlaceDeco(CLGH, light[0], light[1])->GetLight()->ChangeColor(light[2]);

	var floodLights = [[ 475,   505, 135, colorLightBlue],
	                   [ 260,   305, -30, colorLightBlue],
	                   [ 435,   305,  30, colorLightBlue],
	                   // mirrored
	                   [-475+w, 505,-135, colorLightRed],
	                   [-260+w, 305,  30, colorLightRed],
	                   [-435+w, 305, -30, colorLightRed]];

	for (var light in floodLights) PlaceFloodLight( light[0], light[1], light[2], light[3]);

	PlaceDeco(_OAZ, 360, 565)->Init(0, colorLightDefault);

	var baseLightRed = PlaceDeco(_OAZ,-360+w, 565);
	baseLightRed->Init(0, colorLightDefault);
	baseLightRed->SetObjDrawTransform(-1000, 0, 0, 0, 1000);

}

protected func InitTeamFlag(int iTeam)
{
	if (GetType(AlertLights) != C4V_Array) AlertLights = [];

	var x,y,color;
	if(iTeam == 2)
	{
		x = -347+LandscapeWidth();
		y = 590;
		color = RGB(255,0,0);

		AlertLights[iTeam] = [PlaceDeco(ALGH, -290+LandscapeWidth(), 544, 0, 180),
		                      PlaceDeco(ALGH, -405+LandscapeWidth(), 544, 0, 180)];
	}
	if(iTeam == 1)
	{
		x = 347;
		y = 590;
		color = RGB(0,0,255);

		AlertLights[iTeam] = [PlaceDeco(ALGH, 290, 544, 0, 180),
		                      PlaceDeco(ALGH, 405, 544, 0, 180)];
	}
	CreateFlag(iTeam, x, y, color);
}

protected func FlagReturned(int iTeam)
{
	for(var light in AlertLights[iTeam])
	{
		light->~TurnOff();
	}
}

protected func FlagCaptured(int iTeam)
{
	for(var light in AlertLights[iTeam])
	{
		light->~TurnOn();
	}
}

protected func InitializePlayer(int iPlr, int iX, int iY, object pBase, int iTeam)
{
	// Flagge setzen (wenn nicht schon erledigt
	if(!Flags[iTeam])
	{
		Flags[iTeam] = true;
		InitTeamFlag(iTeam);
	}
	for(var i=0, pCrew ; pCrew = GetCrew(iPlr, i) ; i++)
		RelaunchPlayer(iPlr, pCrew, 0, iTeam);
}

public func RelaunchPosition(& iX, & iY, int iTeam)
{
	if(iTeam == 2)
	{
		iX = -280-Random(45)+LandscapeWidth();
		iY = 580;
		//if(!Random(3)) iY = 25;
	}
	if(iTeam == 1)
	{
		iX = 280+Random(45);
		iY = 580;
		//if(!Random(3)) iY = 25;
	}
}

