#include Library_AmmoManager
#appendto Clonk

public func GetAmmoSource(id ammo)
{
	// redundant at the moment, but it should avoid problems in the future
	// when real ammo counts are implemented 
	if (IsInstaGibConfigured())
	{
		return AMMO_Source_Infinite;
	}
	else
	{
		return AMMO_Source_Infinite;
	}
}