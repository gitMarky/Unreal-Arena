/*-- Flammenwefer --*/

#strict

#include WEPN // Das Objekt ist eine Waffe  

// Anzeige in der Hand
public func HandSize() { return(1000); }
public func HandX()    { return(5000); }
public func HandY()    { return(500);  }

public func FMData1(int data)
{
  if(data == FM_Name)     return("$FlameThrower$");
  if(data == FM_AmmoID)   return(GSAM);
  if(data == FM_AmmoLoad) return(50);
  if(data == FM_Reload)   return(60);
  if(data == FM_Recharge) return(5);
  if(data == FM_Auto)     return(true);  

  if(data == FM_Damage)   return(22);

  return(Default(data));
}

public func BotData1(int data)
{
  if(data == BOT_Range)    return(150);
  if(data == BOT_DmgType)  return(DMG_Fire);
  if(data == BOT_Power)    return(BOT_Power_2);

  return(Default(data));
}

public func Fire1()
{
  var user = GetUser();
  var x,y; user->WeaponEnd(x,y);
  var angle = user->~AimAngle(20);
  var flame = CreateObject(FFLM, x,y, GetController(user));
  var xdir = +Sin(angle,35)+GetXDir(user);
  var ydir = -Cos(angle,35)+GetYDir(user);

  flame -> Launch(xdir,ydir,0, GetFMData(FM_Damage, 1));
}
/* Sounds */

public func OnAutoStart(int i)
{
  if(i == 1) Sound("Flamer",0,0,0,0,1);
  if(i == 2) Sound("BioFlamer",0,0,0,0,1);
}

public func OnAutoStop(int i)
{
  if(i == 1) Sound("Flamer",0,0,0,0,-1);
  if(i == 2) Sound("BioFlamer",0,0,0,0,-1);
}

public func OnReload()
{
  Sound("FlamerLoad");
}
