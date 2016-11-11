#include Pickup_Basic

protected func PickupEffect(object player)
{
	if (player->GetEnergy() >= player->GetMaxEnergy()) return false;
	player->~DoEnergy(20);
	player->~Sound("Pickup::UTHeal");
	return true;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// properties

local Name = "$Name$";
local Description = "$Description$";
