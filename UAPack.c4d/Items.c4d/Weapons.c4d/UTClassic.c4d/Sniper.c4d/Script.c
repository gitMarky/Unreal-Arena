/*-- Pistole --*/

#strict 2

#include WEPN

public func KillMessage(){ return $KillMsg$; }

public func FeedUserAmmo(){ return true; }
public func AmmoPackID(){ return ID_UT99_Sniper_Ammo;}

// Anzeige in der Hand
public func HandSize() { return 800; }
public func HandX()    { return 8500; }
public func HandY()    { return -1000; }

public func AkimboX()    { return 5500; }
public func AkimboY()    { return -3000; }

public func BarrelYOffset() { return -2700-2700*IsAkimbo(); }

//public func GetHUDOffsetX(){ return -40000; }
//public func GetHUDOffsetY(){ return 5000; }

public func GetHUDOffsetX(){ return -40000+40000; }
public func GetHUDOffsetY(){ return 5000+3000; }

public func GetAkimboHUDOffsetX(){ return 10000; }
public func GetAkimboHUDOffsetY(){ return -10000; }


public func FMData1(int data)
{
  if(data == FM_Name)      return "$Standard$";
  if(data == FM_AmmoID)    return ID_Ammo_Sniper;
  if(data == FM_AmmoLoad)  return 8;

  if(data == FM_Recharge)  return 21; //30;
  if(data == FM_Condition) return true;

  if(data == FM_Damage)    return 45; //18;
  if(data == FM_DamageType) return DMG_Projectile|DMG_Headshot;
  //if(data == FM_ProjSound)  return "snp-fire1";
  if(data == FM_Accuracy) 	return 5;
  if(data == FM_ProjSpeed)  return 200;
  if(data == FM_ProjSize)   return 2;
  if(data == FM_ProjTrail)  return 500;
  if(data == FM_ProjCustom) return true;
  if(data == FM_ProjID)		return S_U8;
  if(data == FM_Hitscan)	return true;

  return(Default(data));
}

public func FMData2(int data)
{
  if(data == FM_Name)      return "$Aim$";
  if(data == FM_AmmoID)    return ID_Ammo_Sniper;
  if(data == FM_AmmoLoad)  return 8;

  if(data == FM_Recharge)  return 30;
  if(data == FM_Condition) return true;

  if(data == FM_Damage)    return 45; // 18;
  if(data == FM_DamageType) return DMG_Projectile|DMG_Headshot;
  if(data == FM_ProjSound)  return "snp-fire1";
  if(data == FM_Accuracy) 	return 0;
  if(data == FM_ProjSpeed)  return 500;
  if(data == FM_ProjSize)   return 2;
  if(data == FM_ProjTrail)  return 500;
  if(data == FM_ProjCustom) return true;
  if(data == FM_ProjID)		return S_U8;
  if(data == FM_Hitscan)	return true;
  if(data == FM_SightBonus) return 750;

  return(Default(data));
}

public func BotData1(int data)
{
  if(data == BOT_Range)    return 300;
  if(data == BOT_RangeMin) return 0;
  if(data == BOT_Power)    return BOT_Power_2;
  if(data == BOT_Rating)   return 18;
  //if(data == BOT_Rating)   return 10;
  if(data == BOT_Priority) return 30;
  if(data == BOT_Override) return true;

  return(Default(data));
}
public func BotData2(int data)
{
  if(data == BOT_Range)    return 1500;
  if(data == BOT_Priority) return 80;
  return(BotData1(data));
}

/*
public func Fire1()    // Projektilschuss
{ 
  var user = GetUser();
  var angle = user->AimAngle(20) + RandomX(-1,+1);
  var x,y; user->WeaponEnd(x,y);
  var ammo = CreateObject(SHT_,x,y,GetController(user));
  ammo->Launch(angle,500,9999,2,500,GetFMData(FM_Damage, 1),GetFMData(FM_Damage, 1));
  //ammo->SetBallistic();

  // Effekte
  MuzzleFlash(40,user,x,y,angle);
  var dir = GetDir(user)*2-1;
  BulletCasing(dir*6,3,-dir*5,-20,5);

  // Sound
  Sound("snp-fire1",0,ammo);
}

*/

public func Fire_BulletEx( int iMode, object &ammo, object user, int angle, int x, int y, bool bLaunch )
{
	if( /*iMode == 1 &&*/ bLaunch)
	{
		  //var ammo = CreateObject(S_U2,x,y,GetController(user));
		  ammo->Sound("shk-fire1");
		  ammo->LaunchLaser(user,angle,GetFMData(FM_Damage, 1),GetFMData(FM_DamageType,1));

		  var boom = CreateObject(TIM1,0,0,-1);
		  boom->~Sound("snp-fire1");
		  Schedule("RemoveObject()",40,0,boom);

		  //MuzzleFlash(40,user,x,y,angle,RGBa(64,64,255,0));
	}
}


public func Fire_Effects( int iMode, object user, int angle, int x, int y )
{
	  // Effekte
	  MuzzleFlash(50,user,x,y,angle);

	  var dir = GetDir(user)*2-1;
	  BulletCasing(0,y,-dir*Cos(angle-35*dir,20+Random(20)),-dir*Sin(angle-35*dir,20+Random(20)),5);
}

protected func Deselection(object pContainer)
{
	_inherited( pContainer );
	SetFireMode(1);
	if(pContainer->GetAction() == "Aim") pContainer->SetAction("Jump");
}

protected func Departure( object pContainer )
{
	_inherited( pContainer );
	SetFireMode(1);
	if(pContainer->GetAction() == "Aim") pContainer->SetAction("Jump");
}

public func ControlDig( object pCaller )
{
		if( firemode == 1)
		{
			var x = GetXDir(pCaller),y = GetYDir(pCaller);
			if(WildcardMatch(GetAction(pCaller),"Walk*")) x = y = 0;
			pCaller->~SetComDir(COMD_Stop);
			pCaller->~SetAction("Aim");

			pCaller->~SetSpeed(x,y);
			SetFireMode(2);
		}
		else
		{
			pCaller->~SetAction("Jump");
			SetFireMode(1);
		}

		return true; //ControlThrow(pCaller);
}


public func UTBotAIUse( pBot, pTarget )
{
	pBot->~UTBotAIAimAt( pTarget );
	var ymod;

	// Quick-Aim
	if( ObjectDistance( pBot, pTarget ) < 400 )
	if(!Inside(GetY( pBot ), GetY( pTarget )-10, GetY( pTarget )+10 ))
	{

		if(Random( 30-BotSkill(20) )) return;

		firemode = 1;
		ControlDig( pBot );
		SetComDir( COMD_Stop, pBot );
		pBot->~UTBotAIFire( this,2);
		ControlDig( pBot );

	}

	if( ObjectDistance( pBot, pTarget ) > 360 )
	{
		var order=[0,0,0];

		//for(var i=0; i<=2; i++) order[i]=pBot->~GetOrder(i+1);

		if( pBot->~GetAggroLevel() == Aggro_Shoot ) // Bot ist Sniper
		//if( order[0]==62 || order[1]==62 || order[2]==62 ) // Bot ist Sniper
		{
			if(!Random( 25-BotSkill(20) )) ymod = -7; // Auf Kopf zielen

			pBot->~UTBotAIAimAt( pTarget, 0, ymod );

			if(firemode != 2) ControlDig( pBot );

			pBot->~UTBotAIFire( this, 2);
			SetComDir( COMD_Stop, pBot );
		}
		else
		{
			return;
		}
	}
	else
	{
		if(firemode == 2) ControlDig( pBot );
		pBot->~UTBotAIFire( this, 1);
	}

}

