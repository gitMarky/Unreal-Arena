#strict 2
#include I_PU

protected func PickupEffect(object pCrew)
{
	if(GetUTShield(pCrew) >= 100) return false;
	//AddEffect("UTShieldPickup",pCrew,50,1,pCrew,0,100,100);
	DoUTShield(100,pCrew);
	pCrew->~Sound("UTShield");
	return true;
}

global func GetUTShield(object pCrew)
{
	  // Lokaler Aufruf
	  if (!pCrew) pCrew=this;
	  // Alle Effekte durchsuchen und die heiﬂen entfernen
	  var iEffect, i, total;
	  while (iEffect = GetEffect("UTShieldPickup", pCrew, i++))
	    total += EffectVar(0,pCrew,iEffect);

	  return total;
}

global func DoUTShield(int iAmount,object pCrew)
{
	  // Lokaler Aufruf
	  if (!pCrew) pCrew=this;
	  AddEffect("UTShieldPickup",pCrew,50,1,pCrew,0,iAmount,100);
	  return;
}

global func FxUTShieldPickupStart(object pTarget, int iEffectNumber, int iTemp, int iAdd, int iMax)
{
	if(!iTemp)
	{
		EffectVar(0,pTarget,iEffectNumber)=Min(EffectVar(0,pTarget,iEffectNumber)+iAdd,iMax); // die braucht eigentlich niemand
		EffectVar(1,pTarget,iEffectNumber)=iMax;
	}
}

global func FxUTShieldPickupTimer(object pTarget, int iNr, int iTime)
{
	if(EffectVar(0,pTarget,iNr)<=0) return -1;
}

global func FxUTShieldPickupEffect(szNewEffect, pTarget, iEffectNumber)
{
	if (szNewEffect == "UTShieldPickup") return -2;
	return;
}

global func FxUTShieldPickupAdd(pTarget, iEffectNumber, szNewEffect, pTarget, iNewTimer, iAdd, iMax)
{
	// Aufruf erfolgt, wenn der Effekt ¸bernommen werden konnte
	EffectVar(1,pTarget,iEffectNumber)=Max(iMax,EffectVar(1,pTarget,iEffectNumber));
	EffectVar(0,pTarget,iEffectNumber)+=Min(iAdd,EffectVar(1,pTarget,iEffectNumber)-EffectVar(0,pTarget,iEffectNumber));
	return 1;
}

