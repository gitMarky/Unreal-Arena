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

public func GetAmmoSource(type_or_firemode)
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


func IsWeaponReadyToUse(object user)
{
	var other_weapons = FindObjects(Find_Container(user), Find_Exclude(this), Find_Func("IsShooterWeapon"));
	
	var can_use = _inherited(user, ...);

	for (var weapon in other_weapons)
	{
		can_use = can_use && weapon->~IsReadyToFire();
	}

	return can_use;
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// Weapon as ammo

public func IsAmmoPacket() { return this->~IsDeparted(); } // only if the weapon has left the user


public func RejectEntrance(object into)
{
	if (into->~IsAmmoManager())
	{
		var other = FindObjects(Find_ID(this->GetID()), Find_Container(into), Find_Exclude(this));

		if (GetLength(other) <= 0) // this weapon is not in the inventory yet? -> collect normally
		{
			return false;
		}
		else if (this->IsAmmoPacket()) // he already has such a weapon -> collect only as ammo
		{
			if (Ammo_Pack->TransferAmmoFromTo(this, into))
			{
				RemoveObject();
			}
		}

		return true;
	}	
	
	return false;
}
