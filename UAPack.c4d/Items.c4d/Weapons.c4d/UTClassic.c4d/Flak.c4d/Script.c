/*-- Pumpgun --*/

#strict

#include WEPN

public func KillMessage(){ return $KillMsg$; }

public func FeedUserAmmo(){ return true;}
public func AmmoPackID(){ return ID_UT99_Flak_Ammo;}

public func HandSize() { return 800;  }
public func HandX()    { return 7000; }
public func HandY()    { return -2500;  }
public func BarrelYOffset(){return -4000-2700*IsAkimbo();}

public func GetAkimboHUDOffsetX(){ return 5000; }
public func GetAkimboHUDOffsetY(){ return -10000; }

public func AkimboX()    { return 2000; }
public func AkimboY()    { return 2000; }

public func FMData1(int data)
{
  if(data == FM_Name)      return "$Standard$";
  if(data == FM_AmmoID)    return ID_Ammo_Flak;
  if(data == FM_AmmoLoad)  return 10;
  if(data == FM_AmmoUsage) return 1;

  if(data == FM_Recharge)  return 26; //43;

  if(data == FM_Damage)    return 8; //11;
  if(data == FM_Ballistic) return 1;
  if(data == FM_Old)	   return true;

  return(Default(data));
}
/*
public func BotData1(int data)
{
  if(data == BOT_Range)    return 200;
  if(data == BOT_Power)    return BOT_Power_2;

  return(Default(data));
}
*/

public func FMData2(int data)
{
  if(data == FM_Name)      return "$Grenade$";
  if(data == FM_AmmoID)    return ID_Ammo_Flak;
  if(data == FM_AmmoLoad)  return 10;
  if(data == FM_AmmoUsage) return 1;

  if(data == FM_Recharge)  return 36; //43;

  if(data == FM_Damage)    return 0;
  if(data == FM_Ballistic) return 1;
  if(data == FM_Old)	   return true;

  return(Default(data));
}
/*
public func BotData2(int data)
{
  if(data == BOT_Range)    return 320;
  if(data == BOT_Power)    return BOT_Power_3;

  return(Default(data));
}
*/


public func BotData1(int data)
{
  if(data == BOT_Range)    return 80;
  if(data == BOT_RangeMin) return 0;
  if(data == BOT_Power)    return BOT_Power_2;
  if(data == BOT_Rating)   return 16;
  if(data == BOT_Priority) return 30;

  return(Default(data));
}
public func BotData2(int data)
{
  if(data == BOT_Range)    return 80;
  if(data == BOT_RangeMin) return 50;
  return(BotData1(data));
}

public func Fire1()    // Projektilschuss
{
  var user = GetUser();
  var x,y; user->WeaponEnd(x,y);
  //var angle = user->AimAngle(1);
  var ammo;
	var iAcc = GetAccuracy(user)/2;
	var angle = user->AimAngle(GetFMData(FM_AimAngle)) + RandomX(-iAcc,+iAcc);

  var dir = GetDir(user)*2-1;

  for(var i=0; i<9; i++)
  {
	//var speed = 60 + Random(30); // war 120+Random(30)
	var speed = 60 + Random(7)*10;
	var rndang = angle+RandomX(-2,2);
	ammo = CreateObject(S_U6, x, y, GetController(user));
    ammo -> Launch(rndang, speed, 9999, 1, 1, GetFMData(FM_Damage, 1) );
    ammo -> SetBallistic();
    LocalN("iRefl",ammo) = 5;
    CreateParticle("ColSparkAttPerm",GetX(ammo)-GetX(),GetY(ammo)-GetY(),0,0,70,RGBa(255,100,0,150),ammo);

    if(!i)
    	  Sound("F2RegFire1",0,ammo);

		var range_x = 60 + Random(7)*10;
		var range_y = 60 + Random(7)*10;
		var xdir, ydir;
		var accu = GetAccuracy(user);

		//xdir =  +Sin( angle +2-Random(5) +Random(accu/2) - accu/4, range_x );
		//ydir =  -Cos( angle +2-Random(5) -Random(2) +Random(accu/2) - accu/4, range_y );
		xdir = +Sin( rndang, range_x); ydir = -Cos(rndang,range_y);
		ammo->~LaunchSpeed(xdir, ydir);
  }

  MuzzleFlash(RandomX(40,60), user,x,y,angle);

  // Sound
  Sound("F2Cock");
}

public func Fire2()    // Projektilschuss
{
  var user = GetUser();
  var x,y; user->WeaponEnd(x,y);
  //var angle = user->AimAngle(1);
  var ammo;

	var iAcc = 2+GetAccuracy(user)/2;
	var angle = user->AimAngle(GetFMData(FM_AimAngle)) + RandomX(-iAcc,+iAcc);

	var dir = GetDir(user)*2-1;

	var speed = 70 + Random(20); // war 120+Random(30)
    ammo = CreateObject(S_U6, x, y, GetController(user));
    ammo -> Launch(angle, speed, 9999, 1, 1, GetFMData(FM_Damage, 1) );
    ammo -> SetSecondary();
    ammo -> SetBallistic();

	var range_x = 60 + Random(7)*10;
	var range_y = 60 + Random(7)*10;
	var xdir, ydir;
	var accu = GetAccuracy(user);

	xdir =  +Sin( angle +2-Random(5) +Random(accu) - accu/2, range_x );
	ydir =  -Cos( angle -Random(2) +Random(accu) - accu/2, range_y );
	ammo->~LaunchSpeed(xdir, ydir);

	Sound("F2AltFire1",0,ammo);

  MuzzleFlash(RandomX(40,60), user,x,y,angle);

  // Sound
  Sound("F2Cock");
}

// Soundeffekte

public func OnReload()
{
  Sound("PumpgunLoad");
}
