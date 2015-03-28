#include Library_Weapon

local Name = "$Name$";
local Description = "$Description$";
local Collectible = 1;

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