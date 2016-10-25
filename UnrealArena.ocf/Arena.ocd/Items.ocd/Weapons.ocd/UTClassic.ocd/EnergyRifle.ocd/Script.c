#include Weapon_UT_Base

local Name = "$Name$";
local Description = "$Description$";

static const PROJECTILE_Range_EnergyRifle = 10000;
static const PROJECTILE_Color_EnergyRifle = -11534081; // RGB(80, 0, 255);

public func GetCarryTransform()
{
	return Trans_Mul(Trans_Rotate(-60, -1, 0, 0), Trans_Translate(1000, -2000, 0), Trans_Rotate(90, 0, 0, 1));
}

local animation_set = {
		AimMode        = AIM_Position,
		AnimationAim   = "CrossbowAimArms",
};


local firemode_primary	= 
{
	name = 				WEAPON_Firemode_Primary,
	icon = 				nil,
	condition = 		nil,

	ammo_id = 			Ammo_Energy,
	ammo_usage =		1,
	ammo_rate =			1,

	delay_prior = 		0,
	delay_reload =		0,
	delay_recover = 	26,

	mode = 			 	WEAPON_FM_Single,

	damage = 			40, 
	damage_type = 		DMG_Energy,	

	projectile_id = 		Projectile_ShockBeam,
	projectile_speed = 		500,
	projectile_range = 		PROJECTILE_Range_Sniper,
	projectile_distance = 	16,
	projectile_offset_y = 	-3,
	projectile_spread = 	nil,

	sound = "Weapon::Classic::shock-fire-reg",
};
	
local firemode_secondary = 
{
	name = 				WEAPON_Firemode_Secondary,
	icon = 				nil,
	condition = 		nil,
	
	ammo_id = 			Ammo_Energy,
	ammo_usage =		1,
	ammo_rate =			1,

	delay_prior = 		0,
	delay_reload =		0,
	delay_recover = 	24,

	mode = 			 	WEAPON_FM_Single,

	damage = 			50, 
	damage_type = 		DMG_Energy,	

	projectile_id = 		Projectile_ShockBolt,
	projectile_speed = 		100,
	projectile_range = 		PROJECTILE_Range_Infinite,
	projectile_distance = 	16,
	projectile_offset_y = 	-3,

	projectile_spread = {angle = 5, precision = 2},

	sound = "Weapon::Classic::shock-fire-alt",
};

public func Initialize()
{
	_inherited(...);
	
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
	if (firemode.name == WEAPON_Firemode_Primary)
	{
		projectile->HitScan();
	}

	if (IsInstaGibConfigured())
	{
		projectile->DamageAmount(PROJECTILE_Damage_InstaGib);
	}
}

public func ChangeFiremode(firemode)
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

public func AmmoOnPickup(){ return [Ammo_Energy, 20]; }
