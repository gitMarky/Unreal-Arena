#include Pickup_Basic

func ArmorValue(){ return 50; }

protected func PickupEffect(object player)
{
	if (player->~GetUTArmor() >= UA_ARMOR_MAX) return false;
	player->~DoUTArmor(ArmorValue());
	player->~Sound("Pickup::UTArmor");
	return true;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// properties

local Name = "$Name$";
local Description = "$Description$";
