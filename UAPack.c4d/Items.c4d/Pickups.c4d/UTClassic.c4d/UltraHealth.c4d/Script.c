#strict 2
#include I_PU

protected func PickupEffect(object pCrew)
{
	if(GetEnergy(pCrew) >= 200) return false;
	AddEffect("UTHealthInc",pCrew,50,1,pCrew,0,100,200);
	pCrew->~Sound("UTSuperHeal");
	return true;
}

global func FxUTHealthIncStart(object pTarget, int iEffectNumber, int iTemp, int iAdd, int iMax)
{
	if(!iTemp)
	{
		EffectVar(0,pTarget,iEffectNumber)=iAdd; // die braucht eigentlich niemand
		EffectVar(1,pTarget,iEffectNumber)=iMax;
		EffectVar(2,pTarget,iEffectNumber)=GetPhysical("Energy",0,pTarget)/1000;
		//Log("Started to to %d %d",EffectVar(0,pTarget,iEffectNumber),EffectVar(1,pTarget,iEffectNumber));

		var i = Min((GetEnergy(pTarget)+iAdd)*1000,iMax*1000);
		if(i > GetPhysical("Energy",2,pTarget))
		{
			SetPhysical("Energy",i,2,pTarget); // wir stacken hier nix
		}
		DoEnergy(iAdd,pTarget);
	}
}

global func FxUTHealthIncTimer(object pTarget, int iNr, int iTime)
{
	//Log("Comparing: %d %d",pTarget->~GetEnergy(),EffectVar(2,pTarget,iNr));
	// Entfernen, wenn er wieder auf Normalniveau ist
	if(pTarget->~GetEnergy() <= EffectVar(2,pTarget,iNr))
	{
		SetPhysical("Energy",EffectVar(2,pTarget,iNr)*1000,2,pTarget);
		return -1;
	}
	else // langsam runtersetzen, wir merken im HUD nicht, wenn wir Schaden über normalwert bekommen
	{
		EffectVar(0,pTarget,iNr)=GetEnergy(pTarget);
		SetPhysical("Energy",GetEnergy(pTarget)*1000,2,pTarget);
	}
}

global func FxUTHealthIncEffect(szNewEffect, pTarget, iEffectNumber)
{
	if (szNewEffect == "UTHealthInc") return -2;
	return;
}

global func FxUTHealthIncAdd(pTarget, iEffectNumber, szNewEffect, pTarget, iNewTimer, iAdd, iMax)
{
	// Aufruf erfolgt, wenn der Effekt übernommen werden konnte
	EffectVar(0,pTarget,iEffectNumber)+=iAdd;
	EffectVar(1,pTarget,iEffectNumber)=Max(iMax,EffectVar(1,pTarget,iEffectNumber));
	//Log("Changed to %d %d",EffectVar(0,pTarget,iEffectNumber),EffectVar(1,pTarget,iEffectNumber));

	var i = Min((GetEnergy(pTarget)+iAdd)*1000,iMax*1000);
	if(i > GetPhysical("Energy",2,pTarget))
	{
		SetPhysical("Energy",i,2,pTarget); // wir stacken hier nix
	}
	DoEnergy(iAdd,pTarget);

	return 1;
}

