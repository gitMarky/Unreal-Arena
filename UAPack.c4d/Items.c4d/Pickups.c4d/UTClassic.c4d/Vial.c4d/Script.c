#strict 2
#include I_PU

protected func PickupEffect(object pCrew)
{
	if(GetEnergy(pCrew) >= 200) return false;
	AddEffect("UTHealthInc",pCrew,50,1,pCrew,0,5,200);
	pCrew->~Sound("UTHeal");
	return true;
}
