func IsPickup()
{
	return true;
}


func OnCollection(object player)
{
	return this->PickupEffect(player);
}

func Entrance(object container)
{
	if (container->~IsSpawnPoint()) return;
	Exit();
	SetPosition();
	SetCategory(C4D_StaticBack);
	this.Visibility = VIS_None;
	ScheduleCall(this, this.RemoveObject, 1);
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
