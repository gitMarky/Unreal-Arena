/*-- Splittergranate --*/

#strict

// Ist eine Granate
#include GREN

func Hit() { inherited(); HitObject(); }

func HitObject() {

  for(var i=RandomX(7,10); i>0; --i)
  {
    var obj=CreateObject(CGFR, 0, 0, GetController());
    var xdir = GetXDir()*2/3+RandomX(-30,30);
    var ydir = GetYDir()*2/3+RandomX(-30,30);
    var angle = Angle(0,0,xdir,ydir);

    obj->SetExplodeSize(iDamage);
    SetSpeed(xdir, ydir, obj);
    if(!Random(2))
      CreateSmokeTrail(RandomX(20,30),angle+RandomX(-30,30),0,0,this());
  }
  Explode(30);
}

func ExplodeDelay() { return(50); }
