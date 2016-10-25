
global func GetUTArmor(object pCrew)
{
	// Lokaler Aufruf
	if (!pCrew)
		pCrew = this;
	// Alle Effekte durchsuchen und die heiﬂen entfernen
	var iEffect, i, total;
	while (iEffect = GetEffect("UTArmorPickup", pCrew, i++))
		total += iEffect.var0;
	
	return total;
}

global func DoUTArmor(int iAmount, object pCrew)
{
	// Lokaler Aufruf
	if (!pCrew)
		pCrew = this;
	AddEffect("UTArmorPickup", pCrew, 50, 1, pCrew, 0, iAmount, 150);
	return;
}

global func FxUTArmorPickupStart(object pTarget, int iEffectNumber, int iTemp, int iAdd, int iMax)
{
	if (!iTemp)
	{
		iEffectNumber.var0 = Min(iEffectNumber.var0 + iAdd, iMax); // die braucht eigentlich niemand
		iEffectNumber.var1 = iMax;
	}
}

global func FxUTArmorPickupTimer(object pTarget, int iNr, int iTime)
{
	if (iNr.var0 <= 0)
		return -1;
}

global func FxUTArmorPickupEffect(szNewEffect, pTarget, iEffectNumber)
{
	if (szNewEffect == "UTArmorPickup")
		return -2;
	return;
}

global func FxUTArmorPickupAdd(pTarget, iEffectNumber, szNewEffect, pTarget, iNewTimer, iAdd, iMax)
{
	// Aufruf erfolgt, wenn der Effekt ¸bernommen werden konnte
	iEffectNumber.var1 = Max(iMax, iEffectNumber.var1);
	iEffectNumber.var0 += Min(iAdd, iEffectNumber.var1 - iEffectNumber.var0);
	return 1;
}
