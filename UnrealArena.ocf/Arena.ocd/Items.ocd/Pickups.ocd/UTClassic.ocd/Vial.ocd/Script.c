#include Pickup_Basic

protected func PickupEffect(object player)
{
	if (player->~GetUTExtraHealth() >= UA_HEALTH_MAX) return false;
	player->~DoUTExtraHealth(5);
	player->~Sound("Pickup::UTHealVial");
	return true;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// properties

local Name = "$Name$";
local Description = "$Description$";
