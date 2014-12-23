#strict 2
#include I_PU

protected func PickupEffect(object pCrew)
{
	if(!AddEffect("BerserkerBonus",pCrew,190,5,pCrew,0,true)) return false;
	pCrew->~Sound("UTInvis");
	return true;
}

//static const bUTMode = true;

global func BerserkerBonusColor( bool bUTMode, bool bParticle)
{
	if(bUTMode)
	{
		if(bParticle)
			return RGB(250,50+Random(100),250);
		return RGB(255,100,255);
	}
	else
	{
		if(bParticle)
			return RGB(250,50+Random(100),Random(20));
		return RGB(255,50,0);
	}
}

global func FxBerserkerBonusStart(object pTarget, int iEffectNumber, int tmp, bool bUTMode) {
  if(tmp)
    return;
  EffectVar(0,pTarget,iEffectNumber) = bUTMode;
  SetClrModulation(BerserkerBonusColor(bUTMode),pTarget);
}

global func FxBerserkerBonusTimer(object pTarget, int iEffectNumber, int iEffectTime) {
  CreateParticle("PxSpark",RandomX(-3,3),-8,0,-10,35,BerserkerBonusColor(EffectVar(0,pTarget,iEffectNumber),true),pTarget,1);
  if(iEffectTime > BONUS_Duration)
    return(-1);
}

global func FxBerserkerBonusStop(object pTarget, int iEffectNumber, int tmp) {
  if(tmp)
    return;
  SetClrModulation(RGB(255,255,255),pTarget);
}

global func FxBerserkerBonusEffect(string szNewEffectName, object pTarget, int iEffectNumber, int iNewEffectNumber) {
  if(szNewEffectName == "BerserkerBonus")
  {
    return(-1);
  }
}

global func FxBerserkerBonusFMData(object pTarget, int iEffectNumber, int iData, value) {
  //Damage*2!
  if(iData == FM_Damage)
  {
	  if(EffectVar(0,pTarget,iEffectNumber))
		  return(value*3);
	  else
		  return(value*2);
  }
  return(value);
}
