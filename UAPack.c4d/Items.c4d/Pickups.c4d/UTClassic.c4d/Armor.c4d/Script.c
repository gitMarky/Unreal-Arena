#strict 2
#include I_PU

protected func PickupEffect(object pCrew)
{
	if(GetUTArmor(pCrew) >= 100) return false; // damit man nicht zwei Kˆrperpanzer hintereinander aufnimmt
	//AddEffect("UTArmorPickup",pCrew,50,1,pCrew,0,100,150);
	DoUTArmor(100,pCrew);
	pCrew->~Sound("UTArmor");
	return true;
}

global func GetUTArmor(object pCrew)
{
	  // Lokaler Aufruf
	  if (!pCrew) pCrew=this;
	  // Alle Effekte durchsuchen und die heiﬂen entfernen
	  var iEffect, i, total;
	  while (iEffect = GetEffect("UTArmorPickup", pCrew, i++))
	    total += EffectVar(0,pCrew,iEffect);

	  return total;
}

global func DoUTArmor(int iAmount,object pCrew)
{
	  // Lokaler Aufruf
	  if (!pCrew) pCrew=this;
	  AddEffect("UTArmorPickup",pCrew,50,1,pCrew,0,iAmount,150);
	  return;
}

global func FxUTArmorPickupStart(object pTarget, int iEffectNumber, int iTemp, int iAdd, int iMax)
{
	if(!iTemp)
	{
		EffectVar(0,pTarget,iEffectNumber)=Min(EffectVar(0,pTarget,iEffectNumber)+iAdd,iMax); // die braucht eigentlich niemand
		EffectVar(1,pTarget,iEffectNumber)=iMax;
	}
}

global func FxUTArmorPickupTimer(object pTarget, int iNr, int iTime)
{
	if(EffectVar(0,pTarget,iNr)<=0) return -1;
}

global func FxUTArmorPickupEffect(szNewEffect, pTarget, iEffectNumber)
{
	if (szNewEffect == "UTArmorPickup") return -2;
	return;
}

global func FxUTArmorPickupAdd(pTarget, iEffectNumber, szNewEffect, pTarget, iNewTimer, iAdd, iMax)
{
	// Aufruf erfolgt, wenn der Effekt ¸bernommen werden konnte
	EffectVar(1,pTarget,iEffectNumber)=Max(iMax,EffectVar(1,pTarget,iEffectNumber));
	EffectVar(0,pTarget,iEffectNumber)+=Min(iAdd,EffectVar(1,pTarget,iEffectNumber)-EffectVar(0,pTarget,iEffectNumber));
	return 1;
}

