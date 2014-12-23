/*-- Pistole --*/

#strict

#include WEPN

public func KillMessage(){ return $KillMsg$; }

public func FeedUserAmmo(){ return true; }

public func AmmoPackID(){ return ID_UT99_Bio_Ammo;}

// Anzeige in der Hand
public func HandSize() { return 800; }
public func HandX()    { return 6000; }
public func HandY()    { return -3000; }

public func GetHUDOffsetX(){ return 0; }
public func GetHUDOffsetY(){ return -10000; }

public func GetAkimboHUDOffsetX(){ return 15000; }
public func GetAkimboHUDOffsetY(){ return -3000; }

public func AkimboX()    { return 2000; }
public func AkimboY()    { return 2000; }

public func BarrelYOffset() { return -6000-2700*IsAkimbo(); }

public func FMData1(int data)
{
  if(data == FM_Name)      return "$Standard$";
  if(data == FM_AmmoID)    return ID_Ammo_Bio;
  if(data == FM_AmmoLoad)  return 25;

  if(data == FM_Recharge)  return 10; //6;
  if(data == FM_Condition) return true;

  if(data == FM_Damage)    return 40; //12;
  if(data == FM_ProjSound) return "bio-fire1";
  if(data == FM_ProjCustom) return true;
  if(data == FM_Ballistic) 	return 1;

  return Default(data);
}

public func FMData2(int data)
{
  if(data == FM_Name)      return "$Blob$";
  if(data == FM_AmmoID)    return ID_Ammo_Bio;
  if(data == FM_AmmoLoad)  return 25;

  if(data == FM_Recharge)  return 16; //12;
  if(data == FM_Condition) return true;

  if(data == FM_Damage)    return 0; //12;
  if(data == FM_Auto)	   return true;
  if(data == FM_ProjCustom) return true;
  if(data == FM_Ballistic) 	return 1;

  return Default(data);
}


public func BotData1(int data)
{
  if(data == BOT_Range)    return 100;
  if(data == BOT_RangeMin) return 0;
  if(data == BOT_Power)    return BOT_Power_2;
  if(data == BOT_Rating)   return 12;
  if(data == BOT_Priority) return 20;
  if(data == BOT_Override) return "UseBioRifle";

  return(Default(data));
}
public func BotData2(int data)
{
	if(data == BOT_Power)    return BOT_Power_3;
	if(data == BOT_Range)    return 70;
	if(data == BOT_RangeMin) return 0;
	return(BotData1(data));
}

public func Fire_BulletEx( int iMode, object &ammo, object user, int angle, int x, int y, bool bLaunch )
{
	var range_x = 60 + Random(3)*10;
	var range_y = 60 + Random(3)*10;
	var xdir, ydir;
	var accu = GetAccuracy(user);

	if( iMode == 1 && bLaunch)
	{
		if(ammo) RemoveObject(ammo);
		  //var rad = 60 + Random(30);
		  var rad = 40 + Random(20);
		  //var xdir = Sin(angle,rad);
		  //var ydir = -Cos(angle,rad);
			xdir =  +Sin( angle +Random(accu) - accu/2, range_x );
			ydir =  -Cos( angle-Random(4) +Random(accu) - accu/2, range_y );
		  // Create & launch
		  ammo=CreateObject(SLST, x+xdir/10,y+ydir/10, GetController(user));
		  ammo->Launch(xdir/*+GetXDir(user)/2*/,ydir/*+GetYDir(user)/2*/,GetFMData(FM_Damage, 1)*Max(1,iSlimeLoaded),DMG_Bio,5+iSlimeLoaded);

		  iSlimeLoaded = 0;
		  SetFireMode(1);

	}
	if( iMode == 2 && bLaunch)
	{

		local iSlimeLoaded;

		iSlimeLoaded++;

		if(iSlimeLoaded >= 9)
		{
			SetFireMode(1);
			Fire_BulletEx( 1, ammo, user, angle, x, y, bLaunch );
			stopauto=true;

		}
		else
			if(ammo) RemoveObject(ammo);

	}

}

public func OnAutoStart( int iMode )
{
  //Sound("Slime",0,0,0,0,1);
}

public func OnAutoStop( int iMode )
{
  //Sound("SlimeStop",0,0,0,0,-1);

  if(iSlimeLoaded) FireShot(1);
}
