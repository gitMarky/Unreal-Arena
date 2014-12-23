/*-- Minigun --*/

#strict

#include WEPN

public func KillMessage(){ return $KillMsg$; }

public func FeedUserAmmo(){ return true; }
public func AmmoPackID(){ return ID_UT99_Minigun_Ammo;}

// Anzeige in der Hand
public func HandSize() { return 800 ;  }
public func HandX()    { return 8500; }
public func HandY()    { return -3000; } // hier heißt - nach unten

public func AkimboX()    { return 2000; }
public func AkimboY()    { return 4000; }


public func BarrelYOffset(){return -2000; } // hier heißt - nach oben

//public func GetHUDOffsetX(){ return -35000; }
//public func GetHUDOffsetY(){ return 10000; }

public func GetHUDOffsetX(){ return -35000+35000; }
public func GetHUDOffsetY(){ return 10000+15000; }

public func GetAkimboHUDOffsetX(){ return 10000; }
public func GetAkimboHUDOffsetY(){ return -10000; }


// Daten für ersten Feuermodus
public func FMData1(int data)
{
  if(data == FM_Name)       return "$Standard$";
  if(data == FM_AmmoID)     return ID_Ammo_Pistol;
  if(data == FM_AmmoLoad)   return 80;

  if(data == FM_Reload)     return 260;
  if(data == FM_Recharge)   return 4; //5;

  if(data == FM_Auto)       return true;
  if(data == FM_Aim)        return -1;

  if(data == FM_Condition) return true;

  if(data == FM_Damage)     return 6;
  if(data == FM_Accuracy)	return 2;
  if(data == FM_ProjSpeed)  return 210;
  if(data == FM_ProjSize)	return 1;
  if(data == FM_ProjTrail)	return 60;
  if(data == FM_ProjEffects ) return false;

  return Default(data);
}

// Daten für ersten Feuermodus
public func FMData2(int data)
{
  if(data == FM_Name)       return "$Secondary$";
  if(data == FM_AmmoID)     return ID_Ammo_Pistol;
  if(data == FM_AmmoLoad)   return 80;

  if(data == FM_Reload)     return 260;
  if(data == FM_Recharge)   return 2;

  if(data == FM_Auto)       return true;
  if(data == FM_Aim)        return -1;

  if(data == FM_Condition) return true;

  if(data == FM_Damage)     return 6;
  if(data == FM_Accuracy)	return 6;
  if(data == FM_ProjSpeed)  return 210;
  if(data == FM_ProjSize)	return 1;
  if(data == FM_ProjTrail)	return 60;
  if(data == FM_ProjEffects ) return false;

  return Default(data);
}

public func BotData1(int data)
{
  if(data == BOT_Range)    return 220;
  if(data == BOT_RangeMin) return 0;
  if(data == BOT_Power)    return BOT_Power_2;
  if(data == BOT_Rating)   return 14;
  if(data == BOT_Priority) return 50;

  return(Default(data));
}
public func BotData2(int data)
{
  if(data == BOT_Range)    return 80;
  return(BotData1(data));
}

public func Fire_Effects( int iMode, object user, int angle, int x, int y )
{
	  // Effekte
	  MuzzleFlash(RandomX(20,50),user,x,y,angle);

	  //user->~WeaponBegin(x,y);
	  var dir = GetDir(user)*2-1;
	  BulletCasing(6*x/10,6*y/10,-dir*Cos(angle-35*dir,40+Random(20)),-dir*Sin(angle-35*dir,40+Random(20)),5);
}

public func OnAutoStart( int iMode )
{
	var szSnd = "M1RegFire";
	if(iMode) szSnd = "M1AltFire";

	Sound(szSnd,0,0,0,0,1);
  //Sound("MiniTurn",0,0,0,0,1);
}

public func OnAutoStop( int iMode )
{
	var szSnd = "M1RegFire";
	if(iMode) szSnd = "M1AltFire";

	Sound(szSnd,0,0,0,0,-1);
	//Sound("M1RegFire",0,0,0,0,-1);
  //Sound("MiniTurn",0,0,0,0,-1);
}


public func Fire_BulletEx( int iMode, object &ammo, object user, int angle, int x, int y, bool bLaunch )
{
	var range_x = 150 + Random(11)*10;
	var range_y = 150 + Random(11)*10;
	var xdir, ydir;
	var accu = GetAccuracy(user);

	if( iMode == 1 )
	{
		xdir =  +Sin( angle +Random(accu) - accu/2, range_x );
		ydir =  -Cos( angle +1 -Random(4) +Random(accu) - accu/2, range_y );
	}
	else
	{
		xdir =  +Sin( angle +Random(accu) - accu/2, range_x );
		ydir =  -Cos( angle -Random(2) +Random(accu) - accu/2, range_y );
	}

	ammo->~LaunchSpeed(xdir, ydir);
}
