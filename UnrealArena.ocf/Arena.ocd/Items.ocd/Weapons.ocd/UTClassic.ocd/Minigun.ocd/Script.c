#include Weapon_UT_Base

local Name = "$Name$";
local Description = "$Description$";

public func GetCarryTransform()
{
	return Trans_Mul(Trans_Rotate(240, 1, 0, 0), Trans_Rotate(180, 0, 1, 0), Trans_Rotate(-5, 0, 0, 1), Trans_Translate(-1000, -2000));
}


local firemode_primary = 
{
	name = 				WEAPON_Firemode_Primary, // string - menu caption
	icon = 				nil, // id - menu icon
	condition = 		nil, // string - callback for a condition
	
	ammo_id = 			Ammo_Pistol,
	ammo_usage =		1,	// this many units of ammo
	ammo_rate =			1, // per this many shots fired

	delay_charge =      0,
	delay_recover = 	4, // time between consecutive shots
	delay_cooldown = 	20,
	delay_reload =		260, // time to reload, in frames

	mode = 			 WEAPON_FM_Auto,

	damage = 			6, 
	damage_type = 		DMG_Projectile,	

	projectile_id = 	Projectile_Bullet,
	projectile_speed = 	210,
	projectile_range = 600,
	projectile_distance = 18,
	projectile_offset_y = -2, // -4
	projectile_number = 1,
	projectile_spread = {angle = 5, precision = 2}, // 2 - default inaccuracy of a single projectile
};
	
local firemode_secondary = 
{
	name = 				WEAPON_Firemode_Secondary,
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
	damage_type = 		DMG_Projectile,	

	projectile_id = 	Projectile_Bullet,
	projectile_speed = 	210,
	projectile_range = 600,
	projectile_distance = 18,
	projectile_offset_y = -2, // -4
	projectile_number = 1,
	projectile_spread = {angle = 15, precision = 2}, // 6 - default inaccuracy of a single projectile
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
	if (firemode.name == WEAPON_Firemode_Primary)
	{
		Sound("Weapon::Classic::m1-fire-reg", nil, nil, nil, 1);
	}
	else
	{
		Sound("Weapon::Classic::m1-fire-alt", nil, nil, nil, 1);
	}
}


public func OnFireProjectile(object user, object projectile, proplist firemode)
{
	projectile->Trail(1, 60);
}

public func OnStartCooldown(object user, proplist firemode)
{
	
	Sound("Weapon::Classic::m1-fire-reg", nil, nil, nil, -1);
	Sound("Weapon::Classic::m1-fire-alt", nil, nil, nil, -1);
	Sound("Weapon::Classic::m1-cooldown");
}

public func FireEffect(object user, int angle, proplist firemode)
{
	
	// muzzle flash
	
	var x = +Sin(angle, firemode.projectile_distance);
	var y = -Cos(angle, firemode.projectile_distance) + firemode.projectile_offset_y;

	EffectMuzzleFlash(user, x, y, angle, RandomX(15, 18) * 3, false, true, -1, "MuzzleFlash2");
		
	// casing
	
	x = +Sin(angle, firemode.projectile_distance / 2);
	y = -Cos(angle, firemode.projectile_distance / 2) + firemode.projectile_offset_y;

	var dir = (user->GetDir() * 2) - 1;

	CreateCartridgeEffect("Cartridge_Pistol", 4, x, y, - dir * Cos(angle - 35 * dir, RandomX(30, 45)), - dir * Sin(angle - 35 * dir, RandomX(30, 45)));
}

// Ammo on pickup
public func AmmoID()       { return Ammo_Pistol; }
public func AmmoCount()    { return 80; }

