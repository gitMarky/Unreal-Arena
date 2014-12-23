/*-- Pistole --*/

#strict 2

#include WEPN

public func KillMessage(){ return $KillMsg$; }

public func FeedUserAmmo(){ return true; }
public func AmmoPackID(){ return ID_UT99_Enforcer_Ammo;}

// Anzeige in der Hand
public func HandSize() { return 800; }
public func HandX()    { return 5500; }
public func HandY()    { return -1000; }

public func CanAkimbo(){ return true; }

public func AkimboX()    { return 5500 ; }
public func AkimboY()    { return -3000; }

public func BarrelYOffset() { return -2700-2700*IsAkimbo(); }

public func FMData1(int data)
{
  if(data == FM_Name)      return "$Standard$";
  if(data == FM_AmmoID)    return ID_Ammo_Pistol;
  if(data == FM_AmmoLoad)  return 30;

  if(data == FM_Recharge)  return 15;
  if(data == FM_Damage)    return 15;
  if(data == FM_Accuracy)	return 1;
  if(data == FM_Ballistic)  return 5;

  if(data == FM_ProjSpeed)	return 180;
  if(data == FM_ProjRange)	return 600;
  if(data == FM_ProjSize)	return 2;
  if(data == FM_ProjTrail)	return 100;
  if(data == FM_ProjSound)	return "enf-fire1";
  if(data == FM_ProjEffects)return true;

  return(Default(data));
}

public func FMData2(int data)
{
  if(data == FM_Recharge)  return 10;
  if(data == FM_Damage)    return 15;
  if(data == FM_Accuracy)	return 3;
  return FMData1( data );
}

public func BotData1(int data)
{
  if(data == BOT_Range)    return 120;
  if(data == BOT_Power)    return BOT_Power_1;
  return(Default(data));
}

public func BotData2(int data)
{
  if(data == BOT_Range)    return 50;
  return(BotData1(data));
}

public func Fire_Effects( int iMode, object user, int angle, int x, int y, bool bLaunch )
{
	  // Effekte
	  MuzzleFlash(40,user,x,y,angle);
	  var dir = GetDir(user)*2-1;
	  BulletCasing(dir*6,3,-dir*5,-20,5);
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
		ydir =  -Cos( angle -Random(2) +Random(accu) - accu/2, range_y );
	}
	else
	{
		xdir =  +Sin( angle +Random(accu) - accu/2, range_x );
		ydir =  -Cos( angle -Random(3) +Random(accu) - accu/2, range_y );
	}

	ammo->~LaunchSpeed(xdir, ydir);
}
