#strict
#include SHT_

public func DoCorrectPosition(){ return true; }

local glow;

/* Schuss */

//Extern für Überladung
private func CreateTrail(int iSize, int iTrail) {
  //glow = AddLight(100,RGB(128,64,255),this(),GLOW);
  /*
  pTrail = CreateObject(TRAI,0,0,-1);
  if(pTrail) {
    pTrail->Set(iSize+2,iTrail,this());
    pTrail->SetAction("Travel2");
    //pTrail->SetGraphics("Plasma");
    SetObjectBlitMode(GetObjectBlitMode(),pTrail);
  }*/

	/*if(!Random(2))*/ //CreateParticle("ColSparkAttach",RandomX(-2,2),RandomX(-2,2),RandomX(-1,1),RandomX(-1,1),40+Random(10),RGBa(0,255,0,80+Random(50)),this());
	CreateParticle("ColSparkAttach",RandomX(-3,3),RandomX(-3,3),RandomX(-1,1),RandomX(-1,1),40+Random(10),RGBa(0,255,0,80+Random(50)),this());

	//CreateParticle ("ColSparkNoGrav", -1+Random(3), -1+Random(3), 9*GetXDir()/10, 9*GetYDir()/10, 30, RGBa(0,120,0,20));

}

private func Traveling()
{
	CreateTrail();
  // effect
  if(glow)
    glow->ChangeColor(Color(GetActTime()));
  //CreateParticle("PSpark",0,0,-GetXDir()/4,-GetYDir()/4,RandomX(100,200)*GetCon()/100,
  //               RGBa(64,32,255,100+50*GetActTime()/iTime),this());
  return(_inherited());
}

/* Treffer */

private func HitObject(object pObject) {
  if(BulletStrike(pObject))
  {
    //LaserLight(RandomX(30,50), Color(GetActTime()),0,0,10);
    Remove();
  }

	CastParticles("ColSpark", 10, 20, 0, 0, 20, 40, RGBa(0,120,0,80), RGBa(0,120,0,140));
	Sound("PulseExp");

}

public func BulletStrike(object pObj) {
  if(pObj)
    DoDmg(iDamage, DMG_Energy, pObj, iPrec);
  return(1);
}

private func Color(int iATime) {
  var iPrg = 100*iATime/iTime;
  return(RGBa(127,255,127,iPrg*2));
}

private func GlowColor(int iATime) {
  var iPrg = 100*iATime/iTime;
  return(RGBa(70,50,255-iPrg,130));
}

public func TrailColor(int iATime) { return(Color(iATime)); }
