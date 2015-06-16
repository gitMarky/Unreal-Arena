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
	
		delay_prior = 		0,
		delay_reload =		0,
		delay_recover = 	10,
	
		mode = 			 WEAPON_FM_Auto,
	
		damage = 			40, 
		damage_type = 		nil,	
	
		projectile_id = 	Projectile_SlimeShot,
		projectile_speed = 	180,
		projectile_range = 600,
		projectile_distance = 10,
		projectile_offset_y = -4,
		projectile_spread = [3, 2],

		spread = [0, 1],
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
		projectile_offset_y = -4,
		
		projectile_spread = [4, 1],

		spread = [0, 1], 
	},
};

public func FireSound(object user, proplist firemode)
{
	Sound("bio-fire");
}

public func OnFireProjectile(object user, object projectile, proplist firemode)
{
	// no effect
}

public func FireEffect(object user, int angle, proplist firemode)
{
	// no effect
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
