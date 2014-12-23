/*-- Ripper --*/

#strict 2

#include WEPN

public func KillMessage(){ return $KillMsg$; }

public func FeedUserAmmo(){ return true; }
public func AmmoPackID(){ return ID_UT99_Ripper_Ammo;}

public func HandSize() { return 800;  }
public func HandX()    { return 8000; }
public func HandY()    { return -2500;  }
public func BarrelYOffset(){return -1000;}

public func GetAkimboHUDOffsetX(){ return 5000; }
public func GetAkimboHUDOffsetY(){ return -10000; }

public func AkimboX()    { return 2000; }
public func AkimboY()    { return 2000; }

public func FMData1(int data)
{
  if(data == FM_Name)      return "$Standard$";
  if(data == FM_AmmoID)    return ID_Ammo_Ripper;
  if(data == FM_AmmoLoad)  return 15;
  if(data == FM_AmmoUsage) return 1;

  if(data == FM_Reload)    return 50;
  if(data == FM_Recharge)  return 13; //20;

  if(data == FM_Damage)    return 30; //18;
  if(data == FM_Accuracy)	return 1;
  if(data == FM_DamageType) return DMG_Projectile|DMG_Headshot;

  if(data == FM_ProjID)		return S_U4;
  if(data == FM_ProjSound)	return "RipperFire";
  if(data == FM_ProjSpeed)	return 180;
  if(data == FM_ProjSize)	return 1;
  if(data == FM_ProjTrail)	return 1;
  if(data == FM_ProjEffects) return 1;

  return(Default(data));
}

public func FMData2(int data)
{
  if(data == FM_Name)      return "$Explosive$";
  if(data == FM_AmmoID)    return ID_Ammo_Ripper;
  if(data == FM_AmmoLoad)  return 15;
  if(data == FM_AmmoUsage) return 1;

  if(data == FM_Recharge)  return 24; //28;

  if(data == FM_Damage)    return 0;
  if(data == FM_Accuracy)	return 1;

  if(data == FM_ProjID)		return S_U4;
  if(data == FM_ProjSound)	return "RipperAltFire";
  if(data == FM_ProjSpeed)	return 180;
  if(data == FM_ProjSize)	return 1;
  if(data == FM_ProjTrail)	return 1;

  if(data == FM_ProjEffects)return true;


  return(Default(data));
}

public func BotData1(int data)
{
  if(data == BOT_Range)    return 150;
  if(data == BOT_RangeMin) return 0;
  if(data == BOT_Power)    return BOT_Power_2;
  if(data == BOT_Rating)   return 16;
  if(data == BOT_Priority) return 30;

  return(Default(data));
}
public func BotData2(int data)
{
  if(data == BOT_Range)    return 80;
  if(data == BOT_RangeMin) return 20;
  return(BotData1(data));
}

/*
public func Fire1()    // Projektilschuss
{ 
  ammo->Launch(angle,180,600,1,1,GetFMData(FM_Damage, 1),GetFMData(FM_DamageType, 1),0,0,3);

}

public func Fire2()    // Projektilschuss
{
  ammo->Launch(angle,180,600,1,1,GetFMData(FM_Damage, 2),GetFMData(FM_DamageType,2));

}
*/
// Soundeffekte

public func OnReload()
{
  Sound("PumpgunLoad");
}

public func Fire_BulletEx( int iMode, object &ammo, object user, int angle, int x, int y, bool bLaunch )
{
	var range = 170;
	var xdir, ydir;
	var accu = GetAccuracy(user);

		xdir = +Sin( angle +Random(accu) - accu/2, range );
		ydir = -Cos( angle -Random(2) +Random(accu) - accu/2, range );

	if(iMode == 2)
	{
		  ammo->SetSecondary();
	}
}
