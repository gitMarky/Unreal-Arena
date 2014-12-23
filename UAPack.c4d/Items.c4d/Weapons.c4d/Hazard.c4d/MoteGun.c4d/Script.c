/*-- Partikelkanone --*/

#strict

#include WEPN

local laser; //der Laserstrahl

// Anzeige in der Hand
public func HandX() { return(5000); }
public func HandY() { return(500); }

//RAILGUN!!!!!
func FMData1(int data) {
  if(data == FM_Name)       return("$Railgun$");

  if(data == FM_AmmoID)     return(ENAM);
  if(data == FM_AmmoLoad)   return(100);
  if(data == FM_AmmoUsage)  return(50);

  if(data == FM_Reload)     return(110);
  if(data == FM_Recharge)   return(120);

  if(data == FM_Damage)     return(165);

  return(Default(data));
}

public func BotData1(int data)
{
  if(data == BOT_Range)    return(600);
  if(data == BOT_DmgType)  return(DMG_Energy);
  if(data == BOT_Power)    return(BOT_Power_LongLoad);

  return(Default(data));
}

//Toller EMPschuss
func FMData2(int data) {
  if(data == FM_Name)       return("$Shockwave$");

  if(data == FM_AmmoID)     return(ENAM);
  if(data == FM_AmmoLoad)   return(30);
  if(data == FM_AmmoUsage)  return(30);

  if(data == FM_Reload)     return(75);

  if(data == FM_Damage)     return(450);

  return(Default(data));
}

public func BotData2(int data)
{
  if(data == BOT_Range)    return(400);
  if(data == BOT_DmgType)  return(0);
  if(data == BOT_Power)    return(BOT_Power_1);
  if(data == BOT_EMP)      return(true);

  return(Default(data));
}

//Railgun ist nur Lasergehaxxe.
public func Fire1() {
  var user = GetUser();
  var x,y; user->WeaponEnd(x,y);
  var angle = user->AimAngle(5);
  laser = CreateObject(LASR,x,y,GetController(user));
  laser->SetClrModulation(RGBa(255,255,255,64));
  laser->Set(angle,8,700,60,this(),user);
  laser->SetContinuous(5);

  MuzzleFlash(300,user,x,y,angle,RGBa(64,64,255,0));
  Sound("RailGunShot");
}

//EMP!
public func Fire2() {
  var user = GetUser();
  var x,y; user->WeaponEnd(x,y);
  var angle = user->AimAngle(15);
  var dir = GetDir(user)*2-1;
  var ammo = CreateObject(SHKR,x,y,GetController(user));
  ammo->Launch(angle,50, GetFMData(FM_Damage, 2), 100, 60, 0);

  MuzzleFlash(50,user,x,y,angle,RGBa(40,40,255,0));
  Sound("EMPShot");
}

public func LaserStrike(object pObj, int iTime) {
  //Schaden machen, natürlich tollen Energiebrazzelschaden!
  var schwaecher = Max(iTime-25,0);
  DoDmg(Max(0,GetFMData(FM_Damage, 1)/8-schwaecher), DMG_Energy, pObj);

	// durch 8 weil er effektiv (nach Abzug des schwächer-werdens) 8mal trifft innerhalb dieser 60 Frames
  return(0);
}

public func IsFiring() { return(laser); }

// Soundeffekte

public func OnReload()
{
  if(firemode == 1)
    Sound("MoteGunLoad");
  if(firemode == 2)
    Sound("EMPLoad");
}

public func OnDeselect() {
  if(laser) {
    RemoveObject(laser);
  }
}
