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
		name = 				"default", // string - menu caption
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
		damage_type = 		nil,	
	
		projectile_id = 	Projectile_Bullet,
		projectile_speed = 	210,
		projectile_range = 600,
		projectile_distance = 10,
		projectile_offset_y = -6,
		projectile_number = 1,
		projectile_spread = [5, 2], // 2 - default inaccuracy of a single projectile

		spread = [0, 1],			   // inaccuracy from prolonged firing
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
		delay_reload =		260,
		delay_recover = 	10,
	
		mode = 			 WEAPON_FM_Single,
	
		damage = 			6, 
		damage_type = 		nil,	
	
		projectile_id = 	Projectile_Bullet,
		projectile_speed = 	210,
		projectile_range = 600,
		projectile_distance = 10,
		projectile_offset_y = -6,

		spread = 6,
		spread_factor = 100,
	},
};

public func FireSound(object user, proplist firemode)
{
	Sound("m1-fire-reg", nil, nil, nil, 1);
}


public func OnFireProjectile(object user, object projectile, proplist firemode)
{
	projectile->Trail(1, 60);
}

public func OnStartCooldown(object user, proplist firemode)
{
	
	Sound("m1-fire-reg", nil, nil, nil, -1);
	Sound("m1-cooldown");
}

public func FireEffect(object user, int angle, proplist firemode)
{
	
	// muzzle flash
	
	var x = +Sin(angle, firemode.projectile_distance);
	var y = -Cos(angle, firemode.projectile_distance) + firemode.projectile_offset_y;

	EffectMuzzleFlash(user, x, y, angle, RandomX(15, 25), false, true);
		
	// casing
	
	x = +Sin(angle, firemode.projectile_distance / 2);
	y = -Cos(angle, firemode.projectile_distance / 2) + firemode.projectile_offset_y;

	var dir = (user->GetDir() * 2) - 1;

	CreateCartridgeEffect("Cartridge_Pistol", 2, x, y, - dir * Cos(angle - 35 * dir, RandomX(30, 45)), - dir * Sin(angle - 35 * dir, RandomX(30, 45)));
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
