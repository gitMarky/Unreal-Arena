#include Clonk
#include Library_ModularAppearance

local skin_gender;
local skin_team;

func SetSkin(int new_skin)
{
	// Remember skin
	skin = new_skin;
	
	// determine skin data
	if (skin == 0 || skin == 1)	skin_name = "soldier"; //"commander";
	if (skin == 2 || skin == 3)	skin_name = "soldier";
	if (skin == 0 || skin == 2)	skin_gender = "m";
	if (skin == 1 || skin == 3)	skin_gender = "f";
	
	RemoveBackpack(); // remove the backpack

	// Go back to original action afterwards and hope
	// that noone calls SetSkin during more complex activities
	var prev_action = GetAction();
	SetAction("Jump");
	SetAction(prev_action);
	SetOwner(GetOwner()); // somehow the color did not get updated after setting the color
	
	var skins = GetSkinTextures();

	// TODO: use the actual skin data instead of predefined strings
	AddAppearance(0, new Skin_Definition { GraphicsDefinition = UA_Clonk_Skins, GraphicsName = "MSoldierBody", Flags = AM_MatchSkeleton, MeshMaterial = [skins.body, skins.arms]});
	AddAppearance(1, new Skin_Definition { GraphicsDefinition = UA_Clonk_Skins, GraphicsName = "MSoldierLegs", Flags = AM_MatchSkeleton, MeshMaterial = ["gore_torso", skins.legs, skins.arms]});
	AddAppearance(2, new Skin_Definition { GraphicsDefinition = UA_Clonk_Skins, GraphicsName = "MSoldierArmL", Flags = AM_MatchSkeleton, MeshMaterial = [skins.body, skins.arms, skins.head]});
	AddAppearance(3, new Skin_Definition { GraphicsDefinition = UA_Clonk_Skins, GraphicsName = "MSoldierArmR", Flags = AM_MatchSkeleton, MeshMaterial = [skins.body, skins.arms, skins.head]});
	AddAppearance(4, new Skin_Definition { GraphicsDefinition = UA_Clonk_Skins, GraphicsName = "MSoldierHead", Flags = AM_MatchSkeleton, MeshMaterial = [skins.head]});

	return skin;
}

func GetSkinTextures()
{
	var skin_suffix = "_guard";
	var prefix = Format("%s_%s%s", skin_gender, skin_name, skin_suffix);
	return {body = Format("%s_body", prefix),
            legs = Format("%s_legs", prefix),
            arms = Format("%s_arms", prefix),
            head = Format("%s_head", prefix),
           };
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

