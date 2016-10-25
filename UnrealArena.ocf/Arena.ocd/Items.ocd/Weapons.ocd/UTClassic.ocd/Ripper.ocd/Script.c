#include Weapon_UT_Base

local Name = "$Name$";
local Description = "$Description$";

public func GetCarryTransform()
{
	return Trans_Mul(Trans_Rotate(60, 1, 0, 0), Trans_Rotate(90, 0, 0, 1));
}

local animation_set = {
		AimMode        = AIM_Position,
		AnimationAim   = "CrossbowAimArms",
};


local firemode_primary = 
{
	name = 				WEAPON_Firemode_Primary, // string - menu caption
	icon = 				nil, // id - menu icon
	condition = 		nil, // string - callback for a condition
	
	ammo_id = 			Ammo_Ripper,
	ammo_usage =		1,	// this many units of ammo
	ammo_rate =			1, // per this many shots fired

	delay_charge =      0,
	delay_recover = 	13, // time between consecutive shots
	delay_cooldown = 	0,
	delay_reload =		50, // time to reload, in frames

	mode = 			 WEAPON_FM_Auto,

	damage = 			30, 
	damage_type = 		DMG_Projectile|DMG_Headshot,	

	projectile_id = 	Projectile_Ripper,
	projectile_speed = 	180,
	projectile_range = PROJECTILE_Range_Infinite,
	projectile_distance = 14,
	projectile_offset_y = -3,
	projectile_number = 1,
	projectile_spread = {angle = 3, precision = 2},

	sound = "Weapon::Classic::ripper-fire-reg",
};

local firemode_secondary = 
{
	name = 				WEAPON_Firemode_Secondary,
	icon = 				nil, // id - menu icon
	condition = 		nil, // string - callback for a condition
	
	ammo_id = 			Ammo_Ripper,
	ammo_usage =		1,	// this many units of ammo
	ammo_rate =			1, // per this many shots fired

	delay_charge =      0,
	delay_recover = 	24, // time between consecutive shots
	delay_cooldown = 	0,
	delay_reload =		50, // time to reload, in frames

	mode = 			 WEAPON_FM_Auto,

	damage = 			24, 
	damage_type = 		DMG_Explosion,	

	projectile_id = 	Projectile_Ripper,
	projectile_speed = 	180,
	projectile_range = PROJECTILE_Range_Infinite,
	projectile_distance = 14,
	projectile_offset_y = -3,
	projectile_number = 1,
	projectile_spread = {angle = 3, precision = 2},

	sound = "Weapon::Classic::ripper-fire-alt",
};

local weapon_properties = 
{
		gfx_distance = 16,
		gfx_offset_y = -2,
};

local animation_set = {
		AimMode        = AIM_Position, // The aiming animation is done by adjusting the animation position to fit the angle
		AnimationAim   = "CrossbowAimArms",
	};

public func FireSound(object user, proplist firemode)
{
	Sound(firemode.sound);
}


public func OnFireProjectile(object user, object projectile, proplist firemode)
{
	if (firemode.name == WEAPON_Firemode_Secondary)
	{
		projectile->SetSecondary();
	}
}

public func OnStartCooldown(object user, proplist firemode)
{
}

public func FireEffect(object user, int angle, proplist firemode)
{
	//
}

public func AmmoOnPickup(){ return [Ammo_Ripper, 15]; }
