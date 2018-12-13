#include Weapon_UT_Base

local Name = "$Name$";
local Description = "$Description$";

public func GetCarryTransform()
{
	return Trans_Mul(Trans_Rotate(90, 1, 0, 0), Trans_Rotate(10, 0, 0, 1), Trans_Rotate(0, 0, 1, 0), Trans_Translate(-2000, 500, 0));
}


local firemode_primary	= 
{
	name = 				WEAPON_Firemode_Primary,
	icon = 				nil,
	condition = 		nil,

	ammo_id = 			Ammo_Pistol,
	ammo_usage =		1,
	ammo_rate =			1,

	delay_prior = 		0,
	delay_reload =		0,
	delay_recover = 	15,

	mode = 			 WEAPON_FM_Single,

	damage = 			15, 
	damage_type = 		DMG_Projectile,	

	projectile_id = 	Projectile_Bullet,
	projectile_speed = 	180,
	projectile_range = 600,
	projectile_distance = 10,
	projectile_offset_y = -6,
	projectile_spread = {angle = 7, precision = 4},

	ai_fire_ballistic = true,	// custom property for the bot
};

local firemode_secondary = 
{
	name = 				WEAPON_Firemode_Secondary,
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
	damage_type = 		DMG_Projectile,	

	projectile_id = 	Projectile_Bullet,
	projectile_speed = 	180,
	projectile_range = 600,
	projectile_distance = 10,
	projectile_offset_y = -6,

	projectile_spread = {angle = 4, precision = 1},

	ai_fire_ballistic = true,	// custom property for the bot
};

public func FireSound(object user, proplist firemode)
{
	Sound("Weapon::Classic::enforcer-fire");
}

public func OnFireProjectile(object user, object projectile, proplist firemode)
{
	projectile->Trail(2, 80);
}

public func FireEffect(object user, int angle, proplist firemode)
{
	// muzzle flash

	var x = +Sin(angle, firemode.projectile_distance);
	var y = -Cos(angle, firemode.projectile_distance) + firemode.projectile_offset_y;


	EffectMuzzleFlash(user, x, y, angle, 30, false, true);

	// casing

	x = +Sin(angle, firemode.projectile_distance / 2);
	y = -Cos(angle, firemode.projectile_distance / 2) + firemode.projectile_offset_y;

	CreateCartridgeEffect("Cartridge_Pistol", 2, x, y, user->GetXDir() + Sin(-angle, 10), user->GetYDir() - RandomX(12, 15));
}


// Ammo on pickup
public func AmmoID()       { return Ammo_Pistol; }
public func AmmoCount()    { return 30; }
