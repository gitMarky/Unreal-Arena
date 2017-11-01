#include Weapon_UT_Base

local Name = "$Name$";
local Description = "$Description$";

local slime_charged = 0;

public func GetCarryTransform()
{
	return Trans_Mul(Trans_Rotate(60, 1, 0, 0), Trans_Rotate(90, 0, 0, 1));
}

local animation_set = {
		AimMode        = AIM_Position,
		AnimationAim   = "CrossbowAimArms",
};

local firemode_primary	= 
{
	name = 				WEAPON_Firemode_Primary,
	icon = 				nil,
	condition = 		nil,
	
	ammo_id = 			Ammo_Bio,
	ammo_usage =		1,
	ammo_rate =			1,

	delay_charge  =     0, // int, frames - time that the button must be held before the shot is fired
	delay_recover = 	10, // int, frames - time between consecutive shots
	delay_cooldown =    0, // int, frames - time of cooldown after the last shot is fired
	delay_reload =		0, // int, frames - time to reload

	mode = 			 	WEAPON_FM_Auto,

	damage = 			40, 
	damage_type = 		DMG_Bio,	

	projectile_id = 	Projectile_SlimeShot,
	projectile_speed = 	70,
	projectile_range = PROJECTILE_Range_Infinite,
	projectile_distance = 10,
	projectile_offset_y = -4,
	projectile_spread = {angle = 3, precision = 2},
	
	ai_fire_ballistic = true,	// custom property for the bot
};

local firemode_secondary = 
{
	name = 				WEAPON_Firemode_Secondary,
	icon = 				nil,
	condition = 		nil,

	ammo_id = 			Ammo_Bio,
	ammo_usage =		1,
	ammo_rate =			1,

	delay_charge  =     160, // int, frames - time that the button must be held before the shot is fired
	delay_recover = 	40, // int, frames - time between consecutive shots
	delay_cooldown =    0, // int, frames - time of cooldown after the last shot is fired
	delay_reload =		0, // int, frames - time to reload

	mode = 			 	WEAPON_FM_Single,

	damage = 			40, 
	damage_type = 		DMG_Bio,	

	projectile_id = 	Projectile_SlimeShot,
	projectile_speed = 	70,
	projectile_range = 	PROJECTILE_Range_Infinite,
	projectile_distance = 10,
	projectile_offset_y = -4,
	projectile_spread = {angle = 3, precision = 2},
	
	ai_fire_ballistic = true,	// custom property for the bot
};

public func FireSound(object user, proplist firemode)
{
	Sound("Weapon::Classic::bio-fire");
}

public func OnFireProjectile(object user, object projectile, proplist firemode)
{
	projectile->Charge(slime_charged);
	slime_charged = 0;
}

public func FireEffect(object user, int angle, proplist firemode)
{
	// no effect
}

/**
 Callback: the weapon has successfully charged. Does nothing by default.
 @par user The object that is using the weapon.
 @par firemode A proplist containing the fire mode information.
 @version 0.1.0
 */
public func OnFinishCharge(object user, int x, int y, proplist firemode)
{
	if (firemode.name == WEAPON_Firemode_Primary) return;
	ShootBigBlob(user, x, y);
}

/**
 Callback: the weapon user cancelled charging. Does nothing by default.
 @par user The object that is using the weapon.
 @par firemode A proplist containing the fire mode information.
 @version 0.1.0
 */
public func OnCancelCharge(object user, int x, int y, proplist firemode)
{
	if (firemode.name == WEAPON_Firemode_Primary) return;
	ShootBigBlob(user, x, y, firemode);
}

private func ShootBigBlob(object user, int x, int y, proplist firemode)
{
	slime_charged = GetChargeProgress();
	Fire(user, x, y);
	CancelCharge(user, x, y, firemode, false);
}

private func Fire(object user, int x, int y)
{
	if (GetFiremode() == firemode_secondary)
	{
		if (slime_charged > 0) _inherited(user, x, y);
	}
	else
	{
		_inherited(user, x, y);
	}
}

// Ammo on pickup
public func AmmoID()       { return Ammo_Bio; }
public func AmmoCount()    { return 25; }
