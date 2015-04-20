#include Library_Weapon

local Name = "$Name$";
local Description = "$Description$";
local Collectible = 1;


public func GetCarryMode(object user) {    if (is_selected) return CARRY_Hand; }
public func GetCarrySpecial(object user) { if (is_selected) return "pos_hand2"; }
public func GetCarryBone() { return "Base"; }
public func GetCarryTransform()
{
	return Trans_Mul(Trans_Rotate(90, 0, 1, 0), Trans_Rotate(-65,1,0,0));
}

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
	default = 
	{
		name = 				"default",
		icon = 				nil, // id - menu icon
		condition = 		nil, // string - callback for a condition
		
		ammo_id = 			nil,
		ammo_usage =		1, // int - this many units of ammo
		ammo_rate =			1, // int - per this many shots fired
	
		delay_charge  =     0, // int, frames - time that the button must be held before the shot is fired
		delay_recover = 	26, // int, frames - time between consecutive shots
		delay_cooldown =    0, // int, frames - time of cooldown after the last shot is fired
		delay_reload =		6, // int, frames - time to reload
	
		mode = 			 WEAPON_FM_Single,
	
		damage = 			8, 
		damage_type = 		nil,	
	
		projectile_id = 	Projectile_FlakShot,
		projectile_speed = 	60,
		projectile_speed_rnd = 70,
		projectile_range = 600,
		projectile_distance = 10,
		projectile_offset_y = -6,
		projectile_number = 10,
		projectile_spread = 0, // default inaccuracy of a single projectile
		projectile_spread_factor = 100, // factor

		spread = 1,			   // inaccuracy from prolonged firing
		spread_factor = 100,   // factor
		
		burst = 0, // number of projectiles fired in a burst
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
		projectile_offset_y = -6,

		spread = 3,
		spread_factor = 100,
	},
};

public func FireSound(object user, proplist firemode)
{
	Sound("flak-fire-reg", nil, nil, nil, nil, true);
	Sound("flak-cock", nil, nil, nil, nil, true);
}

public func OnFireProjectile(object user, object projectile, proplist firemode)
{
	projectile->Trail(2, 80);
}

public func FireEffect(object user, int angle, proplist firemode)
{
	// this does nothing at the moment, 
	// probably because the animation is too small to be noticeable on the attached mesh :(
	
	//PlayAnimation("Fire", 1, Anim_Linear(0, 0, GetAnimationLength("Fire"), 10, ANIM_Remove), Anim_Const(1000));
	//PlayAnimation("Fire", 6, Anim_Linear(0, 0, GetAnimationLength("Fire"), 10, ANIM_Hold), Anim_Const(1000));
	//PlayAnimation("Fire", 6, Anim_Linear(0, 0, GetAnimationLength("Fire"), animation_set["ShootTime"], ANIM_Hold), Anim_Const(1000));

	//SetAction("Fire");
	
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
