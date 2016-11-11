#include Pickup_Basic

protected func PickupEffect(object player)
{
	if (player->~GetUTExtraHealth() >= UA_HEALTH_MAX) return false;
	player->~DoUTExtraHealth(UA_HEALTH_MAX);
	player->~Sound("Pickup::UTHealSuper");
	return true;
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// properties

local Name = "$Name$";
local Description = "$Description$";
