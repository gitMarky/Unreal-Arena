#include Clonk
#include Library_ModularAppearance

local skin_gender;
local skin_team;

func SetSkin(int new_skin)
{
	// Remember skin
	skin = new_skin;
	
	// determine skin data
	if (skin == 0 || skin == 1)	skin_name = "Commander";
	if (skin == 2 || skin == 3)	skin_name = "Soldier";
	if (skin == 0 || skin == 2)	skin_gender = "M";
	if (skin == 1 || skin == 3)	skin_gender = "F";
	
	RemoveBackpack(); // remove the backpack

	// Go back to original action afterwards and hope
	// that noone calls SetSkin during more complex activities
	var prev_action = GetAction();
	SetAction("Jump");
	SetAction(prev_action);
	SetOwner(GetOwner()); // somehow the color did not get updated after setting the color

	// TODO: use the actual skin data instead of predefined strings
	AddAppearance(0, new Skin_Definition { GraphicsDefinition = UA_Clonk_Skins, GraphicsName = "MSoldierBody", Flags = AM_MatchSkeleton});
	AddAppearance(1, new Skin_Definition { GraphicsDefinition = UA_Clonk_Skins, GraphicsName = "MSoldierLegs", Flags = AM_MatchSkeleton});
	AddAppearance(2, new Skin_Definition { GraphicsDefinition = UA_Clonk_Skins, GraphicsName = "MSoldierArmL", Flags = AM_MatchSkeleton});
	AddAppearance(3, new Skin_Definition { GraphicsDefinition = UA_Clonk_Skins, GraphicsName = "MSoldierArmR", Flags = AM_MatchSkeleton});
	AddAppearance(4, new Skin_Definition { GraphicsDefinition = UA_Clonk_Skins, GraphicsName = "MSoldierHead", Flags = AM_MatchSkeleton});

	return skin;
}

func Splatter()
{
	DoSplatter(RemoveAppearance(0));
	DoSplatter(RemoveAppearance(1));
	DoSplatter(RemoveAppearance(2));
	DoSplatter(RemoveAppearance(3));
	DoSplatter(RemoveAppearance(4));
}


func DoSplatter(proplist skin)
{
	if (!skin) return;
	var corpse = CreateObject(UA_Clonk_Corpse, 0, 0, NO_OWNER);
	corpse->AddAppearance(0, skin);
	corpse->SetSpeed(RandomX(-1, 1) * 30, RandomX(-20, -60));
	corpse->StartDead();
//	corpse.Visibility = VIS_All;
}
