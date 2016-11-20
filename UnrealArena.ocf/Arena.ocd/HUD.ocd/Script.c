
local pCommandObj;
local aWeaponTabs;
local aMessages;
local pInfoHealth, pInfoArmor, pInfoShield, pInfoAmmo;

static const HUD_iMessages = 5;
static const HUD_iMessageSize = 23;
static const HUD_iMessageTime = 180;
static const HUD_bVert = true;

/* -------------------- globale Funktionen -----------------------*/

// aus dem TEAM-Objekt
global func GetGoalHUDEx(player)
{
	if(player <= -1) return;

	var bar = FindObjectOwner(3HUD,player);

	if( !g_bChooserFinished ) bar=0;

	return bar;
}

global func CreateHUD( obj )
{
	if( !obj ) return;
	var hud;
	if( hud = FindObjectOwner(_HUD,GetOwner(obj)))
	{
		LocalN("pCommandObj",hud)=obj;
		return;
	}


	var menu = CreateObject( _HUD,0,0,-1);

	SetPosition( 0, 0, menu);

	menu->~AttachHUD( obj );


	return menu;
}

global func FindHUD( plr )
{
	if(GetType(plr) == C4V_Int)
	{
		plr--; // man muss eine um eins erhöhte Zahl angeben, sonst wird sie nicht als Int erkannt
	}
	else if(GetType(plr) == C4V_C4Object)
	{
		plr = GetOwner(plr);
	}
	else return 0;
	return FindObjectOwner(_HUD,plr);
}


public func AttachHUD( object pObj )
{
	if(!pObj) return RemoveObject();
	SetOwner(GetOwner(pObj));

	pCommandObj = pObj;

	aWeaponTabs = [];

	//CreateWeaponTabs();

	ScheduleCall(this,"CreateWeaponTabs",5);
	ScheduleCall(this,"UpdateInfoTabs",5);
	AddEffect("ShowArmor",this,50,1,this);

}

protected func Destruction()
{
	if(bDestruction) return;
	local bDestruction;
	bDestruction = true;
	ClearScheduleCall();

	var tab;
	for( tab in aWeaponTabs ) RemoveObject(tab);
	if (pInfoHealth) RemoveObject(pInfoHealth);
	if (pInfoArmor) RemoveObject(pInfoArmor);
	if (pInfoShield) RemoveObject(pInfoShield);
	if (pInfoAmmo) RemoveObject(pInfoAmmo);
}

public func CreateWeaponTabs()
{
	ScheduleCall(this,"CreateWeaponTabs",5);
	var tab, bActive;
	// altes Zeug löschen
	for( tab in aWeaponTabs ) if(tab) RemoveObject(tab);

	if (!pCommandObj) return;

	// Inventar rückwärts durchgehen
	var inventory = pCommandObj->~GetInventory();//FindObjects( Find_Container(pCommandObj));

	if(!inventory) return;

	var x, xi=60, xa=90;


	//x = -50;
	x = 20;

	for(var i=0; i<GetLength(inventory); i++)
	//for(var i=GetLength(inventory)-1; i>=0; i--)
	{
		if(!inventory[i]) continue;
		//Log("Did inventory %d",i);
		if(Contents(0,pCommandObj) == inventory[i])
		{
			//x-=xa;
			bActive = true;
			//x+=xa;
		}
		else
		{
			//x-=xi;
			bActive = false;
			//x+=xi;
		}


		tab = CreateObject(_HWT,0,0,-1);

		tab->AttachToHUD(this,x,-50-30,inventory[i],bActive);
		PushBack(tab,aWeaponTabs);//aWeaponTabs[GetLength(inventory)-1-i]=tab;

		if(bActive)
			x+=xa;
		else
			x+=xi;
	}
}

public func UpdateInfoTabs()
{
	ScheduleCall(this,"UpdateInfoTabs",5);
	var tab, bActive;
	// altes Zeug löschen
	//for( tab in aInfoTabs ) if(tab) RemoveObject(tab);

	var x, fVert, fHor;

	fVert = HUD_bVert;
	fHor = !HUD_bVert;

	//var iTabs = 0;

	if(!pInfoAmmo)
	{
		tab = CreateObject(_HIT,0,0,-1);

		tab->AttachToHUD(this,10,-145*fVert -40*fHor,0,false,"Ammo");
		pInfoAmmo = tab;
		//iTabs++;
	}

	if(!pInfoHealth)
	{
		tab = CreateObject(_HIT,0,0,-1);

		tab->AttachToHUD(this,10,-145-40*fVert -40*fHor,0,false,"Health");
		pInfoHealth = tab;
		//iTabs++;
	}

	if(!pInfoArmor)
	{
		tab = CreateObject(_HIT,0,0,-1);

		tab->AttachToHUD(this,10 +85*fHor,-145-80*fVert -40*fHor,0,true,"Armor");
		pInfoArmor = tab;
		//iTabs++;
	}

	if(!pInfoShield)
	{
		tab = CreateObject(_HIT,0,0,-1);

		tab->AttachToHUD(this,10 +170*fHor,(-145-120)*fVert -40*fHor,0,false,"Shield");
		pInfoShield = tab;
	}
}

public func FxShowArmorTimer( target, nr, time)
{
	if(!pCommandObj) return;
	var val;
	var iTabs = 0;
	var fVert, fHor;

	fVert = HUD_bVert;
	fHor = !HUD_bVert;

	//PlayerMessage(GetOwner(target),Format("Shield %d|Armor %d|Health %d",GetUTShield(target),GetUTArmor(target),GetEnergy(target)));
	if( pInfoAmmo )
	{
		var weap = Contents(0,pCommandObj);
		var ammoid, ammo;
		if(!weap || !(weap->~IsWeapon()))
		{
			//pInfoAmmo->~SetVisibility(VIS_None);
			ammo = 0;
		}
		else
		{
			ammoid = weap->~GetFMData(FM_AmmoID);
			ammo = pCommandObj->~GetAmmo(ammoid);
			//iTabs++;
		}
		//ammo = pCommandObj->~GetAmmo(ammoid);
		pInfoAmmo->~UpdateVal(ammo);
	}
	if( pInfoHealth )
		pInfoHealth->~UpdateVal(GetEnergy(pCommandObj));

	if( pInfoArmor )
	{
		val = GetUTArmor(pCommandObj);
		pInfoArmor->~UpdateVal(val);
		pInfoArmor->~SetPosition(10,(-145-80)*fVert -40*fHor);
		if(!val)
		{
			SetVisibility(VIS_None,pInfoArmor);
		}
		else
		{
			SetVisibility(VIS_Owner,pInfoArmor);
			iTabs++;
		}

	}
	if( pInfoShield )
	{
		val = GetUTShield(pCommandObj);
		pInfoShield->~UpdateVal(val);
		pInfoShield->~SetPosition(10,(-145-80-40*iTabs)*fVert -40*fHor);
		if(!val)
		{
			SetVisibility(VIS_None,pInfoShield);
		}
		else
		{
			SetVisibility(VIS_Owner,pInfoShield);
			iTabs++;
		}

	}

}


global func HUDMessage( string szMessage, iPlr )
{
	// iPlr >= 0: Spieler
	// == -1: global
	// == -1 -TeamIndex: Team
	var plrs = [];
	if( iPlr < 0 )
	{
		if( iPlr == -1)
		{
			for(var i = 0; i < GetPlayerCount(C4PT_User); i++)
				plrs[i] = GetPlayerByIndex(i,C4PT_User);
		}
		else
		{
			var iTeam = Abs(iPlr)-1;
			Log("iTeam %d",iTeam);
			var j = 1;
			var plr = GetTeamPlayer(iTeam, j);
			while( plr != -1)
			{
				plrs[i] = plr;
				j++;
				plr = GetTeamPlayer(iTeam, j);
			}
			//for(var i = 0; i < GetPlayerCount(C4PT_User); i++)
			//	plrs[i] = GetPlayerByIndex(i,C4PT_User);
		}

	}
	else
		plrs = [iPlr];

	for( plr in plrs)
	{
		var hud = FindHUD(plr+1);
		if( hud ) hud->~MessageFeed(szMessage);
	}
}

public func MessageFeed( string szMessage )
{
	var obj = CreateObject(_HWT,0,0,-1);

	obj->~HUDDisplayMessage( this, szMessage );
	PushFront(obj,aMessages);
	SortMessages();
	Sound("ChatBeep",true,this,0,GetOwner());
}

public func SortMessages()
{
	var i = 0;
	for( obj in aMessages )
	{
		if(!obj) continue;

		var d = 0;
		if(GetGoalHUDEx(GetOwner())) d = 1;

	    	//SetPosition(45+60+d*13,95+ d*70 + i*HUD_iMessageSize,obj);
	    	SetPosition(45+d*13,95+ d*70 + i*HUD_iMessageSize,obj);
	    	i++;

	    if( i >= HUD_iMessages)
	    	obj->RemoveObject();
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// properties

local Name = "$Name$";
local Description = "$Description$";
