#include Weapon_UT_Base

local Name = "$Name$";
local Description = "$Description$";

public func GetCarryBone() { return "Grip"; }
public func GetCarryTransform()
{
	return Trans_Rotate(-60, -1, 0, 0);
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
	
	ammo_id = 			nil,
	ammo_usage =		1, // int - this many units of ammo
	ammo_rate =			1, // int - per this many shots fired

	delay_charge  =     0, // int, frames - time that the button must be held before the shot is fired
	delay_recover = 	26, // int, frames - time between consecutive shots
	delay_cooldown =    0, // int, frames - time of cooldown after the last shot is fired
	delay_reload =		0, // int, frames - time to reload

	mode = 			 WEAPON_FM_Single,

	damage = 			8, 
	damage_type = 		nil,	

	projectile_id = 	Projectile_FlakShot,
	projectile_speed = 	[60, 90, 10],
	projectile_range = PROJECTILE_Range_Infinite,
	projectile_distance = 12,
	projectile_offset_y = -3,
	projectile_number = 10,
	projectile_spread = {angle = 2, precision = 1}, // default inaccuracy of a single projectile

	burst = 0, // number of projectiles fired in a burst
	
	sound = "flak-fire-reg",
};

local firemode_secondary = 
{
	name = 				WEAPON_Firemode_Secondary,
	icon = 				nil,
	condition = 		nil,
	
	ammo_id = 			nil,
	ammo_usage =		1,
	ammo_rate =			1,

	delay_charge  =     0, // int, frames - time that the button must be held before the shot is fired
	delay_recover = 	36, // int, frames - time between consecutive shots
	delay_cooldown =    0, // int, frames - time of cooldown after the last shot is fired
	delay_reload =		0, // int, frames - time to reload

	mode = 			 WEAPON_FM_Single,

	damage = 			64, 
	damage_type = 		nil,	

	projectile_id = 	Projectile_FlakGrenade,
	projectile_speed = 	[70, 90],
	projectile_range = PROJECTILE_Range_Infinite,
	projectile_distance = 10,
	projectile_offset_y = -3,
	projectile_number = 1,
	projectile_spread = {angle = 5, precision = 2},

	burst = 0,
	
	sound = "flak-fire-alt",
};

public func FireSound(object user, proplist firemode)
{
	Sound(firemode.sound);
	Sound("flak-cock");
}

public func FireEffect(object user, int angle, proplist firemode)
{
	// muzzle flash
	
	var x = +Sin(angle, firemode.projectile_distance);
	var y = -Cos(angle, firemode.projectile_distance) + firemode.projectile_offset_y;
	 
	
	EffectMuzzleFlash(user, x, y, angle, RandomX(10, 20), false, true);		
}
