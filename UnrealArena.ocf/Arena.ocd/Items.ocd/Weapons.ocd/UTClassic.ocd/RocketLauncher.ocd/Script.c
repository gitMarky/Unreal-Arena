#include Weapon_UT_Base

local Name = "$Name$";
local Description = "$Description$";

static const WEAPON_UT99_Rocket_Max_Projectiles = 6;

public func GetCarryTransform()
{
	return Trans_Rotate(120, -1, 0, 0);
}

local animation_set = {
		AimMode        = AIM_Position,
		AnimationAim   = "CrossbowAimArms",
};


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
	
		delay_charge  =     32, // int, frames - time that the button must be held before the shot is fired
		delay_recover = 	32, // int, frames - time between consecutive shots
		delay_cooldown =    0, // int, frames - time of cooldown after the last shot is fired
		delay_reload =		0, // int, frames - time to reload
	
		mode = 			 	WEAPON_FM_Single,
	
		damage = 			140, 
		damage_type = 		nil,	
	
		projectile_id = 	Projectile_Rocket,
		projectile_speed = 	120,
		projectile_range = PROJECTILE_Range_Infinite,
		projectile_distance = 12,
		projectile_offset_y = -4,
		projectile_number = 1,
		projectile_spread = nil, // default inaccuracy of a single projectile

		burst = 0, // number of projectiles fired in a burst
		
		sound = "rocket-fire-reg",
	},
	
	secondary = 
	{
		name = 				"secondary",
		icon = 				nil,
		condition = 		nil,
		
		ammo_id = 			nil,
		ammo_usage =		1,
		ammo_rate =			1,
	
		delay_charge  =     32, // int, frames - time that the button must be held before the shot is fired
		delay_recover = 	32, // int, frames - time between consecutive shots
		delay_cooldown =    0, // int, frames - time of cooldown after the last shot is fired
		delay_reload =		0, // int, frames - time to reload
	
		mode = 			 	WEAPON_FM_Single,
	
		damage = 			140, 
		damage_type = 		nil,	
	
		projectile_id = 		Projectile_Rocket,
		projectile_speed = 		[60, 80, 5],
		projectile_range = 		PROJECTILE_Range_Infinite,
		projectile_distance = 	12,
		projectile_offset_y = 	-4,
		projectile_number = 	1,
		projectile_spread = 	{angle = 7, precision = 2},

		burst = 0,
		
		sound = "rocket-fire-alt",
	},
};

local projectiles_loaded;

private func Initialize()
{
	projectiles_loaded = {};
	ClearProjectiles();
	_inherited();
}

private func LoadProjectiles(proplist firemode)
{
	projectiles_loaded[firemode.name] = BoundBy(projectiles_loaded[firemode.name] + 1, 0, WEAPON_UT99_Rocket_Max_Projectiles);
}

private func ClearProjectiles()
{
	projectiles_loaded["primary"] = 0;
	projectiles_loaded["secondary"] = 0;
}

private func GetProjectiles(proplist firemode)
{
	return projectiles_loaded[firemode.name];
}

private func FireProjectiles(object user, int angle, proplist firemode)
{
	_inherited(user, angle, firemode);
	ClearProjectiles();
}

public func FireSound(object user, proplist firemode)
{
	Sound(firemode.sound);
}

public func FireEffect(object user, int angle, proplist firemode)
{
	// no effects!
}


private func DoCharge(object user, int x, int y, proplist firemode)
{
	_inherited(user, x, y, firemode);
	return GetProjectiles(firemode) == WEAPON_UT99_Rocket_Max_Projectiles;
}

public func OnFinishCharge(object user, int x, int y, proplist firemode)
{
	if (GetProjectiles(firemode) < WEAPON_UT99_Rocket_Max_Projectiles)
	{
		Sound("rocket-load");
		LoadProjectiles(firemode);
		CancelCharge(user, x, y, firemode, false);
	}
}

public func OnCancelCharge(object user, int x, int y, proplist firemode)
{
	Fire(user, x, y);
}

public func OnFireProjectile(object user, object projectile, proplist firemode)
{
	if (firemode.name == fire_modes.secondary.name)
	{
		projectile->Grenade();
	}
}

private func GetSpread(proplist firemode)
{
	var spread = _inherited(firemode);
	var additional = Projectile_Deviation(GetProjectiles(firemode));
	return NormalizeDeviations([spread, additional]);
}
