#include Library_Projectile

protected func Initialize()
{
	SetObjectBlitMode(GFX_BLIT_Additive);
}

private func OnLaunch()
{
	SetAction("TravelBallistic");
	
	if (damage == 0) damage = RandomX(5, 10);
	
	var size = 40 + 4 * damage / 5;
	size *= 5;
	
	 SetObjDrawTransform(size, nil, nil, nil, size);
}


//public func Launch(int iXDir, int iYDir, int iDmg, int iSize)
//{
//  //Standardwerte setzen
//  if(!iDmg)     iDmg = 5+Random(5);
//  if(!iSize) iSize = 200;
//
//  //und zuweisen
//  iDamage = iDmg;
//  SetSpeed(iXDir, iYDir);
//
//  var self = this();
//  SetAction("Travel");
//  if(!self) return;   // Kleiner Sicherheitscheck, ob die Kugel nicht sofort verschwindet
//
//  //SetGraphics("Wodka");
//  //AddAlienFlareEffect(this(),iSize,RGB(81,53,30));
//  AddAlienFlareEffect(this(),iSize);
//
//  SetCon( 40 + 4*iDmg/5 ); // // 50+ iDmg*50/12);
//  //SetCon( 40 + 4*12*iDmg/5*40 );
//  SetCon( 40 + 6*iDmg/25 );
//
//  //AddAlienFlareEffect(this(),iSize);
//  AddEffect("HitCheck", this, 1,1, 0, SHT1);
//  return 1;
//}


//private func HitLandscape()
//{
//	CreateObject(Projectile_SlimeGlob, 0, 5, GetOwner())->~Glob2Ground(iDamage,GetCon());
//	Sound("bio-hit");
//}
//
//
//private func HitObject(object pObject)
//{
//	CreateObject(Projectile_SlimeGlob, 0, 0, GetOwner())->~AttachTo(pObject,GetCon());
//}

local ActMap = {
	TravelBallistic = {
		Prototype = Action,
		Name = "TravelBallistic",
		Procedure = DFA_NONE,
		Length = 1,
		Delay = 1,
		FacetBase = 0,
		X = 0,
		Y = 0,
		Wdt = 10,
		Hgt = 10,
		OffX = -2,
		OffY = -2,
		NextAction = "TravelBallistic",
		StartCall = "Travelling",
	},
};