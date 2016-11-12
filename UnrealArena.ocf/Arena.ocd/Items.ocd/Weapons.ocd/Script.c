#include Library_Weapon
#include Plugin_Weapon_FiremodeByUse

local Collectible = true;

local firemode_primary, firemode_secondary;


public func GetCarryMode(object clonk, bool idle, bool nohand)
{
	if (idle || nohand || !is_selected)
	{
		return CARRY_None;
	}
	return CARRY_Hand;
}

public func GetCarrySpecial(object user) { if (is_selected) return "pos_hand2"; }
public func GetCarryBone() { return "Base"; }

func Hit()
{
	Sound("Hits::GeneralHit?");
}

public func GetAmmoSource(id ammo)
{
	return AMMO_Source_Container;
}

public func GetAmmoContainer()
{
	return Contained();
}

public func Initialize()
{
	_inherited(...);

	// firemode list
	ClearFiremodes();
	AddFiremode(firemode_primary);
	AddFiremode(firemode_secondary);
	ChangeFiremode(firemode_primary);
}


func IsShooterWeapon(){return true;}


func IsReadyToUse(object user)
{
	var other_weapons = FindObjects(Find_Container(user), Find_Exclude(this), Find_Func("IsShooterWeapon"));
	
	var can_use = _inherited(...);

	for (var weapon in other_weapons)
	{
		can_use = can_use && weapon->~IsReadyToFire();
	}

	return can_use;
}
