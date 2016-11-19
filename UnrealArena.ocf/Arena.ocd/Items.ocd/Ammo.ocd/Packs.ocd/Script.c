//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// ammo definitions

public func IsAmmoPacket() { return true; }
public func AmmoID()       { return nil; }
public func AmmoCount()    { return 100; }

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// add ammo

public func RejectEntrance(object into)
{
	if (into->~IsAmmoManager() || into->~IsSpawnPoint())
	{
		if (TransferAmmoFromTo(this, into) > 0)
		{
			RemoveObject();
			return true;
		}
		else if (into->~IsAmmoManager())
		{
			return true;
		}
	}

	return false;
}

func TransferAmmoFromTo(object pack, object container)
{
	if (container->~IsAmmoManager())
	{
		var ammo_id = pack->AmmoID();
		var ammo_count = pack->AmmoCount();
		var transferrable = ammo_id->MaxAmmo() - container->~GetAmmo(ammo_id);

		// Fill up, but only if not full already
		if (transferrable > 0)
		{
			return container->DoAmmo(ammo_id, Min(transferrable, ammo_count));
		}		
	}
	return 0;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// properties

local Collectible = true;
