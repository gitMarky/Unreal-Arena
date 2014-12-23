/*-- Importiert benötigtes Zeug aus dem Hazardclonk --*/

#strict 2
#appendto UBOT

/* Leitet die Steuerung an das erste Inhaltsobjekt weiter. Rückgabewert 1 wenn erfolgreich */

private func Control2Contents (string command)
{
  // Haben wir was angefasst?
  if(GetAction() == "Push")
    return(0);
  // Pause Reload: nicht wieder anfangen ey!!!
  if(command == "ControlThrow") {
	if(WildcardMatch(GetAction(),"Scale*") || GetAction() == "Hangle")
	  return(1);
  }

  // Getragenes Objekt hat spezielle Steuerungsauswertung
  if(ObjectCall(Contents(), command, this, Par(1), Par(2), Par(3), Par(4), Par(5), Par(6), Par(7)))
    return(1);
  return(0);
}

/* Und mit angefassten Objekten auch noch */

private func Control2Grab ()
{
  // Haben wir was angefasst?
  if(GetAction() == "Push")
    return(1);

}

/* Spezielles Leiterverhalten */

public func ReleaseLadderUp() {
	//nach oben abspringen
	ReleaseLadder(-15*(GetDir()*2-1),-25);
}


public func CustomContentsCount(string funcname)
{
  // Alle Inventarobjekte durchlaufen und Waffen zählen
  for(var i=0,j,obj ; obj = Contents(i) ; i++)
    if(ObjectCall(obj, funcname))
      j++;
  // Waffen gezählt
  return(j);
}

// Vorbedingung für Jetpack prüfen
private func Check4Jetpack(object pCurrent, int path, bool fJetpack, int jetp, int ammoload)
{
  // kein Jetpackpath... dann ist ja gut
  if(!(pCurrent->WAYP::GetPathJetpack(path)))
    return(1);
  // ansonsten Vorbedingungen checken
  if(fJetpack)
    if(HasJetpack())
      //if(ammoload <= GetAmmo(GSAM)-jetp)
        return(2);
  return(0);
}


protected func Hangling() {
  PauseReloading();
  if(!GetEffect("ScaleReloading",this()))
    AddEffect("ScaleReloading",this(),1,1,this(),HZCK);
}

protected func Scaling()
{
  PauseReloading();
  if(!GetEffect("ScaleReloading",this()))
    AddEffect("ScaleReloading",this(),1,1,this(),HZCK);
  return(_inherited());
}

func FxScaleReloadingTimer()
{
  if(!Contents())
    return;
  if(!WildcardMatch(GetAction(),"*Scale*") && !WildcardMatch(GetAction(), "*Hangle*"))
  {
    ResumeReloading();
    return(-1);
  }
  if(Contents()->~IsReloading())
  	PauseReloading();
}

public func PauseReloading() {
  var c = Contents();
  // only if the weapon is an aim-weapon
  if(c)
    if(c->~IsWeapon())
      if(c->GetFMData(FM_Aim) > 0)
        c->PauseReload();
}

public func ResumeReloading() {
  var c = Contents();
  // only if the weapon is an aim-weapon
  if(c)
    if(c->~IsWeapon())
      if(c->GetFMData(FM_Aim) > 0)
        c->ResumeReload();
}

//protected func Departure(pContainer)      // Gebäude verlassen
//{
//  // nach Waffe suchen
//  CheckArmed();
//}

protected func ContentsDestruction()      // Wenn Inhaltsobjekte verschwinden
{
  // nach Waffe suchen
  ScheduleCall(this(), "CheckArmed", 1);
  // nächstes Objekt benachrichtigen
  if(Contents(1)) Schedule("Contents(0)->~Selection(this());", 1);
}

public func AmmoStoring()
{
  // keiner da: neuen erstellen
  if(!ammobag) {
    ammobag = CreateObject(ABAG);
    //ammobag->Init(this());
  }
  return(ammobag);
}

protected func ComNone()
{
  SetComDir(COMD_None);
}





// Inventar behandeln
public func CheckInventory()
{
  for(var i=0,obj ; obj = Contents(i) ; i++)
  {
      // BR-Hack
    if(Contents()->GetID() == GBRB)
    {
      BombingRunBomb();
      break;
    }
    // Wenn man etwas hardcoden möchte, dann einfach die Funktion AI_Inventory im Objekt erstellen und true zurückgeben
    if(obj->~AI_Inventory(this))
      continue;
    // Waffe
    if(obj->~IsWeapon())
      continue;
    // Ausrüstungsgegenstand
    if(obj->~IsHazardGear())
    {
      // Die KI braucht kein Licht und keine doppelten Dinge
	  // Kontext der Auskommentierung: http://cppp.tyron.at/bugtrack.php?shbug=833
	  // KI legt Taschenlampen an damit Spieler sie in der Dunkelheit sehen können
      if(/*obj->GetGearType() == GEAR_Light || */HasGear(obj->GetGearType()))
      {
        DropObject(obj);
        continue;
      }
      // Ausrüstung wird prinzipiell als was gutes angesehen -> anlegen
      ActivateGear(obj);
      continue;
    }
    // Mine
    if(obj->~IsMine())
    {
      // Platzieren
      PlaceMine(obj);
      continue;
    }
    // Upgrade
    if(obj->~IsUpgrade())
    {
      var wpn = GetUpgradeableWeapon(obj);
      // Eine passende Waffe gefunden -> upgraden (Upgrades sind immer gut.)
      if(wpn) UpgradeWeapon(obj, wpn);
      continue;
    }
    // Munition
    if(obj->~IsAmmoPacket())
    {
      ActivateAmmo(obj);
      continue;
    }
    // Objekt ist wertlos für uns
    DropObject(obj);
  }
}

// Objekt fallen lassen (verzögert, damit die Schleife nicht durcheinander kommt
public func DropObject(object pObj)
{
  Schedule(Format("Exit(Object(%d), 0, 10);", ObjectNumber(pObj)), 1, 0, this());
  // Nicht wieder einsammeln
  var effect = AddEffect("CollectionException", pObj, 1, 36);
  EffectVar(0, pObj, effect) = this();
}

// Ausrüstung anlegen (verzögert, damit die Schleife nicht durcheinander kommt
public func ActivateGear(object pObj)
{
  ScheduleCall(pObj, "Activate", 1, 0, this());
}

// Mine platzieren (verzögert, damit die Schleife nicht durcheinander kommt
public func PlaceMine(object pObj)
{
  ScheduleCall(pObj, "Activate", 1, 0, this());
}

// Waffe suchen, die man upgraden kann
public func GetUpgradeableWeapon(object pObj)
{
  for(var i=0,obj ; obj = Contents(i) ; i++)
    if(obj->~IsUpgradeable(pObj->GetID()))
      return(obj);
}

// Waffe upgraden (verzögert, damit die Schleife nicht durcheinander kommt
public func UpgradeWeapon(object pObj, object pWeapon)
{
  ScheduleCall(pObj, "UpgradeObject", 1, 0, pWeapon);
}

// Munition aufnehmen (verzögert, damit die Schleife nicht durcheinander kommt
public func ActivateAmmo(object pObj)
{
  ScheduleCall(pObj, "Activate", 1, 0, this());
}

// Hardgecodete Objekte

// Support-Drohne
// Zufällige Waffe für eine Drohne aussuchen
private func GetRandomDroneWeapon()
{
  var aWps = CreateArray();
  for(var i=0,obj ; obj = Contents(i) ; i++)
    if(obj->~IsWeapon())
      if(obj->GetFMData(FM_Aim)<=0)
        aWps[GetLength(aWps)] = obj;
  // Zufällig zurückgeben
  if(!GetLength(aWps)) return;
  return(aWps[Random(GetLength(aWps))]);
}

// BR-Bombe
protected func BombingRunBomb()
{
  // nächstes, feindliches Tor suchen
  var nextding, distance = -1;
  for(var pGate in FindObjects(Find_ID(GBRG)))
  {
    if(pGate->GetGoalTeam() == GetPlayerTeam(GetOwner())) continue;
    if(distance == -1)
    {
      nextding = pGate;
      distance = ObjectDistance(pGate);
    }
    else
    {
      if(ObjectDistance(pGate) < distance)
      {
        nextding = pGate;
        distance = ObjectDistance(pGate);
      }
    }
  }
  // Tor ist zu weit weg?
  if(distance > 200)
  {
    distance = -1;
    nextding = false;
  }
  // Freunde suchen, wenn der Timer abläuft
  if(!nextding && Contents()->GetBombTimer() <= 100)
  {
    for(var pFriend in FindObjects(Find_OCF(OCF_CrewMember), Find_Allied(GetOwner()), Find_Exclude(this)))
    {
      if(distance == -1)
      {
        nextding = pFriend;
        distance = ObjectDistance(pFriend);
      }
      else
      {
        if(ObjectDistance(pFriend) < distance)
        {
          nextding = pFriend;
          distance = ObjectDistance(pFriend);
        }
      }
    }
  }
  // Ziel?
  if(nextding)
  {

    // Ziel ist in Reichweite?
    if(distance <= 200 && PathFree(GetX(), GetY(), nextding->GetX(), nextding->GetY()))
    {
      // Winkel bestimmen
      var angle = Angle(GetX(), GetY(), nextding->GetX(), nextding->GetY());

      // Winkel stimmt!
      if(Inside(angle, 70, 110) || Inside(angle, 250, 290))
      {
		if(ReadyToFire()) {
	        if(GetX() < nextding->GetX())
	          SetDir(DIR_Right);
	        else
	          SetDir(DIR_Left);
	        // Feuer frei!
	        Contents()->ControlThrow();
		}
      }
      else
      {
        // Zielen beginnen

          // Feuer frei!
          if(GetDir() == DIR_Left)
            angle = 360-angle;
          angle = BoundBy(angle-10, 0, AIM_Max);
          crosshair->SetAngle(angle);
          UpdateAiming();
          Contents()->ControlThrow();
      }
    }
  }
}

/* Serialisierung */

public func Serialize(array& extra)
{
	var x, ammo;
	while(ammo = Contents(x++,ammobag)) {
		if(!(ammo ->~IsAmmo())) continue;
		extra[GetLength(extra)] = Format("DoAmmo(%i,%d)", GetID(ammo), GetAmmo(GetID(ammo)));
	}
}
