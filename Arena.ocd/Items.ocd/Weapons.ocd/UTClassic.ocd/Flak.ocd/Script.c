#include Library_Weapon
#include Plugin_Weapon_FiremodeByUse



local Name = "$Name$";
local Description = "$Description$";
local Collectible = 1;


public func GetCarryMode(object user) {    if (is_selected) return CARRY_Hand; }
public func GetCarrySpecial(object user) { if (is_selected) return "pos_hand2"; }
public func GetCarryBone() { return "Grip"; }
public func GetCarryTransform()
{
	return Trans_Rotate(-60, 0, 1, 0);
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
	primary = 
	{
		name = 				"primary",
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
		projectile_spread = [2, 1], // default inaccuracy of a single projectile

		spread = [0, 1],			   // inaccuracy from prolonged firing
		
		burst = 0, // number of projectiles fired in a burst
		
		sound = "flak-fire-reg",
	},
	
	secondary = 
	{
		name = 				"secondary",
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
	
		damage = 			15, 
		damage_type = 		nil,	
	
		projectile_id = 	Projectile_FlakGrenade,
		projectile_speed = 	[70, 90],
		projectile_range = PROJECTILE_Range_Infinite,
		projectile_distance = 10,
		projectile_offset_y = -3,
		projectile_number = 1,
		projectile_spread = [5, 2],

		spread = [0, 1],
		
		burst = 0,
		
		sound = "flak-fire-alt",
	},
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
