#strict 2
#appendto WEPN

// kann im Chooser ausgew�hlt werden
public func IsChooseableWeapon(){ return true; }

// herumliegende Waffen werden entfernt
public func RemoveOnRestart(){ return !Contained(); }

/* Konstanten */
//Feuermodi-Konstanten
static const FM_Name = 			 1;		// Name des Feuermodus'

static const FM_AmmoID = 		 2;		// ID der benutzten Munition
static const FM_AmmoLoad = 		 3;		// Menge der eingeladenen Munition
static const FM_AmmoUsage = 	 4;		// Munitionsverbrauch pro AmmoRate Schüsse
static const FM_AmmoRate = 		 5;		// s.o.

static const FM_Reload = 		 6;		// Nachladezeit der Waffe in Frames.
static const FM_Recharge = 		 7;		// Zeitabstand zwischen zwei Schüssen in Frames.

static const FM_Auto = 			 8;		// Automatische Waffe.
static const FM_Aim =			 9;		// Waffe zielt
static const FM_Condition =		10;		// Vorraussetzung für den Schussmodus

static const FM_Icon =			11;		// Icon das im Schussmodimenü angezeigt wird

static const FM_Damage =		12;		// Schaden, den der Schussmodus anrichtet

// neue Konstanten:
static const FM_DamageType =	13;		// Schadenstyp, den der Schussmodus anrichtet
static const FM_Accuracy = 		14;		//
static const FM_AimAngle = 		15;		//
static const FM_ProjID = 		16;		// ID des Projektils
static const FM_Ballistic = 	17;		// ist das Projektil ballistisch?
static const FM_ProjSpeed = 	18;		// Geschwindigkeit des Projektils
static const FM_ProjRange = 	19;		// wie weit fliegt das Projektil
static const FM_ProjSize = 		20;		// wie breit ist das Projektil / die Spur
static const FM_ProjTrail = 	21;		// wie lang ist die Spur?
static const FM_ProjSound =		22;		// welchen Sound macht der Modus
static const FM_ProjEffects = 	23;		// hat der Feuermodus einen eigenen Effekte-Call?
static const FM_ProjCustom = 	24;		// hat der Feuermodus einen eigenen Launch-Call?
static const FM_Old = 			25;		// macht der Feuermods einen Fire%d-Call?
static const FM_Hitscan =		26;		// trifft die Waffe sofort?
static const FM_Burst = 		27;		// wenn Auto, wie oft wird maximal geschossen? Unbenutzt und nicht implementiert!!
static const FM_SightBonus = 	28;		// so viel kriegt der Spieler zu seiner ViewRange

// Bot-Konstanten für Feuermodi
static const BOT_Range    = 	100;  // Reichweite des Modus
static const BOT_DmgType  = 	101;  // Schadenstyp, den der Modus hauptsächlich anrichtet

static const BOT_Ballistic= 	102;  // Der Modus schießt ballistische Geschosse - ist eigentlich �berholt, da das jetzt im FM drin ist.

static const BOT_Power = 		103;  // Stärke der Waffe
static const BOT_EMP = 			104;  // EMP-Waffe. Wird nur gegen Maschinen eingesetzt
static const BOT_Rating = 		105; // bestimmt zuf�llig und nach Clonk-Skill, ob er schie�t, wenn er k�nnte
static const BOT_RangeMin = 	106; // Alternative Modi: minimale Reichweite, die er haben muss, um es zu benutzen
static const BOT_Priority =		107; // Inventar- und Benutzungspriorit�t - ersetzt gewissermassen BOT_Power
static const BOT_Override = 	108; // Hat die Waffe eigene KI-Anwendungs-Behandlung?

public func Default(int data)    // Standardeinstellungen
{
  if(data == FM_Name)		return "Standard";
  if(data == FM_AmmoID)		return STAM;
  if(data == FM_AmmoUsage)	return 1;
  if(data == FM_AmmoRate)	return 1;
  if(data == FM_Auto)		return false;
  if(data == FM_Aim)		return 0;
  if(data == FM_Condition)	return true;
  if(data == FM_AmmoLoad)	return 5;
  if(data == FM_Reload)		return 100;
  if(data == FM_Recharge)	return 10;
  if(data == FM_Icon)		return GetID();
  if(data == FM_Damage)		return 0;
  if(data == FM_DamageType)	return DMG_Projectile;
  if(data == FM_AimAngle)	return 1;
  if(data == FM_Accuracy)	return 3;
  if(data == FM_ProjID)		return SHT_;
  if(data == FM_Ballistic) 	return 5;
  if(data == FM_ProjSpeed)	return 250;
  if(data == FM_ProjRange)	return 9999;
  if(data == FM_ProjSize)	return 2;
  if(data == FM_ProjTrail)	return 100;
  if(data == FM_ProjSound)	return 0;
  if(data == FM_ProjEffects)return false;
  if(data == FM_ProjCustom)	return false;
  if(data == FM_Old)		return false;
  if(data == FM_Hitscan )	return false;
  if(data == FM_Burst)		return 0;
  if(data == FM_SightBonus) return 0;
  if(data == BOT_Range)		return 120;
  if(data == BOT_RangeMin)	return 0;
  if(data == BOT_DmgType)	return DMG_Projectile;
  if(data == BOT_Ballistic)	return 0;
  if(data == BOT_Power)		return BOT_Power_1;
  if(data == BOT_EMP)		return false;
  if(data == BOT_Rating)	return 10;
  if(data == BOT_Priority)	return 20;
}

public func KillMessage(){ return $KillMsg$; }


/*
// setzt eine Grafik, f�r eine zweite Waffe
public func Collection2( object pObj )
{
	if( GetID(pObj) == GetID(this) )
	{
		SetGraphics(0,this,GetID(),1,GFXOV_MODE_ExtraGraphics,0,0,pObj);
	}
	_inherited( pObj );
}

public func Ejection( object pObj)
{
	if( GetID(pObj) == GetID(this) )
	{
		SetGraphics(0,this,GetID(),1);
	}
	_inherited( pObj );
}
*/

// wenn true, dann benutzt die Waffe Munition direkt aus der AmmoBag
public func FeedUserAmmo(){ return false; }

public func CanAkimbo(){ return MOD_Akimbo(); }
public func GetAkimbo(){ return FindContents(GetID(this)); }
public func IsAkimbo(){ if(!Contained()) return false; return (GetID(Contained()) == GetID(this)); }
public func GetAkimboHUDOffsetX(){ return 3000; }
public func GetAkimboHUDOffsetY(){ return -5000; }
public func GetAkimboHUDRotation(){ return 0; }

public func OnCollection( object pCrew )
{
	if(CanAkimbo())
	{
		var inventory = FindObjects( Find_ID(GetID(this)), Find_Container(pCrew) );

		var weapon;
		//if(GetType(inventory) == C4V_Array)
		//if(GetLength(inventory))
		for( weapon in inventory )
		{
			if(weapon->~GetAkimbo()) continue;
			Enter(weapon,this);
			break;
		}
	}
	return _inherited( pCrew);
}


protected func Entrance(object pContainer)
{
	if(FeedUserAmmo())
	{
		DoAmmo(GetFMData(FM_AmmoID, 1), GetFMData(FM_AmmoLoad, 1), pContainer);
		pContainer->~AmmoTransferred();
	}

	SetR();

	_inherited( pContainer );
}


/* Feuern */


public func ControlThrow(caller)
{
	if(!caller) return 0;
	var pAkimbo = GetAkimbo();
	if( pAkimbo )
		ScheduleCall(pAkimbo,"ControlThrow",5,0,caller);

	_inherited(caller);
}

/*
public func Fire()
{
	// Akimbo-Waffe benutzen
	var pAkimbo = GetAkimbo();
	if( pAkimbo )
	{
		pAkimbo->~SetUser(caller);
		//ScheduleCall(pAkimbo,"Fire",5);
		ScheduleCall(pAkimbo,"Fire",5,0);
	}
}*/
/*
// das gleiche wie fr�her, nur im Akimbo-Modus
public func Shoot(object caller)
{
	// Feuern mit Feuermodus

	// Akimbo-Waffe benutzen
	var pAkimbo = GetAkimbo();
	if( pAkimbo )
	{
		pAkimbo->~SetUser(caller);
		ScheduleCall(pAkimbo,"Shoot",5,0,caller);
	}

	_inherited( caller );
}

*/
// EffectVars:
//  0 - ID der Waffe
//  1 - Aktuelle Drehung
//  2 - X-Position des Anfangs der Waffe
//  3 - Y-Position des Anfangs der Waffe
//  4 - X-Position des Ende des Laufs
//  5 - Y-Position des Ende des Laufs
//  6 - Aktuelle Waffe
//  7 - Akimbo-Waffe
//  8 - X-Position des Ende des Akimbo-Laufs
//  9 - Y-Position des Ende des Akimbo-Laufs

// so angepasst, dass die Waffe doppelt getragen werden kann
global func FxShowWeaponTimer(object pTarget, int iNumber, int iTime)
{
	// Waffe aktualisieren:
	var xoff, yoff, r;	// Offset, Winkel
	var xoff2, yoff2;	 // das gleiche f�r die zweite Waffe
	// kein Inventar oder falsche Aktion
	if(!Contents(0,pTarget))
	{
		EffectVar(0, pTarget, iNumber) = 0;
		if(EffectVar(6, pTarget, iNumber))
		{
			SetObjDrawTransform(1000,0,0,0,1000,0,EffectVar(6,pTarget,iNumber));
			EffectVar(6, pTarget, iNumber) = 0;
		}
		if(EffectVar(7, pTarget, iNumber))
		{
			SetObjDrawTransform(1000,0,0,0,1000,0,EffectVar(7,pTarget,iNumber));
			EffectVar(7, pTarget, iNumber) = 0;
		}
		//if(EffectVar(6,pTarget,iNumber)->~GetAkimbo()) SetObjDrawTransform(1000,0,0,0,1000,0,EffectVar(6,pTarget,iNumber)->~GetAkimbo());
		SetGraphics(0, pTarget, 0, WeaponDrawLayer);
		SetGraphics(0, pTarget, 0, WeaponDrawLayer+1);
		return(FX_OK);
	}
	//Die Waffe momentan überhaupt anzeigen?
	if(!(pTarget->~WeaponAt(xoff, yoff, r)))
	{
		EffectVar(0, pTarget, iNumber) = 0;
		if(EffectVar(6, pTarget, iNumber))
		{
			SetObjDrawTransform(1000,0,0,0,1000,0,EffectVar(6,pTarget,iNumber));
			//if(EffectVar(6,pTarget,iNumber)->~GetAkimbo()) SetObjDrawTransform(1000,0,0,0,1000,0,EffectVar(6,pTarget,iNumber)->~GetAkimbo());
			EffectVar(6, pTarget, iNumber) = 0;
		}
		if(EffectVar(7, pTarget, iNumber))
		{
			SetObjDrawTransform(1000,0,0,0,1000,0,EffectVar(7,pTarget,iNumber));
			EffectVar(7, pTarget, iNumber) = 0;
		}
		SetGraphics(0, pTarget, 0, WeaponDrawLayer);
		SetGraphics(0, pTarget, 0, WeaponDrawLayer+1);
		return(FX_OK);
	}
	var obj = Contents(0,pTarget), id=GetID(obj);
	var akimbo = obj->~GetAkimbo();
	// Waffe nicht mehr aktuell
	if(EffectVar(0, pTarget, iNumber) != id)
	{
		// neues Objekt ist Waffe, oder ein Objekt, das gezeichnet werden soll
		if(obj->~IsWeapon() || obj->~IsDrawable())
		{
			EffectVar(0, pTarget, iNumber) = id;
			EffectVar(6, pTarget, iNumber) = obj;
			SetGraphics(0, pTarget,id, WeaponDrawLayer, GFXOV_MODE_Object,0,GFX_BLIT_Parent,obj);
			if(akimbo)
			{
				EffectVar(7, pTarget, iNumber) = akimbo;
				SetGraphics(0, pTarget,id, WeaponDrawLayer+1, GFXOV_MODE_Object,0,GFX_BLIT_Parent,akimbo);
			}
		}
		// neues Objekt ist keine Waffe
		else
		{
			EffectVar(0, pTarget, iNumber) = 0;
			if(EffectVar(6, pTarget, iNumber))
			{
				SetObjDrawTransform(1000,0,0,0,1000,0,EffectVar(6,pTarget,iNumber));
				EffectVar(6, pTarget, iNumber) = 0;
			}
			if(EffectVar(7, pTarget, iNumber))
			{
				SetObjDrawTransform(1000,0,0,0,1000,0,EffectVar(7,pTarget,iNumber));
				EffectVar(7, pTarget, iNumber) = 0;
			}
			SetGraphics(0, pTarget, 0, WeaponDrawLayer);
			SetGraphics(0, pTarget, 0, WeaponDrawLayer+1);
			return(FX_OK);
		}
	}

	id = EffectVar(0, pTarget, iNumber);
	obj = EffectVar(6, pTarget, iNumber);
	akimbo = EffectVar(7, pTarget, iNumber);

	// Ausrichtung nach Blickrichtung des Clonks
	// Variablen für die Transformation

	var width, height;	// Breiten- und Höhenverzerrung der Waffe
	var xskew, yskew;	 // Zerrung der Waffe, wird zur Rotation gebraucht
	var size;					 // Größe der Waffe in der Hand: 1000 = 100%
	// Variablen für die Position
	var xaim, yaim;		 // Offset, dass sich durch zielen ergibt
	var dir;						// Richtung in die das Objekt schaut

	//schnell noch Rotation dazurechnen oder so!
	r += ObjectCall(obj,"HandR");

	// Variablen mit Werten versehen
	width = height = xskew = yskew = 1;
	size = id->~HandSize();
	if(!size) size = 1000;
	//dir	= GetDir()*2-1;
	dir	= pTarget->~GetDir(pTarget)*2-1;
	if(r > 180 || r < -180)
		dir *= -1;
	r *= dir;

	//var bAkimbo;
	//if(GetUnusedOverlayID(1,obj)>1) bAkimbo = true;
	//var pAkimbo = obj->~GetAkimbo();

	// Attachpunkte dazurechnen
	var xfact = size * ObjectCall(obj,"HandX");
	var yfact = size * ObjectCall(obj,"HandY");
	var xfact2, yfact2;

	xoff += Cos(r,xfact)/1000 + dir*Sin(r,yfact)/1000;
	yoff -= Cos(r,yfact)/1000 - dir*Sin(r,xfact)/1000;

	if(akimbo)
	{
		// Attachpunkte dazurechnen
		xfact2 = size * ObjectCall(obj,"AkimboX");
		yfact2 = size * ObjectCall(obj,"AkimboY");

		xoff2 += Cos(r,xfact2)/1000 + dir*Sin(r,yfact2)/1000;
		yoff2 -= Cos(r,yfact2)/1000 - dir*Sin(r,xfact2)/1000;
	}


	if(dir == 1)
	{
		height = -1;
		xskew = -1;
		yskew = -1;
	}

	r = -90*dir-r-90;
	height *= width *= Cos(r, size);
	xskew *= Sin(r, size);
	yskew *= -xskew;
	xoff *= dir; xoff2 *= dir;

	SetObjDrawTransform(1000,xskew,xoff,yskew,1000,yoff, pTarget, WeaponDrawLayer); //position
	SetObjDrawTransform(width,xskew,0,yskew,height,0, obj); //Groesse und Rotation
	if( akimbo)
	{
		SetObjDrawTransform(1000,xskew,xoff+xoff2,yskew,1000,yoff+yoff2, pTarget, WeaponDrawLayer+1); //position
		SetObjDrawTransform(width,xskew,0,yskew,height,0, akimbo); //Groesse und Rotation

		//SetObjDrawTransform(width,xskew,xoff2,yskew,width,yoff2, obj, 1); //Groesse und Rotation der zweiten Waffe
	}
	//if( bAkimbo) SetObjDrawTransform(width,0,xoff2,0,width,yoff2, obj, 1); //Groesse und Rotation der zweiten Waffe

	// abspeichern, damit abrufbar
	r = -r-90;
	var w = GetDefCoreVal("Width",0,id)/2;
	//var brly = DefinitionCall(id,"BarrelYOffset");
	//var brlx = DefinitionCall(id,"BarrelXOffset");
	var brly = obj->~BarrelYOffset();
	var brlx = obj->~BarrelXOffset();
	var r2 = (Angle(0,0,w-brlx/1000,brly/1000)-90)*dir;
	var dist = Distance(0,0,w*1000-brlx,brly);

	//Log("%d - %d - %d - %d",w,brl,r2,dist);
	EffectVar(1, pTarget, iNumber) = r;
	EffectVar(2, pTarget, iNumber) = xoff-Sin(r,size*w);
	EffectVar(3, pTarget, iNumber) = yoff+Cos(r,size*w);
	EffectVar(4, pTarget, iNumber) = xoff+Sin(r+r2,size*(dist))/1000;
	EffectVar(5, pTarget, iNumber) = yoff-Cos(r+r2,size*(dist))/1000;
	//EffectVar(4, pTarget, iNumber) = xoff+Sin(r,size*(w));
	//EffectVar(5, pTarget, iNumber) = yoff-Cos(r,size*(w));
	//Log("%d / %d",EffectVar(4, pTarget, iNumber),EffectVar(5, pTarget, iNumber));

	if( akimbo)
	{
		brly = akimbo->~BarrelYOffset();
		brlx = akimbo->~BarrelXOffset();
	}
	else
	{
		brly = 0; brlx = 0;
	}
	r2 = (Angle(0,0,w-brlx/1000,brly/1000)-90)*dir;
	dist = Distance(0,0,w*1000-brlx,brly);

	EffectVar(8, pTarget, iNumber) = xoff+Sin(r+r2,size*(dist))/1000;
	EffectVar(9, pTarget, iNumber) = yoff-Cos(r+r2,size*(dist))/1000;
}

global func FxShowWeaponStop(object pTarget, int iNumber, int iReason, bool fTemp)
{
	if (fTemp) return(FX_OK);
	// Grafik entfernen
	SetGraphics(0, pTarget, 0, WeaponDrawLayer);
	SetGraphics(0, pTarget, 0, WeaponDrawLayer+1);

	if(EffectVar(6,pTarget,iNumber))
		SetObjDrawTransform(1000,0,0,0,1000,0,EffectVar(6,pTarget,iNumber));
	if(EffectVar(7,pTarget,iNumber))
		SetObjDrawTransform(1000,0,0,0,1000,0,EffectVar(7,pTarget,iNumber));
}

global  func GetAmmo(id ammoid, object target)
{
	if(!target) target = this;
	if( target->~IsWeapon() && target == this && target->~GetUser() && target->~FeedUserAmmo() )
		return _inherited(ammoid, this->~GetUser());
	else
		return _inherited( ammoid, target);
}
/*
public  func CheckAmmo(id ammoid, int count)
{
	if( target == this && GetUser() )
		_inherited(ammid, GetUser());
	else
		_inherited( ammoid, count);
}
*/
/*
global func DoAmmo(id ammoid, int change, object target)
{
	if(!target) target = this;
	if( target->~IsWeapon() && target == this && target->~GetUser() && target->~FeedUserAmmo() )
		return _inherited(ammoid, change, this->~GetUser());
	else
		return _inherited( ammoid, change, target);
}
*/

public func GetCharge()
{   // wie voll ist die Waffe
 	var charge;
	var ammoid = GetFMData(FM_AmmoID);
	// lädt nach: Nachladestatus anzeigen
	if(IsReloading())
	{
		// Nachladestatus in %
		charge = 1000*(EffectVar(0, this(), GetEffect("Reload", this())))/GetFMData(FM_Reload);
	}
	// ansonsten: Ladestand anzeigen
	else
	{
		var max = GetFMData(FM_AmmoLoad);
		if(FeedUserAmmo())
		{
			max = ammoid->~MaxAmmo();
			if(!max) max = 999;
		}
		charge = 1000*GetAmmo(ammoid)/max;
	}

  return(charge);
}


/* Waffenmenü */

public func ControlDigDouble(caller)
{
	if(HasWeaponMenu()) return _inherited(caller);
}

public func HasWeaponMenu(){ return false;}

public func ControlDig( object pCaller )
{
	if(!HasWeaponMenu())
	{
		StopAutoFire();

		if( firemode == 1)
			SetFireMode(2);
		else
			SetFireMode(1);

		if(GetFMData(FM_Auto))
			return ControlThrow(pCaller);
		else
			return true; //ControlThrow(pCaller);
	}
}


private func SetFireMode(int i)
{
	if(i == 0) return;
	if(HasWeaponMenu())
		return _inherited(i);
	else
	{
		  // Gleicher Modus: Nur nachladen wenn nicht mehr voll und lädt nicht nach
		  if(i == firemode) return;

		  // Schussmodus umstellen
		  firemode=i;
		  stopauto=false;
		  shooting = false;
		  //OnEmpty();
		  if(GetFMData(FM_Auto)) OnAutoStop(firemode);
		  ratecount = GetFMData(FM_AmmoRate, i);

		  if(GetAkimbo()) GetAkimbo()->~SetFireMode(i);

		  // Helpmessage
		  if(GetUser())
			HelpMessage(GetUser()->GetOwner(),"$FireModeChanged$",GetUser(),GetFMData(FM_Name),GetFMData(FM_AmmoID));

		return 1;
	}
}



public func Empty()    // Waffe ausleeren
{
	  // Laden wir nach? Abbrechen.
	  if(IsReloading()) RemoveEffect("Reload", this());
	  if(IsRecharging()) RemoveEffect("Recharge", this());

	  if(!(this->~FeedUserAmmo()))
	  {
		  // Munitionsart wo raus muss
		  var ammoid = GetFMData(FM_AmmoID);
		  // Zu schiebende Munitionsmenge
		  var ammoamount = GetAmmo(ammoid, this());
		  // Clonk wiedergeben
		  DoAmmo(ammoid, ammoamount, GetUser());
		  // Hier entfernen
		  DoAmmo(ammoid, -ammoamount, this());
	  }
}

private func Reloaded(caller)   // Waffe nachgeladen
{

	  // Munitionsart wo rein muss
	  var ammoid = GetFMData(FM_AmmoID);
	  var ammoamount = MaxReloadAmount(caller);

	  // zwischendurch Ammo zuviel ammo verbraucht? Fehlschlag.
	  if(!CheckAmmo(ammoid,ammoamount,caller))
		return(false);

	  if(!(this->~FeedUserAmmo()))
	  {
		  // Hier einfügen
		  DoAmmo(ammoid, ammoamount, this());
		  // Dem Clonk abziehen
		  DoAmmo(ammoid, -ammoamount, caller);

		  HelpMessage(caller->GetOwner(),"$Reloaded$",caller,ammoamount,ammoid);
	  }

	  // Callback
	  OnReloaded(firemode);
}

public func GetAccuracy( object pObj )
{
	var accuracy;

	accuracy = CrewGetAccuracy( pObj );
	if(!accuracy) accuracy = 7;

	return 10-accuracy;
}


private func Recharge( dummy, timeoverride )    // Wartezeit zwischen zwei SchÃ¼ssen
{
	var rechargetime = GetFMData(FM_Recharge)*(1+MOD_DoubleRecharge());
	if( timeoverride) rechargetime = timeoverride;

	AddEffect("Recharge", this(), 1, 1+Max(1, rechargetime), this);

	// andere Waffen sollen bei Inventarwechsel nicht sofort schie�en k�nnen
	// Doppelte Ladezeit sollte sinnvoll sein
	if(Contained())
	if(Contents(0,Contained()) == this )
	{
		for(var i = 1, obj; i<ContentsCount(0,Contained()); i++)
		{
			obj = Contents(i,Contained());
			if(obj->~IsWeapon())
				obj->~Recharge(0,rechargetime*2);
		}
	}

}


public func FxRechargeStop(object pTarget, int iNumber, int iReason, bool fTemp) {
  // Waffenträger weg?
  if(!GetUser()) return(0);
  if(GetUser()->~IsWeapon()) return(0);
  if(!GetAlive(GetUser()) && GetCategory(GetUser())&C4D_Living) return(0);
  // automatisch weiterschießen, mit JnR-Control auch bei normalen Waffen
  if(GetFMData(FM_Auto) || GetPlrCoreJumpAndRunControl(pTarget->GetController())) {
    // ... außer wenn abgebrochen oder keine Munition mehr (!)
    if(stopauto || (GetUser()->Contents() != this() && GetUser() == Contained(this())) || !(GetUser()->~ReadyToFire()) || !CheckAmmo(GetFMData(FM_AmmoID), GetFMData(FM_AmmoUsage))) {
      // Callback bei AutoFire
      if(GetFMData(FM_Auto))
        OnAutoStop(firemode);

      stopauto = false;
      shooting = false;
    }
    else {
      Shoot(pTarget);
    }
  }
}
