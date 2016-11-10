#include Pickup_Basic

func ArmorValue(){ return 100; }

protected func PickupEffect(object player)
{
	if (player->~GetUTArmor() >= ArmorValue()) return false;
	player->~DoUTArmor(ArmorValue());
	player->~Sound("Pickup::UTArmor");
	return true;
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// properties

local Name = "$Name$";
local Description = "$Description$";
