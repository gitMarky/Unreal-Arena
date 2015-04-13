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

public func GetAmmoSource()
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
	
		delay_prior = 		0, // time before the first shot is fired
		delay_reload =		260, // time to reload, in frames
		delay_recover = 	4, // time between consecutive shots
		delay_burst = 		0, // time between consecutive bursts
	
		mode = 			 WEAPON_FM_Auto,
	
		damage = 			6, 
		damage_type = 		nil,	
	
		projectile_id = 	Projectile_Bullet,
		projectile_speed = 	210,
		projectile_range = 600,
		projectile_distance = 10,
		projectile_offset_y = -6,
		projectile_number = 1,
		projectile_spread = 0, // default inaccuracy of a single projectile
		projectile_spread_factor = 100, // factor

		spread = 2,			   // inaccuracy from prolonged firing
		spread_factor = 100,   // factor
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
	Sound("enforcer-fire");
}

public func FireEffect(object user, int angle, proplist firemode)
{
	// this does nothing at the moment, 
	// because the attached mesh does not animate :(
	
	//PlayAnimation("Fire", 1, Anim_Linear(0, 0, GetAnimationLength("Fire"), 10, ANIM_Remove), Anim_Const(1000));
	PlayAnimation("Fire", 6, Anim_Linear(0, 0, GetAnimationLength("Fire"), 10, ANIM_Hold), Anim_Const(1000));
	//PlayAnimation("Fire", 6, Anim_Linear(0, 0, GetAnimationLength("Fire"), animation_set["ShootTime"], ANIM_Hold), Anim_Const(1000));

	//SetAction("Fire");
	
	// muzzle flash
	
	var x = +Sin(angle, firemode.projectile_distance);
	var y = -Cos(angle, firemode.projectile_distance) + firemode.projectile_offset_y;
	 
	
	EffectMuzzleFlash(user, x, y, angle, 10, false, true);
		
	// casing
	
	x = +Sin(angle, firemode.projectile_distance / 2);
	y = -Cos(angle, firemode.projectile_distance / 2) + firemode.projectile_offset_y;

	CreateCartridgeEffect("Cartridge_Pistol", 2, x, y, user->GetXDir() + Sin(-angle, 10), user->GetYDir() - RandomX(12, 15));
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
