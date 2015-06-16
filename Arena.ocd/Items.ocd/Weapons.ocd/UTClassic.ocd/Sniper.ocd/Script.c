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

local animation_set = {
		AimMode        = AIM_Position, // The aiming animation is done by adjusting the animation position to fit the angle
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
		name = 				"primary", // string - menu caption
		icon = 				nil, // id - menu icon
		condition = 		nil, // string - callback for a condition
		
		ammo_id = 			Ammo_Pistol,
		ammo_usage =		1,	// this many units of ammo
		ammo_rate =			1, // per this many shots fired
	
		delay_charge =      0,
		delay_recover = 	21, // time between consecutive shots
		delay_cooldown = 	0,
		delay_reload =		260, // time to reload, in frames
	
		mode = 			 	WEAPON_FM_Single,
	
		damage = 			45, 
		damage_type = 		nil,	
	
		projectile_id = 	Projectile_Bullet,
		projectile_speed = 	2000,
		projectile_range = 4000,
		projectile_distance = 18,
		projectile_offset_y = -2, // -4
		projectile_number = 1,
		projectile_spread = [3, 5],

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
		projectile_distance = 18,
		projectile_offset_y = -2, // -4
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

public func FireSound(object user, proplist firemode)
{
	Sound("sniper-fire");
}


public func OnFireProjectile(object user, object projectile, proplist firemode)
{
	// no effect
}
	
public func FireEffect(object user, int angle, proplist firemode)
{
	
	// muzzle flash
	
	var x = +Sin(angle, firemode.projectile_distance);
	var y = -Cos(angle, firemode.projectile_distance) + firemode.projectile_offset_y;

	EffectMuzzleFlash(user, x, y, angle, 10, false, true);
		
	// casing
	
	x = +Sin(angle, firemode.projectile_distance / 4);
	y = -Cos(angle, firemode.projectile_distance / 4) + firemode.projectile_offset_y;

	var dir = (user->GetDir() * 2) - 1;

	CreateCartridgeEffect("Cartridge_Pistol", 2, x, y, - dir * Cos(angle - 35 * dir, RandomX(30, 45)), - dir * Sin(angle - 35 * dir, RandomX(30, 45)));
}
