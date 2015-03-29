#include Library_Weapon

local Name = "$Name$";
local Description = "$Description$";
local Collectible = 1;


public func GetCarryMode(object user) {    if (is_selected) return CARRY_Hand; }
public func GetCarrySpecial(object user) { if (is_selected) return "pos_hand2"; }
public func GetCarryBone() { return "Bone"; }
public func GetCarryTransform()
{
	return Trans_Mul(Trans_Rotate(90, 0, 1, 0), Trans_Rotate(-65, 1, 0, 0), Trans_Translate(13000, 8000, -2000));
	return Trans_Scale(600, 600, 600); //;Trans_Rotate(-90, 0, 1, 0);
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
		name = 				"default",
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

		spread = 1,
		spread_factor = 100,
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
	Sound("enforcer-fire");
}

public func FireEffect(object user, int angle, proplist firemode)
{
	//("Fire", 50, Anim_Linear(0, 0, GetAnimationLength("Fire"), 10, ANIM_Remove), Anim_Const(1000));
	//SetAction("Fire");
	user->CreateMuzzleFlash(0, 0, angle, 10);
	CreateCartridgeEffect("Cartridge_Pistol", 6, Sin(-angle, 10), -RandomX(6, 12));
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
