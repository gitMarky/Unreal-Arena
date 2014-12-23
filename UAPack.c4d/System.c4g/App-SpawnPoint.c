/*-- Erweiterter Spawnpoint --*/

#strict 2
#appendto SPNP

public func IsSpawnPoint(){ return true; }

local szMyPos, aSound;


protected func Initialize()
{
  // Standardtimer = 1000 Frames
  spawntimer = 50;
  SetClrModulation(RGBa(0,0,0,255));
  // Arrays anlegen
  nocollect = CreateArray();
  player = CreateArray();
}


protected func Timer()
{
  // Noch nicht initialisiert?
  if(!initialized)
    // Ein Inhaltsobjekt?
    if(Contents())
      // Initialisieren
      return(Initialized());
  // Kein Inhalt?
  if(!Contents())
    {
    // Nicht mehr initialisiert
    initialized = false;
    return;
    }
  // Tolle Effekt starten
  angle += 30;
  if(angle >= 360) angle -= 360;
  SetObjDrawTransform(1000, 0,0,0, 1000, Sin(angle, 3)*1000 - 2000,0, 1);
  if(Random(2)) CreateParticle("NoGravSpark", RandomX(-5,5), RandomX(5,10), 0, -5, 25, RGBa(210, 210, 255, 100));
  // Alle Timer runterzählen
  DecreaseTimer();
}



global func PlaceSpawnpointEx(string szGlobal, int iX, int iY, int timer)
{
	var spwn = CreateObject(SPNP, iX, iY, -1);
	LocalN("szMyPos",spwn) = szGlobal;
	spwn->CreateContents(eval( szGlobal));
	if(timer)
		spwn->LocalN("spawntimer") = timer;
	return(spwn);
}

static gIDWeaponSet;
static gWeaponPos01, gWeaponPos02, gWeaponPos03, gWeaponPos04, gWeaponPos05, gWeaponPos06, gWeaponPos07, gWeaponPos08, gWeaponPos09;
static gWeaponPos10, gWeaponPos11, gWeaponPos12, gWeaponPos13, gWeaponPos14, gWeaponPos15;
static gAmmoPos01, gAmmoPos02, gAmmoPos03, gAmmoPos04, gAmmoPos05, gAmmoPos06, gAmmoPos07, gAmmoPos08, gAmmoPos09, gAmmoPos10;
static gItemPos01, gItemPos02, gItemPos03, gItemPos04, gItemPos05, gItemPos06, gItemPos07, gItemPos08, gItemPos09, gItemPos10;

global func UpdateSpawnPoints()
{
	var obj;
	for( obj in FindObjects(Find_ID(SPNP)) )
	if(LocalN("szMyPos",obj))
	{
		while(Contents(0,obj)) RemoveObject(Contents(0,obj));

		  SetGraphics(0, obj, obj->GetID(), 1, 1);

			obj->CreateContents(eval(LocalN("szMyPos",obj)));
			LocalN("initialized",obj) = false; // damit es ein neues Bild gibt usw.
	}
}


private func DecreaseTimer()
{
	if(GetType(aSound) != C4V_Array) aSound = [];
	// Waffen-Bleiben-Regel
	if(FindObject(WPST))
	{
		for(var i=0, iPlr ; i < GetPlayerCount() ; i++)
		// Spielertimer runterzählen
		{
			iPlr = GetPlayerByIndex(i);
			if(GetPlayerType(iPlr) == C4PT_Script) continue;

			if(player[iPlr])
			{
			player[iPlr] -= 5;
			aSound[iPlr] = 1;
			}
			if(player[iPlr] <= 0 && nocollect[iPlr])
			{
				// Spieler kann das Objekt wieder einsammeln
				player[iPlr] = 0;
				nocollect[iPlr] = false;
				Local() += 1<<GetPlayerByIndex(i);
				if(aSound[iPlr])
				{
					aSound[iPlr]=0;
					Sound("Respawn");
				}
			}
			}
	}
	else
	{
		// Timer runterzählen
			if(player[0])
			{
			player[0] -= 5;
			aSound[0] = 1;
			}
		if(player[0] <= 0)
		{
			// Objekt ist wieder da
			player[0] = 0;
			nocollect[0] = false;
			SetVisibility(VIS_All);
			if(aSound[0])
			{
				aSound[0]=0;
				Sound("Respawn");
			}
	}
	 }
}

/*
public func RejectEntrance(object pClonk)
{
	// Objekt ist ein Clonk?
	if(!(GetOCF(pClonk) & OCF_CrewMember) && !(pClonk->~CanCollectFromSpawnpoints())) return true;
	// Ich hab Contents?
	if(!Contents() || !initialized) return true;
	// Darf einsammeln
	if(CheckCollect(GetOwner(pClonk), pClonk))
	{
		var pObj = CreateContents(GetID(Contents()));
		// Kann der Clonk einsammeln?
		if(pObj->~IsAmmoPacket()) {
			if(!(pObj->MayTransfer(pClonk)))
				return true;
			if(NoAmmo()) return true;
		}

		Collect(pObj, pClonk);
		if( Contained(pObj) == this)
			RemoveObject(pObj);
		//}
		else
		{
			//Sound("Grab", 0, pClonk, 0, GetOwner(pClonk)+1);
			Collected(GetOwner(pClonk), pClonk);
			// Munition
			if(pObj->~IsAmmoPacket()) {
					pObj->~TransferAmmo(pClonk);
			}
			// Waffe, gleich einsatzbereit!
			else if(pObj->~IsWeapon())
				DoAmmo(pObj->GetFMData(FM_AmmoID),pObj->GetFMData(FM_AmmoLoad),pObj);
		}
	}
	return true;
}
*/
