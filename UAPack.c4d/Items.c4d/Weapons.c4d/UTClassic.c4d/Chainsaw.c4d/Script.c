/*-- Motorsäge --*/

#strict 2

#include WEPN

public func KillMessage(){ return $KillMsg$; }
public func GetWeaponID(){ return GetID();}

public func FeedUserAmmo(){ return true; }

local rot, imfiring;

// Anzeige in der Hand
public func HandSize() { return 700; }
public func HandX()    { return 10000; }
public func HandY()    { return -2000; }
public func HandR()		 { return rot; }
public func BarrelXOffset(){return 2000;}
public func BarrelYOffset(){return 2000;}

public func GetHUDOffsetX(){ return 0; }
public func GetHUDOffsetY(){ return 10000; }

public func GetAkimboHUDOffsetX(){ return 10000; }
public func GetAkimboHUDOffsetY(){ return -7000; }

public func AkimboX()    { return 2000; }
public func AkimboY()    { return 2000; }

public func FMData1(int data)
{
  if(data == FM_Name)      return "$Saw$";
  if(data == FM_AmmoID)    return GSAM;
  if(data == FM_AmmoLoad)  return 20;

  if(data == FM_Reload)    return 120;
  if(data == FM_Recharge)  return 2;

  if(data == FM_AmmoUsage) return 1;
  if(data == FM_AmmoRate)  return 10;
  if(data == FM_Auto)      return true;

  if(data == FM_Damage)    return 5;
  if(data == FM_Old)	   return true;

  return Default(data);
}

public func BotData1(int data)
{
  if(data == BOT_Range)    return 10;
  if(data == BOT_DmgType)  return DMG_Melee;
  if(data == BOT_Power)    return BOT_Power_2;

  return Default(data);
}

public func FMData2(int data)
{
  if(data == FM_Name)      return "$Swipe$";
  if(data == FM_AmmoID)    return GSAM;
  if(data == FM_AmmoLoad)  return 20;

  if(data == FM_Reload)    return 120;
  if(data == FM_Recharge)  return 40;

  if(data == FM_AmmoUsage) return 1;
  if(data == FM_AmmoRate)  return 10;
  if(data == FM_Auto)      return false;

  if(data == FM_Damage)    return 40;
  if(data == FM_Old)	   return true;

  return Default(data);
}

public func BotData1(int data)
{
  if(data == BOT_Range)    return 20;
  if(data == BOT_RangeMin) return 0;
  if(data == BOT_DmgType)  return DMG_Melee;
  if(data == BOT_Power)    return BOT_Power_3;
  if(data == BOT_Rating)   return 0;

  return Default(data);
}

public func BotData2(int data)
{
  if(data == BOT_Range)    return 30;
  if(data == BOT_RangeMin) return 10;
  if(data == BOT_DmgType)  return DMG_Melee;
  if(data == BOT_Power)    return BOT_Power_2;
  if(data == BOT_Rating)   return 0;

  return Default(data);
}

public func Fire1()    // Wrrrrr
{
}

public func Running()
{
  var user = GetUser();
  var x,y; user->WeaponEnd(x,y,IsAkimbo());
  
  var treffer = false;
  
	for ( var i in FindObjects(
								Find_NoContainer(),
								Find_Or(Find_AtPoint(x,y), Find_AtPoint(x/2, y/2), Find_AtPoint(0,0)),
								Find_Or(
									Find_Func("IsBulletTarget",GetID(),this()),
									Find_OCF(OCF_Alive)),
								Find_Func("CheckEnemy", user),
								Find_Exclude(user)))
	{
		if(CheckEnemy (i, user))
		{
			var bullet = CreateObject(TIM1,0,0,-1);
			SetPosition(GetX()+x,GetY()+y,bullet);
			bullet->~DoDmg(GetFMData(FM_Damage,1), DMG_Melee, i);
			bullet->Schedule("RemoveObject()",1,0,bullet);
			//Unser gegner weg? (alienz zerplatze ja :<)
			if(!i)
			  continue;
			// kA, grad mal billiger Zappel-Effekt hier..
			i ->~ SetAction("Jump");
			
			treffer = true;
			
			//for ( var i = 3 ; i > 0; i--)
			//	CreateParticle("MSpark", x, y, -RandomX(0, 2*x), -RandomX(0, 2*y), Random(25)+10, RGB(255,100+Random(155),Random(220)));
		}
		// TODO: Ratsch-Sound, Effekte
	}
	if(GBackSolid(x,y)) 
	{
		treffer = true;
		for ( var i = 3 ; i > 0; i--)
			CreateParticle("MSpark", x, y, -RandomX(0, 2*x), -RandomX(0, 2*y), Random(25)+10, RGB(255,100+Random(155),Random(220)));
		// TODO: Sirr-Sound
	}
	// TODO: Ratter-Sound
	//DoSound("chainsaw",3);
	
	if(treffer)
		rot = Pulse(GetActTime()*35)/40-15;
	else
		rot = RandomX(2,7);
}

public func Fire2()    // Schwingen!!
{
	SetAction("Swiping");
	//SetFireMode(1);
}

public func Swiping()
{
	rot = -80+GetActTime()*4 + RandomX(2,7);

	if(GetActTime() > 20)
	{
		SetAction("SwipeRecover");
	}

	var user = GetUser();
	var x,y; user->WeaponEnd(x,y,IsAkimbo());

	if(GetActTime()<15) return;
	var treffer = false;

	for ( var i in FindObjects(
								Find_NoContainer(),
								Find_Or(Find_AtPoint(x,y), Find_AtPoint(x/2, y/2), Find_AtPoint(0,0)),
								Find_Or(
									Find_Func("IsBulletTarget",GetID(),this()),
									Find_OCF(OCF_Alive)),
								Find_Func("CheckEnemy", user),
								Find_Exclude(user)))
	{
		if(CheckEnemy (i, user))
		{
			var bullet = CreateObject(TIM1,0,0,-1);
			SetPosition(GetX()+x,GetY()+y,bullet);
			bullet->~DoDmg(GetFMData(FM_Damage,1), DMG_Melee|DMG_Headshot, i);
			bullet->Schedule("RemoveObject()",1,0,bullet);
			//Unser gegner weg? (alienz zerplatze ja :<)
			if(!i)
			  continue;
			// kA, grad mal billiger Zappel-Effekt hier..
			i ->~ SetAction("Jump");

			treffer = true;
		}
	}
	if(GBackSolid(x,y))
	{
		treffer = true;
		for ( var i = 3 ; i > 0; i--)
			CreateParticle("MSpark", x, y, -RandomX(0, 2*x), -RandomX(0, 2*y), Random(25)+10, RGB(255,100+Random(155),Random(220)));
		// TODO: Sirr-Sound
	}

	if(treffer)
	{
		//SetFireMode(1);
		//stopauto = true;
		SetAction("SwipeRecover");
	}
}

public func OnAutoStart()
{
	//if(GetAction() == "SwipeRecover")
	imfiring = true;
	//Sound("chainsaw_start",0,this());
	//ScheduleCall(this(),"ChainsawSound", 19);
	//DoSound("chainsaw_start",10);
	//Sound("ChainSaw",0,this(),0,0,+1);
	SetAction("Idle");
	SetAction("Run");
}

public func OnAutoStop()
{
	SetAction("Idle");
	imfiring = 0;
	rot = 0;
  //Sound("ChainSaw",0,this(),0,0,-1);
  //Sound("chainsaw_stop",0,this());
  //DoSound("chainsaw_stop",10);
}

public func DoSound(string snd, int time)
{
	if(GetEffect("NoSound",this()))
		return;
	
	Sound(snd,0,this());
	AddEffect("NoSound",this(),1,time,this(),GetID());
}

// Soundeffekte

public func OnSelect()
{
	Sound("ChainIdle",0,this(),0,0,+1);
}

public func OnDeselect()
{
	Sound("ChainIdle",0,this(),0,0,-1);
	SetAction("Idle");
	rot = 0;
}

protected func Departure() {
	_inherited();
	Sound("ChainIdle",0,this(),0,0,-1);
	SetAction("Idle");
	rot = 0;
}

public func OnReload()
{
  Sound("FlamerLoad");
}


public func OnSwipeRecover()
{
	SetFireMode(1);
}

public func ControlDig( object pCaller )
{
		if(ActIdle() || GetAction() == "Run")
		{
			SetFireMode(2);
			return ControlThrow(pCaller);
		}
		return true; //ControlThrow(pCaller);
}

