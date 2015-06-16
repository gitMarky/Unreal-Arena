/*-- Schleimklumpen --*/


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


local target, // Klebezielobjekt
      damage; // Evtl. Schaden

func Initialize()
{
	//SetGraphics("Wodka");
	//AddAlienStinkEffect(this,100,RGB(81,53,30));

	AddAlienStinkEffect(this,100,0);
}

/* Kleben am Clonk */

public func AttachTo(pClonk,iCon)    // An einem Clonk heften
{
  SetVertexXY(0, GetX()-GetX(pClonk)+GetVertex(0, 0, pClonk), GetY()-GetY(pClonk)+GetVertex(0, 1, pClonk));
  SetAction("AGlobbing", pClonk);
  SetCon(iCon);
  if(GetX() < GetX(pClonk))
	  SetAction("AGlobbingLeft",pClonk);
  else
	  SetAction("AGlobbingRight",pClonk);

  if(!DecreasePhysical(pClonk)) return(RemoveObject());
}

private func DecreasePhysical(object pClonk)
{
  // Maximal 30% des Gesamtwertes von Walk ~!/$&)=%/
  //if(GetPhysical("Walk", 2, pClonk) <= GetPhysical("Walk", 1, 0, GetID(pClonk))*7/10)
  //  return(0);
  // Effekt starten
  AddEffect("Slimed", pClonk, 50, 800, 0, GetID());
  return(1);
}

/* Kleben am Boden */

public func Glob2Ground(iDamage,iCon)
{
	if(!iCon) iCon = 100;
  damage = iDamage;
  SetAction("Globbing");
  /*
  if(GBackSolid(-1,0)) SetAction("GlobbingRight");
  if(GBackSolid(+1,0)) SetAction("GlobbingLeft");
  if(GBackSolid(0,-1)) SetAction("GlobbingTop");
  if(GBackSolid(0,+1)) SetAction("GlobbingBottom");
*/
  var x = GetX();
  var y = GetY();
  SetCon(iCon);

  SetPosition(x,y);
}

protected func Check()
{
  var living;
  while(living = FindObject(0, -5,-5, 10,10, OCF_Living(), 0,0, NoContainer(), living))
    if(GetAlive(living) && !(living)->~IsAlien())
      return(HitLiving(living));
  while(living = FindObject(0, 1,1, 0,0, OCF_Living(), 0,0, NoContainer(), living))
    if(GetAlive(living) && !(living)->~IsAlien())
      return(HitLiving(living));

  //if(GetActTime() >= 600) return(Boom());
  if(GetActTime() >= 144) return(Boom()); // An UT-Zeit angepasst

}

private func HitLiving(target)
{
  // Maximal 30% des Gesamtwertes von Walk ~!/$&)=%/
  if(!(GetPhysical("Walk", 2, target) <= GetPhysical("Walk", 1, 0, GetID(target))*7/10))
    // Effekt starten
    AddEffect("Slimed", target, 101, 1500, 0, GetID());

  // Schaden roflol
  DoDmg(damage, DMG_Bio, target);
  // Weg
  Boom();
}

/* Schleimdingsis */

public func Boom()
{
	if( GetCon() > 53) return(MakeSplash());
  // Weg
  RemoveObject();
}

protected func AttachTargetLost()
{
  RemoveObject();
}

/* Schleim-Effekt */

public func FxSlimedStart(target, no, temp,int iStrength)
{
  if(temp)
    if(GetPhysical("Walk", 2, target) <= GetPhysical("Walk", 1, 0, GetID(target))*7/10)
      return(0);

  // Variablen
  // 0: Walk
  // 1: Jump
  // 2: Scale
  // 3: Hangle
  EffectVar(0, target, no) = GetPhysical("Walk", 1, 0, GetID(target))*1/10;
  EffectVar(1, target, no) = GetPhysical("Jump", 1, 0, GetID(target))*1/10;
  EffectVar(2, target, no) = GetPhysical("Scale", 1, 0, GetID(target))*1/10;
  EffectVar(3, target, no) = GetPhysical("Hangle", 1, 0, GetID(target))*1/10;

  SetPhysical("Walk", GetPhysical("Walk", 0, target)-EffectVar(0, target, no), 2, target);
  SetPhysical("Jump", GetPhysical("Jump", 0, target)-EffectVar(1, target, no), 2, target);
  SetPhysical("Scale", GetPhysical("Scale", 0, target)-EffectVar(2, target, no), 2, target);
  SetPhysical("Hangle", GetPhysical("Hangle", 0, target)-EffectVar(3, target, no), 2, target);

  // blar
  SetClrModulation(RGB(200,255,200), target);
}

public func FxSlimedTimer()
{
  return(-1);
}

public func FxSlimedStop(target, no, reason, temp)
{
  SetPhysical("Walk", GetPhysical("Walk", 0, target)+EffectVar(0, target, no), 2, target);
  SetPhysical("Jump", GetPhysical("Jump", 0, target)+EffectVar(1, target, no), 2, target);
  SetPhysical("Scale", GetPhysical("Scale", 0, target)+EffectVar(2, target, no), 2, target);
  SetPhysical("Hangle", GetPhysical("Hangle", 0, target)+EffectVar(3, target, no), 2, target);

  // blar
  if(!GetEffect("*Slimed*", target)) SetClrModulation(RGB(0,0,0), target);

  if(temp) return(0);

  // Schleimklumpen entfernen
  var glob = FindObject(GLOB, 0,0,0,0,0, "AGlobbing", target);
  if(glob) glob->~Boom();
}

// Feuer! :o
protected func Incineration() { Boom(); }




public func MakeSplash()
{
	//var blobs=(GetCon()-50)/4;
	//if(GetCon()<=50) blobs=0;

	//CastParticles("ColSpark", 10+blobs*2, 20+blobs, 0, 0, 20, 30, RGBa(0,80,0,80), RGBa(0,100,0,120));

	var constant = 40;//12;

	damage -= constant;

	while( damage > 0 )
	{
		CastBlob( Min(damage,constant));
		damage -= constant;
	}

	//MakeBlast();
	RemoveObject();
}

public func CastBlob( int iDamage)
{
	var obj=CreateObject( SLST,0,0,GetOwner() );
	SetPosition(GetX(),GetY(),obj);
	obj->~Launch(-20+Random(41), -20+Random(40), iDamage, iDamage);
	SetPosition(GetX()+BoundBy(GetXDir(obj),-1,1),GetY()+BoundBy(GetYDir(obj),-1,1),obj);
}


public func MakeBlast()
{
	Explosion( this(), 12 + 10*GetCon()/50, 10*GetCon()/50 , 0, "Slime" );
	RemoveObject();
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// properties

local Name = "$Name$";
local Description = "$Description$";
local BlastIncinerate = 1;
local ContactIncinerate = 1;

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// actions

local ActMap = {
Globbing = {
	Prototype = Action,
	Name = "Globbing",
	Procedure = DFA_FLOAT,
	Delay = 4,
	FacetBase = 1,
	NextAction = "Globbing",
	EndCall = "Check",
},

AGlobbing = {
	Prototype = Action,
	Name = "AGlobbing",
	Procedure = DFA_ATTACH,
	Delay = 4,
	FacetBase = 1,
	NextAction = "AGlobbing",
},

GlobbingLeft = {
	Prototype = Action,
	Name = "GlobbingLeft",
	Procedure = DFA_FLOAT,
	Directions = 1,
	Length = 1,
	Delay = 4,
	X = 10,
	Y = 0,
	Wdt = 8,
	Hgt = 14,
	NextAction = "GlobbingLeft",
	EndCall = "Check",
},

GlobbingRight = {
	Prototype = Action,
	Name = "GlobbingRight",
	Procedure = DFA_FLOAT,
	Directions = 1,
	Length = 1,
	Delay = 4,
	X = 10,
	Y = 14,
	Wdt = 8,
	Hgt = 14,
	NextAction = "GlobbingRight",
	EndCall = "Check",
},

GlobbingTop = {
	Prototype = Action,
	Name = "GlobbingTop",
	Procedure = DFA_FLOAT,
	Directions = 2,
	Length = 1,
	Delay = 4,
	X = 18,
	Y = 0,
	Wdt = 10,
	Hgt = 8,
	NextAction = "GlobbingTop",
	EndCall = "Check",
},

GlobbingBottom = {
	Prototype = Action,
	Name = "GlobbingBottom",
	Procedure = DFA_FLOAT,
	Directions = 2,
	Length = 1,
	Delay = 4,
	X = 18,
	Y = 8,
	Wdt = 10,
	Hgt = 8,
	OffX = 0,
	OffY = 1,
	NextAction = "GlobbingBottom",
	EndCall = "Check",
},

AGlobbingLeft = {
	Prototype = Action,
	Name = "AGlobbingLeft",
	Procedure = DFA_ATTACH,
	Directions = 2,
	FlipDir = 1,
	Length = 1,
	Delay = 4,
	X = 10,
	Y = 0,
	Wdt = 8,
	Hgt = 14,
	NextAction = "AGlobbingLeft",
},

AGlobbingRight = {
	Prototype = Action,
	Name = "AGlobbingRight",
	Procedure = DFA_ATTACH,
	Directions = 2,
	FlipDir = 1,
	Length = 1,
	Delay = 4,
	X = 10,
	Y = 14,
	Wdt = 8,
	Hgt = 14,
	NextAction = "AGlobbingRight",
},

AGlobbingTop = {
	Prototype = Action,
	Name = "AGlobbingTop",
	Procedure = DFA_ATTACH,
	Directions = 2,
	Length = 1,
	Delay = 4,
	X = 18,
	Y = 0,
	Wdt = 10,
	Hgt = 8,
	NextAction = "AGlobbingTop",
},

AGlobbingBottom = {
	Prototype = Action,
	Name = "AGlobbingBottom",
	Procedure = DFA_ATTACH,
	Directions = 2,
	Length = 1,
	Delay = 4,
	X = 18,
	Y = 8,
	Wdt = 10,
	Hgt = 8,
	OffX = 0,
	OffY = 1,
	NextAction = "AGlobbingBottom",
},

};
