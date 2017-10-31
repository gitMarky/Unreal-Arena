#include Weapon_UT_Base

local Name = "$Name$";
local Description = "$Description$";

local laser_beam;

public func GetCarryTransform()
{
	return Trans_Mul(Trans_Rotate(60, 1, 0, 0), Trans_Rotate(90, 0, 0, 1));
}


local firemode_primary = 
{
	name = 				WEAPON_Firemode_Primary, // string - menu caption
	icon = 				nil, // id - menu icon
	condition = 		nil, // string - callback for a condition

	ammo_id = 			Ammo_Plasma,
	ammo_usage =		1,	// this many units of ammo
	ammo_rate =			1, // per this many shots fired

	delay_charge =      0,
	delay_recover = 	6, // time between consecutive shots
	delay_cooldown = 	20,
	delay_reload =		80, // time to reload, in frames

	mode = 			 WEAPON_FM_Auto,

	damage = 			10, 
	damage_type = 		DMG_Energy,	

	projectile_id = 	Projectile_Plasma,
	projectile_speed = 	140,
	projectile_range = PROJECTILE_Range_Infinite,
	projectile_distance = 12,
	projectile_offset_y = -3,
	projectile_number = 1,
	projectile_spread = {angle = 7, precision = 2}, // 3 - default inaccuracy of a single projectile
};

local firemode_secondary = 
{
	name = 				WEAPON_Firemode_Secondary,
	icon = 				nil, // id - menu icon
	condition = 		nil, // string - callback for a condition

	ammo_id = 			Ammo_Plasma,
	ammo_usage =		1,	// this many units of ammo
	ammo_rate =			8, // per this many shots fired

	delay_charge =      0,
	delay_recover = 	2, // time between consecutive shots
	delay_cooldown = 	20,
	delay_reload =		260, // time to reload, in frames

	mode = 			 WEAPON_FM_Auto,

	damage = 			1,
	damage_type = 		DMG_Energy,

	projectile_id = 	Projectile_PlasmaBeam,
	projectile_speed = 	210,
	projectile_range = 90,
	projectile_distance = 13,
	projectile_offset_y = -3,
	projectile_number = 1,
	projectile_spread = nil,
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
	if (firemode.name == WEAPON_Firemode_Primary)
	{
		Sound("Weapon::Classic::pulse-fire", nil, nil, nil, 1);
	}
	else
	{
		Sound("Weapon::Classic::pulse-bolt", nil, nil, nil, 1);
	}
}


public func OnFireProjectile(object user, object projectile, proplist firemode)
{
	if (firemode.name == WEAPON_Firemode_Primary)
	{
		projectile->Trail(1, 60);
	}
	else
	{
		projectile->HitScan();
		projectile->SetBeam(laser_beam);
	}
}

public func OnStartCooldown(object user, proplist firemode)
{	
	Sound("Weapon::Classic::pulse-fire", nil, nil, nil, -1);
	Sound("Weapon::Classic::pulse-bolt", nil, nil, nil, -1);
	Sound("Weapon::Classic::pulse-down");
	LaserStop();
}

public func FireEffect(object user, int angle, proplist firemode)
{
	var x = +Sin(angle, firemode.projectile_distance);
	var y = -Cos(angle, firemode.projectile_distance) + firemode.projectile_offset_y;

	if (firemode.name == WEAPON_Firemode_Primary)
	{
		EffectMuzzleFlash(user, x, y, angle, RandomX(12, 16) * 3, false, true, RGB(0,255,0));
	}
	else
	{
		LaserStart(x, y);
	}
}

private func LaserStart(int x, int y)
{
	var x_start = GetX() + x;
	var y_start = GetY() + y;
	if (!laser_beam)
		laser_beam = CreateObject(LaserEffect, x, y, NO_OWNER);
	else
		laser_beam->SetPosition(x_start, y_start);

	laser_beam->SetWidth(6)->SetLifetime(0)->Color(RGB(0, 255, 0))->Activate();	
}

private func LaserStop()
{
	if (laser_beam) laser_beam->RemoveObject();
}

public func AmmoOnPickup(){ return [Ammo_Plasma, 60]; }


public func Departure(object container)
{
	// necessary, so that the weapon stops sound effects, etc.
	// might be moved to the library
	StartCooldown(container, GetFiremode());

	_inherited(container);
}
