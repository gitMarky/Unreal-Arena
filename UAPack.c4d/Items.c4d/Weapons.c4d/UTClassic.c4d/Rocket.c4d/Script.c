/*-- Pistole --*/

#strict

#include WEPN

public func KillMessage(){ return $KillMsg$; }

public func FeedUserAmmo(){ return true; }
public func AmmoPackID(){ return ID_UT99_Rocket_Ammo;}

// Anzeige in der Hand
public func HandSize() { return 800; }
public func HandX()    { return 5500; }
public func HandY()    { return -1000; }

public func AkimboX()    { return 5500; }
public func AkimboY()    { return -3000; }

public func BarrelYOffset() { return -2700-2700*IsAkimbo(); }

public func GetHUDOffsetX(){ return 0; }
public func GetHUDOffsetY(){ return 15000; }

public func FMData1(int data)
{
  if(data == FM_Name)      return "$Standard$";
  if(data == FM_AmmoID)    return ID_Ammo_Missile;
  if(data == FM_AmmoLoad)  return 6;

  if(data == FM_Recharge)  return 32; //20;
  if(data == FM_Condition) return true;

  if(data == FM_Damage)    return 0;

  if(data == FM_ProjID)		return S_U7;
  if(data == FM_ProjSound)	return "RocketFire";
  if(data == FM_ProjSpeed)	return 120;
  if(data == FM_ProjSize)	return 4;
  if(data == FM_ProjTrail)	return 1;
  if(data == FM_Ballistic)	return 0;
  if(data == FM_ProjEffects)return true;


  return Default(data);
}

public func FMData2(int data)
{
  if(data == FM_Name)      return "$Grenade$";
  if(data == FM_AmmoID)    return ID_Ammo_Missile;
  if(data == FM_AmmoLoad)  return 6;

  if(data == FM_Reload)    return 80;
  if(data == FM_Recharge)  return 32; //20;
  if(data == FM_Condition) return true;

  if(data == FM_Damage)    return 0;

  if(data == FM_ProjID)		return S_U7;
  if(data == FM_ProjSound)	return "RocketAltFire";
  if(data == FM_ProjSpeed)	return 60;
  if(data == FM_ProjSize)	return 4;
  if(data == FM_ProjTrail)	return 1;
  if(data == FM_Ballistic)	return 1;
  if(data == FM_ProjEffects)return true;


  return Default(data);
}

public func FMData3( int data )
{
	  if(data == FM_Name)      return "$Charge$";
	  if(data == FM_AmmoID)    return ID_Ammo_Missile;
	  if(data == FM_AmmoLoad)  return 6;

	  if(data == FM_Recharge)  return 32; //20;
	  if(data == FM_Condition) return true;

	  if(data == FM_Damage)    return 0;
	  if(data == FM_Auto)		return true;

	  if(data == FM_Old)		return true;
	  return Default(data);
}
/*
public func BotData1(int data)
{
  if(data == BOT_Range)    return 500;
  if(data == BOT_Power)    return BOT_Power_3;

  return(Default(data));
}

public func BotData2(int data)
{
  if(data == BOT_Range)    return 200;
  if(data == BOT_Power)    return BOT_Power_3;

  return(Default(data));
}
*/

public func BotData1(int data)
{
  if(data == BOT_Range)    return 400;
  if(data == BOT_RangeMin) return 40;
  if(data == BOT_Power)    return BOT_Power_2;
  if(data == BOT_Rating)   return 15;
  if(data == BOT_Priority) return 30;
  if(data == BOT_Override) return true;

  return(Default(data));
}
public func BotData2(int data)
{
  if(data == BOT_Range)    return 90;
  return(BotData1(data));
}

public func Fire_BulletEx( int iMode, object &ammo, object user, int angle, int x, int y, bool bLaunch )
{
	if(iMode == 2)
	{
		  //ammo->SetGraphics("Grenade");
		  ammo->SetGrenade();
	}

	var range_x = 120;
	var range_y = 120;
	var xdir, ydir;
	var accu = GetAccuracy(user);

	if( iMode == 1 )
	{
		xdir =  +Sin( angle +Random(accu) - accu/2, range_x );
		ydir =  -Cos( angle -Random(2) +Random(accu) - accu/2, range_y );
	}
	else
	{
		range_x = 40 + Random(4)*10;
		range_y = 60;
		xdir =  +Sin( angle +Random(accu) - accu/2, range_x );
		ydir =  -Cos( angle -Random(3) +Random(accu) - accu/2, range_y );
	}
}


public func ControlSpecial3( object pCaller)
{
	if( firemode == 3 ) return( FireRockets() );

	local oldmode;
	oldmode = firemode;
	stopauto=false;
	SetFireMode(3);
	ControlThrow( pCaller );

	return true;
}

public func Fire3()
{
	local rocket_count;

	if( rocket_count >= 6 )
	{
		FireRockets();
	}
	else
	{
		rocket_count++;
		Sound("RocketLoad");
	}
}

public func ControlDig( object pCaller )
{
	if( firemode == 3 )
	{
		FireRockets();
		return true;
	}
	else
		return _inherited( pCaller );
}

public func FireRockets()
{
	SetFireMode( oldmode );
	while(rocket_count)
	{
		Schedule(Format("FireShot(%d)", oldmode ),rocket_count*1,0,this);
		rocket_count--;
	}

}


public func OnAutoStop( int iMode )
{
	if(iMode == 3 && rocket_count > 1 && Contained()) FireRockets();
}



public func UTBotAIUse( pBot, pTarget )
{
	var obj, angle;

		if(Inside( ObjectDistance( pBot, pTarget ), BotData2(BOT_RangeMin), BotData2(BOT_Range) ))
		{
			if(firemode != 2) SetFireMode(2);
		}
		else
		{
			if(firemode != 1) SetFireMode(1);
		}

	if( ObjectCount2( Find_ID(UBOT), Find_OCF(OCF_Living), Find_InRect(GetX(pTarget)-GetX()-40,GetY(pTarget)-GetY()-30,80,60) ) >=2 )
	{
		if( firemode != 3 ) return(ControlSpecial3( pBot ));

		if( ObjectDistance( pBot, pTarget ) < ( 20 * rocket_count ) ) FireRockets();

		return;
	}

	if( ObjectDistance( pBot, pTarget ) < 30 ) return;

	var angle = pBot->~UTBotAIAimAt( obj );

	pBot->~UTBotAIAimAt( obj, +Sin( angle, 100 ), -Cos( angle, 100 ) );// ControlCommand( "MoveTo", 0 , GetX(pBot), GetY(pBot));
}
