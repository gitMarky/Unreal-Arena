#strict 2
#include I_PU

protected func PickupEffect(object pCrew)
{
	if(GetUTArmor(pCrew) >= 150) return false;
	//AddEffect("UTArmorPickup",pCrew,50,1,pCrew,0,50,150);
	DoUTArmor(50,pCrew);
	pCrew->~Sound("UTArmor");
	return true;
}
