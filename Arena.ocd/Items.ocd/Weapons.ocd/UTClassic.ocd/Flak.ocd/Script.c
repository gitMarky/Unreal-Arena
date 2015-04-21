#include Library_Weapon

local Name = "$Name$";
local Description = "$Description$";
local Collectible = 1;


public func GetCarryMode(object user) {    if (is_selected) return CARRY_Hand; }
public func GetCarrySpecial(object user) { if (is_selected) return "pos_hand2"; }
public func GetCarryBone() { return "Base"; }
public func GetCarryTransform()
{
	return Trans_Mul(Trans_Rotate(90, 0, 1, 0), Trans_Rotate(-65,1,0,0));
}

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
	default = 
	{
		name = 				"default",
		icon = 				nil, // id - menu icon
		condition = 		nil, // string - callback for a condition
		
		ammo_id = 			nil,
		ammo_usage =		1, // int - this many units of ammo
		ammo_rate =			1, // int - per this many shots fired
	
		delay_charge  =     0, // int, frames - time that the button must be held before the shot is fired
		delay_recover = 	26, // int, frames - time between consecutive shots
		delay_cooldown =    0, // int, frames - time of cooldown after the last shot is fired
		delay_reload =		6, // int, frames - time to reload
	
		mode = 			 WEAPON_FM_Single,
	
		damage = 			8, 
		damage_type = 		nil,	
	
		projectile_id = 	Projectile_FlakShot,
		projectile_speed = 	[60, 90, 10],
		projectile_range = PROJECTILE_Range_Infinite,
		projectile_distance = 10,
		projectile_offset_y = -6,
		projectile_number = 10,
		projectile_spread = [2, 1], // default inaccuracy of a single projectile

		spread = [0, 1],			   // inaccuracy from prolonged firing
		
		burst = 0, // number of projectiles fired in a burst
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
		delay_recover = 	10,
	
		mode = 			 WEAPON_FM_Single,
	
		damage = 			15, 
		damage_type = 		nil,	
	
		projectile_id = 	Projectile_Bullet,
		projectile_speed = 	180,
		projectile_range = 600,
		projectile_distance = 10,
		projectile_offset_y = -6,

		spread = 3,
		spread_factor = 100,
	},
};

public func FireSound(object user, proplist firemode)
{
	Sound("flak-fire-reg", nil, nil, nil, nil, true);
	Sound("flak-cock", nil, nil, nil, nil, true);
}

public func FireEffect(object user, int angle, proplist firemode)
{
	// muzzle flash
	
	var x = +Sin(angle, firemode.projectile_distance);
	var y = -Cos(angle, firemode.projectile_distance) + firemode.projectile_offset_y;
	 
	
	EffectMuzzleFlash(user, x, y, angle, RandomX(10, 20), false, true);		
}
