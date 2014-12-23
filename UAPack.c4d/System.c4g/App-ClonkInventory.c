/*-- Inventarsteuerung --*/

#strict 2
#appendto UBOT

local aInventory;
local iLastContents;

public func WeaponEnd(&x, &y, bool bAkimbo)
{
	var number = GetEffect("ShowWeapon",this);
	if(!number)
		return 0;
	if(bAkimbo)
	{
		x = EffectVar(8, this, number)/1000;
		y = EffectVar(9, this, number)/1000;
	}	else
	{
		x = EffectVar(4, this, number)/1000;
		y = EffectVar(5, this, number)/1000;
	}
}


func Initialize()
{
	aInventory = [];
	_inherited();
}

protected func Collection2(object pObj)
{
	MakeInvInvisible(pObj);
	PushBack(pObj,aInventory);
	while(GetArrayItemPosition(0,aInventory)>0) DropArrayItem(0,aInventory);
	// aktuelles Objekt nochmal selektieren!
	if (!iLastContents) iLastContents = GetID(pObj);
	else ShiftContents(this,0,iLastContents,false);
	_inherited(pObj);
}

protected func Ejection(object pObj)
{
	DropArrayItem(pObj, aInventory);
	while(GetArrayItemPosition(0,aInventory)>0) DropArrayItem(0,aInventory);
	if (Contents())
		iLastContents = GetID(Contents());
	else iLastContents = 0;
	_inherited(pObj);
}

public func GetInventory(){ return aInventory; }

public func MakeInvInvisible( object pObj)
{
	var idID = GetID(pObj);
	//if(GetArrayItemPosition(idID,[]) > -1) idID = _HWT;
	SetGraphics("NoPic", pObj, idID );
}



protected func RejectCollect(id idObj, object pObj)
{
	if(!(pObj->~IsUAItem())) return true; // only collect UA items
	if(IsDisintegrating(pObj)) return true;
	// Für die KI
	var effect;
	if(effect = GetEffect("CollectionException", pObj))
		if(EffectVar(0, pObj, effect) == this)
			return true;
	// Spawnpunkt-Hack
	if(idObj == SPNP) return;
	// Munitionspaket?
	if(pObj ->~ IsAmmoPacket())
		// Davon kann man in jeden Fall _eines_ im Inventar haben
		if(!CustomContentsCount("IsAmmoPacket"))
			return false;
	// Pickup
	if(pObj->~IsPickup()) return(!(pObj->~OnCollection(this)));
	//Waffe?
	if(pObj ->~ IsWeapon())
	{
		//Sonderbehandlung?
		if(!(pObj ->~ OnCollection(this)))
		{
			//nein. Standardprozedur:
			//schon so eine Waffe im Inventar? Oder bereits 3 andere Waffen?
			if(ContentsCount(idObj) /* || CustomContentsCount("IsWeapon") >= 3*/) // der hier hat unbegrenzt Waffen, muahahaha!!!
				return  true;  //Ja, nicht aufnehmen
			else
				return false;
		}
		else
			return false;
	}
	// Einsammellimit für Ausrüstung
	if(pObj ->~ IsEquipment() && ContentsCount(idObj) > 0)
		return true;
	//Wieviel haben wir denn schon im inventar?
	if(ContentsCount() - CustomContentsCount("IsWeapon") >= 2)
		return true;

	// nicht angelegte Ausrüstung nochmal aufsammeln
	for(var gear in aGear)
    if(gear)
      if(GetID(gear) == idObj)
        return true;

	// Ok
	return false;
}

protected func ControlContents(idTarget)
{
	// Zielobjekt will gar nicht, dass wir was machen
	if(idTarget->~NoControlContents()) return false;
	// Hardcode: BR-Bombe darf man nicht abwählen
	if(Contents())
		if(Contents()->GetID() == GBRB)
			return true;

	// controlel Aim -> aufhören zu zielen außer die nächste Waffe kann es
	// Siehe ChangeWeapon
	if(ControlAim("ControlContents",idTarget)) return true;

	// Hast du noch einen letzten Wunsch, Contents(0)??!
	if(Contents(0)) Contents(0)->~Deselection(this);
	// Rotieren
	if (!ShiftContents(0, 0, idTarget)) return true;
	iLastContents = idTarget;
	// Waffe ziehen/wegstecken
	CheckArmed();

	// Objekt benachrichtigen
	if(Contents(0)) Contents(0)->~Selection(this);
}


