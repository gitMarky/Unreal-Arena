#include Weapon_UT_Base

local Name = "$Name$";
local Description = "$Description$";

public func GetCarryTransform()
{
	return Trans_Rotate(120, -1, 0, 0);
}

local animation_set = {
		AimMode        = AIM_Position,
		AnimationAim   = "CrossbowAimArms",
};


local firemode_primary = 
{
	name = 				WEAPON_Firemode_Primary,
	icon = 				nil, // id - menu icon
	condition = 		nil, // string - callback for a condition
	
	ammo_id = 			Ammo_Redeemer,
	ammo_usage =		1, // int - this many units of ammo
	ammo_rate =			1, // int - per this many shots fired

	delay_charge  =     0, // int, frames - time that the button must be held before the shot is fired
	delay_recover = 	32, // int, frames - time between consecutive shots
	delay_cooldown =    0, // int, frames - time of cooldown after the last shot is fired
	delay_reload =		0, // int, frames - time to reload

	mode = 			 WEAPON_FM_Single,

	damage = 			0, 
	damage_type = 		nil,	

	projectile_id = 	Projectile_Rocket,
	projectile_speed = 	120,
	projectile_range = PROJECTILE_Range_Infinite,
	projectile_distance = 10,
	projectile_offset_y = -6,
	projectile_number = 1,
	projectile_spread = nil, // default inaccuracy of a single projectile

	burst = 0, // number of projectiles fired in a burst
	
	sound = "Weapon::Classic::rocket-fire-reg",
};

local firemode_secondary = 
{
	name = 				WEAPON_Firemode_Secondary,
	icon = 				nil,
	condition = 		nil,
	
	ammo_id = 			Ammo_Redeemer,
	ammo_usage =		1,
	ammo_rate =			1,

	delay_charge  =     0, // int, frames - time that the button must be held before the shot is fired
	delay_recover = 	32, // int, frames - time between consecutive shots
	delay_cooldown =    0, // int, frames - time of cooldown after the last shot is fired
	delay_reload =		0, // int, frames - time to reload

	mode = 			 WEAPON_FM_Single,

	damage = 			15, 
	damage_type = 		nil,	

	projectile_id = 	Projectile_Rocket,
	projectile_speed = 	[60, 80, 5],
	projectile_range = PROJECTILE_Range_Infinite,
	projectile_distance = 10,
	projectile_offset_y = -6,
	projectile_number = 1,
	projectile_spread = [7, 2],

	spread = [0, 1],
	
	burst = 0,
	
	sound = "Weapon::Classic::rocket-fire-alt",
};

public func FireSound(object user, proplist firemode)
{
	Sound(firemode.sound, nil, nil, nil, nil, true);
}

public func FireEffect(object user, int angle, proplist firemode)
{
	// no effects!
}

public func OnFireProjectile(object user, object projectile, proplist firemode)
{
	if (firemode.name == WEAPON_Firemode_Secondary)
	{
		projectile->Grenade();
	}
}

public func AmmoOnPickup(){ return [Ammo_Redeemer, 1]; }

