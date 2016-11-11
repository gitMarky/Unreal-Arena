func IsPickup()
{
	return true;
}


func Entrance(object container)
{
	if (container->~IsSpawnPoint()) return;
	Exit();
	SetPosition();
	SetCategory(C4D_StaticBack);
	this.Visibility = VIS_None;
	this->PickupEffect(container);
	RemoveObject();
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
