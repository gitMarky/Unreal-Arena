
global func GetUTShield(object pCrew)
{
	  // Lokaler Aufruf
	  if (!pCrew) pCrew=this;
	  // Alle Effekte durchsuchen und die hei�en entfernen
	  var iEffect, i, total;
	  while (iEffect = GetEffect("UTShieldPickup", pCrew, i++))
	    total += iEffect.var0;

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
		iEffectNumber.var0=Min(iEffectNumber.var0+iAdd,iMax); // die braucht eigentlich niemand
		iEffectNumber.var1=iMax;
	}
}

global func FxUTShieldPickupTimer(object pTarget, int iNr, int iTime)
{
	if(iNr.var0<=0) return -1;
}

global func FxUTShieldPickupEffect(szNewEffect, pTarget, iEffectNumber)
{
	if (szNewEffect == "UTShieldPickup") return -2;
	return;
}

global func FxUTShieldPickupAdd(pTarget, iEffectNumber, szNewEffect, pTarget, iNewTimer, iAdd, iMax)
{
	// Aufruf erfolgt, wenn der Effekt �bernommen werden konnte
	iEffectNumber.var1=Max(iMax,iEffectNumber.var1);
	iEffectNumber.var0+=Min(iAdd,iEffectNumber.var1-iEffectNumber.var0);
	return 1;
}
