#include Library_Weapon
#include Plugin_Weapon_FiremodeByUse

local Name = "$Name$";
local Description = "$Description$";
local Collectible = 1;

static const PROJECTILE_Range_EnergyRifle = 10000;
static const PROJECTILE_Color_EnergyRifle = -11534081; // RGB(80, 0, 255);

public func GetCarryMode(object user) {    if (is_selected) return CARRY_Hand; }
public func GetCarrySpecial(object user) { if (is_selected) return "pos_hand2"; }
public func GetCarryBone() { return "Base"; }
public func GetCarryTransform()
{
	return Trans_Mul(Trans_Rotate(-60, 0, 1, 0), Trans_Translate(0, 1000, 0));
}

local animation_set = {
		AimMode        = AIM_Position,
		AnimationAim   = "CrossbowAimArms",
};

func Hit()
{
	Sound("GeneralHit?");
}

public func GetAmmoSource(id ammo)
{
	return AMMO_Source_Container;
}

public func GetAmmoContainer()
{
	return Contained();
}

local fire_modes =
{
	primary	= 
	{
		name = 				"primary",
		icon = 				nil,
		condition = 		nil,
		
		ammo_id = 			Ammo_Pistol,
		ammo_usage =		1,
		ammo_rate =			1,
	
		delay_prior = 		0,
		delay_reload =		0,
		delay_recover = 	26,
	
		mode = 			 	WEAPON_FM_Single,
	
		damage = 			40, 
		damage_type = 		nil,	
	
		projectile_id = 		Projectile_ShockBeam,
		projectile_speed = 		500,
		projectile_range = 		PROJECTILE_Range_Sniper,
		projectile_distance = 	16,
		projectile_offset_y = 	-3,
		projectile_spread = 	[0, 1],

		spread = [0, 1],
		
		sound = "shock-fire-reg",
	},
	
	secondary = 
	{
		name = 				"secondary",
		icon = 				nil,
		condition = 		nil,
		
		ammo_id = 			Ammo_Pistol,
		ammo_usage =		1,
		ammo_rate =			1,
	
		delay_prior = 		0,
		delay_reload =		0,
		delay_recover = 	24,
	
		mode = 			 	WEAPON_FM_Single,
	
		damage = 			40, 
		damage_type = 		nil,	
	
		projectile_id = 		Projectile_ShockBolt,
		projectile_speed = 		100,
		projectile_range = 		PROJECTILE_Range_Infinite,
		projectile_distance = 	16,
		projectile_offset_y = 	-3,
		
		projectile_spread = [5, 2],

		spread = [0, 1],
		
		sound = "shock-fire-alt",
	},
};

public func Initialize()
{
	_inherited();
	
	if (IsInstaGibConfigured())
	{
		SetMeshMaterial("weapon_ut99_instagib", 0);
	}
}

public func FireSound(object user, proplist firemode)
{
	Sound(firemode.sound);
}

public func OnFireProjectile(object user, object projectile, proplist firemode)
{
	if (firemode.name == fire_modes.primary.name)
	{
		projectile->HitScan();
	}

	if (IsInstaGibConfigured())
	{
		projectile->DamageAmount(PROJECTILE_Damage_InstaGib);
	}
}

public func ChangeFiremode(string firemode)
{
	// InstaGib supports only the primary firemode
	if (IsInstaGibConfigured())
	{
		selected_firemode = WEAPON_Firemode_Primary;
	}
	else
	{
		_inherited(firemode);
	}
}

local ActMap = {
	Fire = {
		Prototype = Action,
		Name = "Fire",
		Procedure = DFA_NONE,
		Length = 10,
		Delay = 1,
		NextAction = "Idle",
		Animation = "Fire",
	},
};