#include Weapon_UT_Base

local Name = "$Name$";
local Description = "$Description$";

local slime_charged = 0;

public func GetCarryTransform()
{
	return Trans_Rotate(120, -1, 0, 0);
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
	primary	= 
	{
		name = 				"primary",
		icon = 				nil,
		condition = 		nil,
		
		ammo_id = 			Ammo_Pistol,
		ammo_usage =		1,
		ammo_rate =			1,
	
		delay_charge  =     0, // int, frames - time that the button must be held before the shot is fired
		delay_recover = 	10, // int, frames - time between consecutive shots
		delay_cooldown =    0, // int, frames - time of cooldown after the last shot is fired
		delay_reload =		0, // int, frames - time to reload
	
		mode = 			 	WEAPON_FM_Auto,
	
		damage = 			40, 
		damage_type = 		nil,	
	
		projectile_id = 	Projectile_SlimeShot,
		projectile_speed = 	70,
		projectile_range = PROJECTILE_Range_Infinite,
		projectile_distance = 10,
		projectile_offset_y = -4,
		projectile_spread = {angle = 3, precision = 2},
	},
	
	secondary = 
	{
		name = 				"secondary",
		icon = 				nil,
		condition = 		nil,

		ammo_id = 			Ammo_Pistol,
		ammo_usage =		1,
		ammo_rate =			1,

		delay_charge  =     160, // int, frames - time that the button must be held before the shot is fired
		delay_recover = 	40, // int, frames - time between consecutive shots
		delay_cooldown =    0, // int, frames - time of cooldown after the last shot is fired
		delay_reload =		0, // int, frames - time to reload

		mode = 			 	WEAPON_FM_Single,

		damage = 			40, 
		damage_type = 		nil,	

		projectile_id = 	Projectile_SlimeShot,
		projectile_speed = 	70,
		projectile_range = 	PROJECTILE_Range_Infinite,
		projectile_distance = 10,
		projectile_offset_y = -4,
		projectile_spread = {angle = 3, precision = 2},
	},
};

public func FireSound(object user, proplist firemode)
{
	Sound("bio-fire");
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
	if (firemode.name == fire_modes.primary.name) return;
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
	if (firemode.name == fire_modes.primary.name) return;
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
	if (GetFiremode() == fire_modes.secondary)
	{
		if (slime_charged > 0) _inherited(user, x, y);
	}
	else
	{
		_inherited(user, x, y);
	}
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
