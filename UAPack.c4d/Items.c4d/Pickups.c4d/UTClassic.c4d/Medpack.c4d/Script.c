#strict 2
#include I_PU

protected func PickupEffect(object pCrew)
{
	if(GetEnergy(pCrew) >= GetPhysical("Energy",0,pCrew)/1000) return false;
	pCrew->~DoEnergy(20);
	pCrew->~Sound("Heal");
	return true;
}
