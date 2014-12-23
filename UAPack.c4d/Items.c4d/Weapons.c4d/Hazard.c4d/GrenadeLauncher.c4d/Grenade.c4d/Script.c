/*-- Granate --*/

#strict

local iDamage;
local shooter;

func Construction(object byObj) {
  // nichts? :C
  if(!byObj)
    return();
  // Waffe?
  shooter = GetShooter(byObj);
}

func Launch(int xdir, int ydir, int iDmg)
{
  AddEffect("HitCheck", this(), 1, 1, 0, SHT1,shooter);
  AddEffect("Grenade", this(), 1, 1, this());
  SetSpeed(xdir, ydir);
  iDamage = iDmg;
  if(!iDamage)
    // Standardexplosion
    iDamage = 30;
}

// Aufprall
func Hit()
{
  var x, y;
  // Rechts angestoßen
  if(GetContact(this(), -1) & CNAT_Right )
    x=-(Abs(GetXDir()/4)+Random(3));
  // Links angestoßen
  else if(GetContact(this(), -1) & CNAT_Left ) x=Abs(GetXDir()/4)+Random(3);
  // Nichts davon: Alte YDir
  else x=GetXDir()/2;
  
  // Oben angestoßen
  if(GetContact(this(), -1) & CNAT_Top) y=Abs(GetYDir()/3);
  // Unten angestoßen
  // Warum verlieren Objekte auf glattem Boden ihre YDir..?
  // Hacken wir halt ein bisschen.. Zufall drauf.
  // Ich kann nicht mehr so recht lesen was da steht und ich denke das hat einen gewissen Vorteil.
  else if(GetContact(this(), -1) & CNAT_Bottom) y=-(Max(Abs(GetYDir()/4), Abs(GetXDir())/4)+Random(3));
  // Nichts davon: Alte YDir
  else y=GetYDir()/2;
  // Rumdopsen.. Komplizierter als man meint
  SetXDir(x);
  SetYDir(y);
  // Letztendlich hört man den Aufprall auch noch
  Sound("GrenadeHit*");
}

local noExplode;

func HitObject()
{
  RemoveEffect("Grenade", this(),0,1);

  Explosion(this,iDamage,iDamage,true);
  RemoveObject();

}

func ExplodeDelay() { return(110); }

func IsBulletTarget(id id)
{
  // Kann von anderen Geschossen getroffen werden
  return(true);
}

public func NoDecoDamage() { return(true); }

func FxGrenadeTimer(object target, int effect, int time)
{
  if(time > ExplodeDelay()) return(HitObject());
  var vel=Abs(GetXDir())+Abs(GetYDir());
  var alpha=Max(0,100-vel);
  
  CreateParticle("Smoke2", -GetXDir()/6, -GetYDir()/6, RandomX(-15, 15), -5,
                       vel/2+RandomX(20, 60), RGBa(100,100,100,50+alpha)); 
}

func FxGrenadeDamage()
{
  HitObject();
}

func IsBouncy() { return(true); }
