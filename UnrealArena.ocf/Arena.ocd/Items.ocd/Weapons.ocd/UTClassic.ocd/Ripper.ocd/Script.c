#include Weapon_UT_Base

local Name = "$Name$";
local Description = "$Description$";

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
		name = 				"primary", // string - menu caption
		icon = 				nil, // id - menu icon
		condition = 		nil, // string - callback for a condition
		
		ammo_id = 			Ammo_Pistol,
		ammo_usage =		1,	// this many units of ammo
		ammo_rate =			1, // per this many shots fired
	
		delay_charge =      0,
		delay_recover = 	13, // time between consecutive shots
		delay_cooldown = 	0,
		delay_reload =		50, // time to reload, in frames
	
		mode = 			 WEAPON_FM_Auto,
	
		damage = 			30, 
		damage_type = 		nil,	
	
		projectile_id = 	Projectile_Ripper,
		projectile_speed = 	180,
		projectile_range = PROJECTILE_Range_Infinite,
		projectile_distance = 14,
		projectile_offset_y = -3,
		projectile_number = 1,
		projectile_spread = {angle = 3, precision = 2},

		sound = "ripper-fire-reg",
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
		delay_recover = 	24, // time between consecutive shots
		delay_cooldown = 	0,
		delay_reload =		50, // time to reload, in frames
	
		mode = 			 WEAPON_FM_Auto,
	
		damage = 			24, 
		damage_type = 		nil,	
	
		projectile_id = 	Projectile_Ripper,
		projectile_speed = 	180,
		projectile_range = PROJECTILE_Range_Infinite,
		projectile_distance = 14,
		projectile_offset_y = -3,
		projectile_number = 1,
		projectile_spread = {angle = 3, precision = 2},

		sound = "ripper-fire-alt",
	},
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
	Sound(firemode.sound);
}


public func OnFireProjectile(object user, object projectile, proplist firemode)
{
	if (firemode.name == fire_modes.secondary.name)
	{
		projectile->SetSecondary();
	}
}

public func OnStartCooldown(object user, proplist firemode)
{
}

public func FireEffect(object user, int angle, proplist firemode)
{
	//
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
