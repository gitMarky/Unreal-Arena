/*-- Partikelkanone --*/

#strict 2

#include WEPN

local laser; //der Laserstrahl



public func KillMessage(){ return $KillMsg$; }

public func FeedUserAmmo(){ return true; }
public func AmmoPackID(){ return ID_UT99_ASMD_Ammo;}

// Anzeige in der Hand
public func HandSize() { return 800; }
public func HandX()    { return 6000; }
public func HandY()    { return -500; } // - heißt runter

public func GetHUDOffsetX(){ return 0; }
public func GetHUDOffsetY(){ return 15000; }

public func GetAkimboHUDOffsetX(){ return 10000; }
public func GetAkimboHUDOffsetY(){ return -6000; }

public func AkimboX()    { return 2000; }
public func AkimboY()    { return 2000; }

public func BarrelYOffset() { return -1000-2700*IsAkimbo(); }


//RAILGUN!!!!!
func FMData1(int data)
{
  if(data == FM_Name)       return "$Railgun$";

  if(data == FM_AmmoID)     return ID_Ammo_Shock;
  if(data == FM_AmmoLoad)   return 20;

  if(data == FM_Recharge)   return 26; //24;
  if(data == FM_Damage)     return 40; //20;
  if(data == FM_DamageType)	return DMG_Energy;

  if(data == FM_AimAngle)	return 5;
  if(data == FM_Accuracy)	return 2;

  if(data == FM_ProjID)		return S_U2;
  if(data == FM_ProjCustom) return true;
  if(data == FM_Hitscan)	return true;
  //if(data == FM_ProjEffects)return true;
  //if(data == FM_ProjSpeed)	return 180;
  //if(data == FM_ProjRange)	return 600;
  //if(data == FM_ProjSize)	return 2;
  //if(data == FM_ProjTrail)	return 100;
  //if(data == FM_ProjSound)	return "enf-fire1";

  return Default(data);
}


//In UT Classic schießt das Ding eigentlich lange Projektile
//Sieht aber nicht so schön aus
func FMData3(int data)
{
  if(data == FM_Name)       return "$Railgun$";

  if(data == FM_AmmoID)     return ID_Ammo_Shock;
  if(data == FM_AmmoLoad)   return 20;

  if(data == FM_Recharge)   return 24;
  if(data == FM_Damage)     return 40; //20;
  if(data == FM_DamageType)	return DMG_Energy;

  if(data == FM_AimAngle)	return 5;
  if(data == FM_Accuracy)	return 2;

  if(data == FM_ProjID)		return S_M2;
  //if(data == FM_ProjCustom) return true;
  //if(data == FM_ProjEffects)return true;
  if(data == FM_ProjSpeed)	return 300;
  if(data == FM_ProjRange)	return 9999;
  if(data == FM_ProjSize)	return 7;
  if(data == FM_ProjTrail)	return 400;
  if(data == FM_ProjSound)	return "shk-fire1";

  return Default(data);
}

public func BotData1(int data)
{
  if(data == BOT_Range)    return 400+BotSkill()*20;
  if(data == BOT_DmgType)  return DMG_Energy;
  if(data == BOT_RangeMin) return 10;
  if(data == BOT_Power)    return BOT_Power_2;
  if(data == BOT_Rating)   return 15;
  if(data == BOT_Priority) return 30;
  if(data == BOT_Override) return true;

  return(Default(data));
}
public func BotData2(int data)
{
  if(data == BOT_RangeMin)    return 50;
  if(data == BOT_Power)		  return BOT_Power_3;
  return(BotData1(data));
}

//RAILGUN!!!!!
func FMData2(int data)
{
  if(data == FM_Name)       return "$Balls$";

  if(data == FM_AmmoID)     return ID_Ammo_Shock;
  if(data == FM_AmmoLoad)   return 20;

  if(data == FM_Recharge)  return 18; //24;
  if(data == FM_Damage)    return 20;
  if(data == FM_Accuracy)	return 1;
  if(data == FM_Ballistic)	return 0;

  if(data == FM_ProjID)		return S_U2;
  if(data == FM_ProjCustom)	return true;
  /*if(data == FM_ProjSpeed)	return 60;
  if(data == FM_ProjSize)	return 16;
  if(data == FM_ProjTrail)	return 1;
  if(data == FM_ProjSound)	return "shk-sek1";
  if(data == FM_Ballistic)	return false;
  if(data == FM_ProjEffects)return true;*/

  return Default(data);
}

public func Fire_BulletEx( int iMode, object &ammo, object user, int angle, int x, int y, bool bLaunch )
{
	if( iMode == 1 && bLaunch)
	{
		  //var ammo = CreateObject(S_U2,x,y,GetController(user));
		  //ammo->Sound("shk-fire1");
		  ammo->LaunchLaser(user,angle,GetFMData(FM_Damage, 1),GetFMData(FM_DamageType,1));

		  var boom = CreateObject(TIM1,0,0,-1);
		  boom->~Sound("shk-fire1");
		  Schedule("RemoveObject()",40,0,boom);

		  //MuzzleFlash(40,user,x,y,angle,RGBa(64,64,255,0));
	}
	if( iMode == 2 && bLaunch)
	{
		  ammo->Launch(angle,60,9999,16,1,GetFMData(FM_Damage, 2),GetFMData(FM_DamageType, 2),10);
		  ammo->Sound("shk-sek1");
	}

}

public func Fire_Effects( int iMode, object user, int angle, int x, int y, bool bLaunch )
{
	MuzzleFlash(40,user,x,y,angle,RGBa(64,64,255,0));
	if( iMode == 2)
	{
		  SetFireMode(1);
	}
}


public func UTBotAIUse( pBot, pTarget )
{
	var obj, angle;

	for( obj in FindObjects( Find_ID( S_U2 ), Find_Action("Travel")))
	{
		if( ObjectCount2( Find_OCF(OCF_Living), Find_InRect(GetX(obj)-GetX()-40,GetY(obj)-GetY()-40,60,60) ) <0 ) continue;

		pBot->~UTBotAIAimAt(obj);
		pBot->~UTBotAIFire(this,1);

		return;
	}

	obj = pTarget;

	if( ObjectDistance( pBot, pTarget ) > 160+( BotSkill(20)*10 ) ) return;

	pBot->~UTBotAIAimAt( obj );

	if(( ObjectCount( UA6J, GetX(pTarget)-GetX()-60, GetY(pTarget)-GetY()-60, 120,120 ) >= 3 ) && ( ObjectDistance( pBot, pTarget ) > 60 ))
	{
		pBot->~UTBotAIFire(this,2);
	}
	else
	{
		pBot->~UTBotAIFire(this,1);
	}

}

