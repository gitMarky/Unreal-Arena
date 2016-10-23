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
	DoSplatter(RemoveAppearance(0))->VertexSetupBody();
	DoSplatter(RemoveAppearance(1))->VertexSetupLegs();
	DoSplatter(RemoveAppearance(2))->VertexSetupArmL();
	DoSplatter(RemoveAppearance(3))->VertexSetupArmR();
	DoSplatter(RemoveAppearance(4))->VertexSetupHead();
}


func DoSplatter(proplist skin)
{
	if (!skin) return;
	var corpse = CreateObject(UA_Clonk_Corpse, 0, 0, NO_OWNER);
	corpse->AddAppearance(0, skin);
	corpse->SetSpeed(RandomX(-1, 1) * 30, RandomX(-20, -60));
	corpse->SetRDir(corpse->GetXDir()/3 -1);
	corpse->SetAction(GetAction());
	corpse->SetDir(GetDir());
	corpse->SetPhase(GetPhase());
	corpse->CopyAnimationPositionFrom(this);
	corpse.MeshTransformation = this.MeshTransformation;
	ScheduleCall(corpse, corpse.StartSplatter, 15);
	return corpse;
}

// Better death animation

func StartDead()
{
	// Copy all animations except for the death slot from the target
	var dead = false;	
	for (var slot = 0; slot < CLONK_ANIM_SLOT_Death; ++slot)
	{
		if (GetRootAnimation(slot) == nil) continue;
		OverlayDeathAnimation(slot);
		dead = true;
	}
	
	if (!dead)
	{
		OverlayDeathAnimation(CLONK_ANIM_SLOT_Death);
	}

	// Update carried items
	UpdateAttach();
	// Set proper turn type
	SetTurnType(1);
}

func OverlayDeathAnimation(int slot)
{
	var animation = "Dead";
	PlayAnimation(animation, slot, Anim_Linear(0, 0, GetAnimationLength(animation), 20, ANIM_Hold), Anim_Linear(0, 0, 1000, 10, ANIM_Remove));
}
