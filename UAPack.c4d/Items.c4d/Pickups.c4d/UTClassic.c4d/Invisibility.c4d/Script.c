#strict 2
#include I_PU

protected func PickupEffect(object pCrew)
{
	if(!AddEffect("InvisBonus",pCrew,190,1,pCrew)) return false;
	pCrew->~Sound("UTInvis");
	return true;
}
