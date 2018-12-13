#include Clonk
#include Library_ModularAppearance
#include Library_UA_PlayerStats
#include Library_UA_PlayerDamage
#include Library_UA_PlayerDeath
#include Library_AmmoManager
#include Library_BotFunctions


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// Properties

local MaxEnergy = 100000;


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// Skins

local skin_gender;
local skin_team;
local skin_suffix;

static const PLAYER_SKIN_SLOT_LEGS = 0;
static const PLAYER_SKIN_SLOT_BODY = 1;
static const PLAYER_SKIN_SLOT_ARML = 2;
static const PLAYER_SKIN_SLOT_ARMR = 3;
static const PLAYER_SKIN_SLOT_HEAD = 4;


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
	AddAppearance(PLAYER_SKIN_SLOT_BODY, new Skin_Definition { GraphicsDefinition = UA_Clonk_Skins, GraphicsName = "MSoldierBody", Flags = AM_DrawBefore | AM_MatchSkeleton, MeshMaterial = [skins.body, skins.arms]});
	AddAppearance(PLAYER_SKIN_SLOT_LEGS, new Skin_Definition { GraphicsDefinition = UA_Clonk_Skins, GraphicsName = "MSoldierLegs", Flags = AM_DrawBefore | AM_MatchSkeleton, MeshMaterial = ["gore_torso", skins.legs, skins.arms]});
	AddAppearance(PLAYER_SKIN_SLOT_ARML, new Skin_Definition { GraphicsDefinition = UA_Clonk_Skins, GraphicsName = "MSoldierArmL", Flags = AM_DrawBefore | AM_MatchSkeleton, MeshMaterial = [skins.body, skins.arms, skins.head]});
	AddAppearance(PLAYER_SKIN_SLOT_ARMR, new Skin_Definition { GraphicsDefinition = UA_Clonk_Skins, GraphicsName = "MSoldierArmR", Flags = AM_DrawBefore | AM_MatchSkeleton, MeshMaterial = [skins.body, skins.arms, skins.head]});
	AddAppearance(PLAYER_SKIN_SLOT_HEAD, new Skin_Definition { GraphicsDefinition = UA_Clonk_Skins, GraphicsName = "MSoldierHead", Flags = AM_DrawBefore | AM_MatchSkeleton, MeshMaterial = [skins.head]});

	return skin;
}


func GetSkinTextures()
{
	var prefix = Format("%s_%s%s", skin_gender, skin_name, skin_suffix);
	return {body = Format("%s_body", prefix),
            legs = Format("%s_legs", prefix),
            arms = Format("%s_arms", prefix),
            head = Format("%s_head", prefix),
           };
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// Death

// TODO: Remove - this is great for testing, though
func Splatter()
{
	DoSplatter(RemoveAppearance(PLAYER_SKIN_SLOT_BODY))->VertexSetupBody();
	DoSplatter(RemoveAppearance(PLAYER_SKIN_SLOT_LEGS))->VertexSetupLegs();
	DoSplatter(RemoveAppearance(PLAYER_SKIN_SLOT_ARML))->VertexSetupArmL();
	DoSplatter(RemoveAppearance(PLAYER_SKIN_SLOT_ARMR))->VertexSetupArmR();
	DoSplatter(RemoveAppearance(PLAYER_SKIN_SLOT_HEAD))->VertexSetupHead();
}


// TODO: Remove - this is great for testing, though
func DoSplatter(proplist skin)
{
	if (!skin) return;
	var corpse = CreateObject(UA_Clonk_Corpse, 0, 0, NO_OWNER);
	corpse.MeshTransformation = this.MeshTransformation;
	corpse->AddAppearance(0, skin);
	corpse->SetSpeed(RandomX(-1, 1) * 30, RandomX(-20, -60));
	corpse->SetRDir((-1 + 2 * Random(2)) * RandomX(5, 15));
	corpse->SetDir(GetDir());
	corpse->SetPhase(GetPhase());
	corpse->CopyAnimationPositionFrom(this);
	corpse->StartSplatter();
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



//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// Weapons

public func GetAmmoSource(id ammo)
{
	// redundant at the moment, but it should avoid problems in the future
	// when real ammo counts are implemented 
	if (IsInstaGibConfigured())
	{
		return AMMO_Source_Infinite;
	}
	else
	{
		return AMMO_Source_Local;
	}
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// Helper functions to play some sounds. They are encapsulated here in case sound names change.

public func PlaySoundConfirm()
{
	// not used at the moment
}


public func PlaySoundDecline()
{
	// not used at the moment
}

public func PlaySoundDoubt()
{
	// not used at the moment
}

public func PlaySoundHurt()
{
	// not used at the moment
}


public func PlaySoundTaunt()
{
	// not used at the moment
}


public func PlaySoundShock()
{
	// not used at the moment
}

public func PlaySoundScream()
{
	// not used at the moment
}

public func PlaySoundIdle()
{
	// not used at the moment
}

public func PlaySkinSound(string sound, ...)
{
	// not used until the sounds are implemented properly
	// Sound(Format("Clonk::Skin::%s::%s", GetSoundSkinName(), sound), ...);
}

