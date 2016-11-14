func IsPickup()
{
	return true;
}


func RejectEntrance(object container)
{
	// always enter spawn points
	if (container->~IsSpawnPoint())
	{
		return false;
	}
	else // do not enter
	{
		// but remove if you cause an effect on the guy
		if (this->PickupEffect(container));
		{
			RemoveObject();
		}
		return true;
	}
}


/* Callbacks */

func PickupEffect(object player)
{
	return false;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// properties

local Collectible = true;
