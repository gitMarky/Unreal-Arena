#include Library_Weapon
#include Plugin_Weapon_FiremodeByUse

local Name = "$Name$";
local Description = "$Description$";
local Collectible = 1;


public func GetCarryMode(object user) {    if (is_selected) return CARRY_Hand; }
public func GetCarrySpecial(object user) { if (is_selected) return "pos_hand2"; }
public func GetCarryBone() { return "Base"; }
public func GetCarryTransform()
{
	return Trans_Rotate(120, 0, 1, 0);
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
	primary = 
	{
		name = 				"primary", // string - menu caption
		icon = 				nil, // id - menu icon
		condition = 		nil, // string - callback for a condition
		
		ammo_id = 			Ammo_Pistol,
		ammo_usage =		1,	// this many units of ammo
		ammo_rate =			1, // per this many shots fired
	
		delay_charge =      0,
		delay_recover = 	6, // time between consecutive shots
		delay_cooldown = 	20,
		delay_reload =		80, // time to reload, in frames
	
		mode = 			 WEAPON_FM_Auto,
	
		damage = 			10, 
		damage_type = 		nil,	
	
		projectile_id = 	Projectile_Plasma,
		projectile_speed = 	140,
		projectile_range = PROJECTILE_Range_Infinite,
		projectile_distance = 12,
		projectile_offset_y = -3,
		projectile_number = 1,
		projectile_spread = [7, 2], // 3 - default inaccuracy of a single projectile

		spread = [0, 1],			   // inaccuracy from prolonged firing
	},
	
	secondary = 
	{
		name = 				"secondary",
		icon = 				nil, // id - menu icon
		condition = 		nil, // string - callback for a condition
		
		ammo_id = 			Ammo_Pistol,
		ammo_usage =		1,	// this many units of ammo
		ammo_rate =			1, // per this many shots fired
	
		delay_charge =      0,
		delay_recover = 	2, // time between consecutive shots
		delay_cooldown = 	20,
		delay_reload =		260, // time to reload, in frames
	
		mode = 			 WEAPON_FM_Auto,
	
		damage = 			6, 
		damage_type = 		nil,	
	
		projectile_id = 	Projectile_Bullet,
		projectile_speed = 	210,
		projectile_range = 600,
		projectile_distance = 12,
		projectile_offset_y = -3,
		projectile_number = 1,
		projectile_spread = [15, 2], // 6 - default inaccuracy of a single projectile

		spread = [0, 1],			   // inaccuracy from prolonged firing	},
	},
};

local weapon_properties = 
{
		gfx_distance = 16,
		gfx_offset_y = -2,
};

local animation_set = {
		AimMode        = AIM_Position,
		AnimationAim   = "CrossbowAimArms",
};

public func FireSound(object user, proplist firemode)
{
	if (firemode.name == fire_modes.primary.name)
	{
		Sound("pulse-fire", nil, nil, nil, 1);
	}
	else
	{
		Sound("pulse-bolt", nil, nil, nil, 1);
	}
}


public func OnFireProjectile(object user, object projectile, proplist firemode)
{
	projectile->Trail(1, 60);
}

public func OnStartCooldown(object user, proplist firemode)
{
	
	Sound("pulse-fire", nil, nil, nil, -1);
	Sound("pulse-bolt", nil, nil, nil, -1);
	Sound("pulse-down");
}

public func FireEffect(object user, int angle, proplist firemode)
{
	// muzzle flash
	
	var x = +Sin(angle, firemode.projectile_distance);
	var y = -Cos(angle, firemode.projectile_distance) + firemode.projectile_offset_y;

	EffectMuzzleFlash(user, x, y, angle, RandomX(12, 16), false, true, RGB(0,255,0));
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