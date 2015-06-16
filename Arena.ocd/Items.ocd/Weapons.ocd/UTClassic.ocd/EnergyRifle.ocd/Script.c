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
	return Trans_Mul(Trans_Rotate(-60,0,1,0), Trans_Translate(0, 1000, 0));
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
		delay_recover = 	26,
	
		mode = 			 WEAPON_FM_Single,
	
		damage = 			40, 
		damage_type = 		nil,	
	
		projectile_id = 	Projectile_ShockBeam,
		projectile_speed = 	500,
		projectile_range = PROJECTILE_Range_Infinite,
		projectile_distance = 15,
		projectile_offset_y = -3,
		projectile_spread = [0, 1],

		spread = [0, 1],
		
		sound = "shock-fire-reg",
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
		delay_recover = 	24,
	
		mode = 			 WEAPON_FM_Single,
	
		damage = 			40, 
		damage_type = 		nil,	
	
		projectile_id = 	Projectile_ShockBolt,
		projectile_speed = 	150,
		projectile_range = PROJECTILE_Range_Infinite,
		projectile_distance = 15,
		projectile_offset_y = -3,
		
		projectile_spread = [5, 2],

		spread = [0, 1],
		
		sound = "shock-fire-alt",
	},
};

public func FireSound(object user, proplist firemode)
{
	Sound(firemode.sound);
}

public func OnFireProjectile(object user, object projectile, proplist firemode)
{
	projectile->Trail(5, 350);
}

public func FireEffect(object user, int angle, proplist firemode)
{
	// muzzle flash
	
	var x = +Sin(angle, firemode.projectile_distance);
	var y = -Cos(angle, firemode.projectile_distance) + firemode.projectile_offset_y;
	 
	
	EffectMuzzleFlash(user, x, y, angle, 10, false, true);
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
