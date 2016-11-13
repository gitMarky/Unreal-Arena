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
		if (into->~IsAmmoManager())
		{
			var transferrable = AmmoID()->MaxAmmo() - into->~GetAmmo(AmmoID());
			
			// Fill up, but only if not full already
			if (transferrable <= 0)
			{
				return false;
			}
		
			into->DoAmmo(AmmoID(), Min(transferrable, AmmoCount()));
			RemoveObject();
		}

		return true;
	}	
	
	return false;
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// properties

local Collectible = true;
