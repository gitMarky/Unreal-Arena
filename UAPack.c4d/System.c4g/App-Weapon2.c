/*-- Waffen-Script --*/

// Standardisiert ein wenig die Waffenfunktion aus Hazard

#strict 2
#appendto WEPN

private func Shoot(object caller)
{
	// Feuern mit Feuermodus
	//User da? :S
	if(!GetUser())
		return (stopauto=true);

	var ammoid = GetFMData(FM_AmmoID);
	//soll er zielen, zielt aber nicht?
	if(!(GetUser()->~IsAiming()) && GetFMData(FM_Aim)>0 && !(GetUser()->~AimOverride()))
		return(stopauto=true); //abbrechen
	// user hat sich irgendwo reinbewegt
	if(GetUser()->Contained() && GetUser() == Contained())
		return(stopauto=true);
	// Feuern...
	//Call(Format("Fire%d",firemode));
	FireShot( firemode );
	if(GetFMData(FM_Auto))
		shooting = true;

	if(!GetUser()) return;

	// Munition abziehen
	if(ratecount == 1)
	{
		var muni = GetFMData(FM_AmmoUsage);
		DoAmmo(ammoid,-muni);
		ratecount = GetFMData(FM_AmmoRate);
	}
	else { --ratecount; }

	// Pause nach einem Schuss
	if(GetAmmo(ammoid)) Recharge();
	else {
		shooting = false;
		OnEmpty();
		if(GetFMData(FM_Auto)) OnAutoStop(firemode);

		// Automatisch nachladen, wenn die Feuertaste nach 5 Frames noch gedrückt ist
		if(GetPlrCoreJumpAndRunControl(GetUser()->GetController()))
			if(!IsReloading())
				ScheduleCall(this(), "Reload", 5);
	}
	// HZCK soll Munition doch bitte neu anschauen
	GetUser()->~UpdateCharge();
}

public func FireShot( int iMode )		// Projektilschuss
{
	var crew = GetUser();
	if (crew) crew->~SetFixedDir(crew->~GetDir(crew));


	if(GetFMData( FM_Old,iMode))
		Call(Format("Fire%d",iMode));
	else
	{
		var user = GetUser();
		var iAcc = GetFMData( FM_Accuracy, iMode)+GetAccuracy(user)/2;
		var angle = user->AimAngle(GetFMData(FM_AimAngle,iMode)) + RandomX(-iAcc,+iAcc);
		var x,y; user->WeaponEnd(x,y,IsAkimbo());

		Fire_Bullet( iMode, user, angle, x, y);
		Fire_Effects( iMode, user, angle, x, y );
	}
}

public func Fire_Bullet( int iMode, object user, int angle, int x, int y )
{
	var ammo = CreateObject(GetFMData(FM_ProjID, iMode),x,y+3,GetController(user));

	if(GetFMData( FM_ProjCustom, iMode ))
		Fire_BulletEx(iMode,ammo,user,angle,x,y,true);
	else
		ammo->Launch(angle,GetFMData(FM_ProjSpeed, iMode),GetFMData(FM_ProjRange,iMode),GetFMData( FM_ProjSize, iMode), GetFMData( FM_ProjTrail, iMode),GetFMData(FM_Damage, iMode),GetFMData(FM_DamageType, iMode));

	if( GetFMData( FM_ProjEffects, iMode) )
		Fire_BulletEx(iMode,ammo,user,angle,x,y,false);

	if(GetFMData(FM_Ballistic, iMode) && ammo)
	{
		//ammo->SetBallistic();
		ScheduleCall(ammo,"SetBallistic",GetFMData(FM_Ballistic, iMode));
	}

	var sound = GetFMData( FM_ProjSound, iMode);
	if( sound && ammo )
		ammo->Sound( sound);
}

public func Fire_Effects( int iMode, object user, int angle, int x, int y )
{
}

public func Fire_BulletEx( int iMode, object &ammo, object user, int angle, int x, int y, bool bLaunch )
{
}
