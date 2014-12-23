/*-- Pistole --*/

#strict 2

#include WEPN

public func FeedUserAmmo(){ return true; }
public func AmmoPackID(){ return ID_UT99_Pulse_Ammo;}

public func KillMessage(){ return $KillMsg$; }

// Anzeige in der Hand
public func HandSize() { return 800; }
public func HandX()    { return 5500; }
public func HandY()    { return -1000; }

public func AkimboX()    { return 5500; }
public func AkimboY()    { return -3000; }

public func BarrelYOffset() { return -500-2700*IsAkimbo(); }

public func FMData1(int data)
{
  if(data == FM_Name)      return "$Standard$";
  if(data == FM_AmmoID)    return ID_Ammo_Pulse;
  if(data == FM_AmmoLoad)  return 60;

  if(data == FM_Reload)    return 80;
  if(data == FM_Recharge)  return 6;
  if(data == FM_Condition) return true;
  if(data == FM_Auto)	   return true;

  if(data == FM_Damage)    return 10;
  if(data == FM_DamageType)	return DMG_Projectile|DMG_Energy;
  if(data == FM_Accuracy)	return 3;
  if(data == FM_ProjID)		return S_U3;
  if(data == FM_ProjSpeed)	return 140; //180;
  if(data == FM_ProjSize)	return 1;//7;
  if(data == FM_ProjTrail)	return 30;
  if(data == FM_Ballistic)  return 0;
  if(data == FM_ProjEffects) return true;

  return(Default(data));
}

public func FMData2(int data)
{
  if(data == FM_Name)      return "$Beam$";
  if(data == FM_AmmoID)    return ID_Ammo_Pulse;
  if(data == FM_AmmoLoad)  return 30;

  if(data == FM_Recharge)  return 1;
  if(data == FM_Condition) return true;
  if(data == FM_AmmoUsage)  return 1 ;
  if(data == FM_AmmoRate)   return 8; // 1 ammo je 8 frames
  if(data == FM_Auto)	   return true;
  if(data == FM_Old)	   return true;

  if(data == FM_Damage)    return 1;
  if(data == FM_Hitscan)	return true;

  return(Default(data));
}


public func BotData1(int data)
{
  if(data == BOT_Range)    return 220;
  if(data == BOT_RangeMin) return 150;
  if(data == BOT_Power)    return BOT_Power_2;
  if(data == BOT_Override) return true;

  return(Default(data));
}
public func BotData2(int data)
{
  if(data == BOT_Range)    return 155;
  if(data == BOT_RangeMin) return 10;
  if(data == BOT_Power)    return BOT_Power_3;
  return(BotData1(data));
}

local burstfire;

public func Fire_Effects( int iMode, object user, int angle, int x, int y, bool bLaunch )
{
	if(iMode == 1)
	{
	  // Effekte
	  MuzzleFlash(50,user,x,y,angle,RGBa(0,250,0,0));
	  burstfire++;
	  if(burstfire >= 3)
	  {
		  burstfire = 0;
		  StopAutoFire();
	  }
	  if(burstfire == 1)
	  {
		  Sound("PulseFire");
	  }
	}
}

public func Fire2()
{
	local beam, iLastAngle;

	var user = GetUser();
	//var iAcc = GetFMData( FM_Accuracy, 2)+GetAccuracy(user)/2;
	var iAngle = user->AimAngle(GetFMData(FM_AimAngle,2)); //+ RandomX(-iAcc,+iAcc);
	var x,y; user->WeaponEnd(x,y,IsAkimbo());

	if(!beam)
	{

		beam = CreateObject( _LSR,x,y,GetOwner(user) );

		beam->~Set( iAngle, 8, 150, 0, this, user, 1, RGB( 0,255,0));
		beam->~SetUser( user );
		beam->~SetContinuous( 1 );
	}
	else if(iAngle != iLastAngle)
	{
		beam->~Set( iAngle, 8, 150, 0, this, user, 1, RGB( 0,255,0));
		//beam->~Set( iAngle, 8, 160, 20, this, 0, 6, RGB( 0,255,0));
	}

	iLastAngle = iAngle;
}


public func OnAutoStart( int iMode )
{
	if(iMode == 2)
		Sound("PulseBolt",0,0,0,0,1);
}

public func OnAutoStop()
{
	Sound("PulseBolt",0,0,0,0,-1);
	if(beam)
	{
		RemoveObject(beam);
		SetFireMode(1);
	}
}


public func LaserStrike( pObj )
{
	if(pObj)
	{
		DoDmg(2,DMG_Energy,pObj);
		return true;
	}
}


public func UTBotAIUse( pBot, pTarget )
{
	if( Inside( ObjectDistance( pTarget, pBot ), 0 , 90 /*+ObjectCount(_MGD)*3*/ ) )
	{
		if(!beam) ControlDig( pBot );

		pBot->~UTBotAIAimAt( pTarget );//FireWeaponSek( pBot->CalcAimAngle( pTarget, this() ) );
	}
	else
	{
		if(beam) ControlDig( pBot );

		pBot->~UTBotAIAimAt( pTarget );//pBot->FireWeaponPri( pBot->CalcAimAngle( pTarget, this() ) );
		if(!IsShooting()) pBot->~UTBotAIFire(this,1);
	}
}


public func Fire_BulletEx( int iMode, object &ammo, object user, int angle, int x, int y, bool bLaunch )
{
	if( iMode == 1 )
	{
		var range_x = 150 + Random(11)*10;
		var range_y = 150 + Random(11)*10;
		var xdir, ydir;
		var accu = GetAccuracy(user);

		xdir =  +Sin( angle +Random(accu) - accu/2, range_x );
		ydir =  -Cos( angle -Random(2) +Random(accu) - accu/2, range_y );
		ammo->~LaunchSpeed(xdir, ydir);
	}
}

