#include Pickup_Basic

func ShieldValue(){ return 100; }

protected func PickupEffect(object player)
{
	if (player->~GetUTShield() >= ShieldValue()) return false;
	player->~DoUTShield(ShieldValue());
	player->~Sound("Pickup::UTShield");
	return true;
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// properties

local Name = "$Name$";
local Description = "$Description$";
