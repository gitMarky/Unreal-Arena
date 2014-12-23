#strict

local iDamage;

// Ist ein Schuss!
func IsBullet() { return true; }
local shooter; 			// Dingens/Clonk das den Schuss abgefeuert hat.
local shooter_weapon;	// ID des Schützen oder seiner Waffe

func Construction(object byObj)
{
	// nichts? :C
	if(!byObj)
		return;
	// Waffe?
	shooter = GetShooter(byObj);
	shooter_weapon = GetShooter(byObj,true);
	// Team?
	if(shooter->GetTeam())
		SetTeam(shooter->GetTeam());
}

public func GetWeaponID()
{
	return shooter_weapon;
}


public func Launch(int iXDir, int iYDir, int iDmg, int iSize)
{
  //Standardwerte setzen
  if(!iDmg)     iDmg = 5+Random(5);
  if(!iSize) iSize = 200;

  //und zuweisen
  iDamage = iDmg;
  SetSpeed(iXDir, iYDir);

  var self = this();
  SetAction("Travel");
  if(!self) return;   // Kleiner Sicherheitscheck, ob die Kugel nicht sofort verschwindet

  //SetGraphics("Wodka");
  //AddAlienFlareEffect(this(),iSize,RGB(81,53,30));
  AddAlienFlareEffect(this(),iSize);

  SetCon( 40 + 4*iDmg/5 ); // // 50+ iDmg*50/12);
  //SetCon( 40 + 4*12*iDmg/5*40 );
  SetCon( 40 + 6*iDmg/25 );

  //AddAlienFlareEffect(this(),iSize);
  AddEffect("HitCheck", this, 1,1, 0, SHT1);
  return 1;
}


private func Hit() {
  CreateObject(GLOB, 0, 5, GetOwner())->~Glob2Ground(iDamage,GetCon());
  Sound("bio-hit1");
  RemoveObject();
}


private func HitObject(object pObject) {
  CreateObject(GLOB, 0, 0, GetOwner())->~AttachTo(pObject,GetCon());
  DoDmg(iDamage, DMG_Bio, pObject);
  RemoveObject();
}

public func SetBallistic()
{
}
