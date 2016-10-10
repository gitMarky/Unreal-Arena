#include Library_Weapon
#include Plugin_Weapon_FiremodeByUse

local Name = "$Name$";
local Description = "$Description$";
local Collectible = true;


public func GetCarryMode(object user) {    if (is_selected) return CARRY_Hand; }
public func GetCarrySpecial(object user) { if (is_selected) return "pos_hand2"; }
public func GetCarryBone() { return "Base"; }
public func GetCarryTransform()
{
	return Trans_Mul(Trans_Rotate(90, -1, 0, 0), Trans_Rotate(-65, 0, 0, -1));
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
		delay_recover = 	15,
	
		mode = 			 WEAPON_FM_Single,
	
		damage = 			15, 
		damage_type = 		nil,	
	
		projectile_id = 	Projectile_Bullet,
		projectile_speed = 	180,
		projectile_range = 600,
		projectile_distance = 10,
		projectile_offset_y = -6,
		projectile_spread = {angle = 7, precision = 4},
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
		
		projectile_spread = {angle = 4, precision = 1},
	},
};

public func FireSound(object user, proplist firemode)
{
	Sound("enforcer-fire");
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
