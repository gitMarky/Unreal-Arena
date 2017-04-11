/* HUD */

/*
SetObjDrawTransform(1000,0,50000,0,1000,25000,2)
SetGraphics(0,GLWP,2,GFXOV_MODE_IngamePicture)


500 / 750

Abstand 60 zwischen kleinen
Abstand 90 zwischen kleine und grossen

SetShape(0,0,60,40)
SetShape(0,-18,90,60)

SetClrModulation(RGBa(100,100,100,128))
 */


local pTarget;
local szMsg; // auszugebende Info-Nachricht
local Visibility = VIS_Owner;

public func SetHUDElement(object pHUD, string szAction)
{
	//TODO Local(0)=Local(1)=0; 		// Parallax auf 0 Setzen

	SetCategory(C4D_Vehicle|C4D_Parallax|C4D_Foreground|C4D_MouseIgnore|C4D_IgnoreFoW);

	SetAction( szAction ); 		// Anzeige-Action setzen

	SetOwner(pHUD->GetOwner());
}

public func AttachToHUD( object pHUD, int iX, int iY, object pObj, bool bActive )
{
	//iPosX = iX;
	//iPosY = iY;

	SetPosition(iX,iY); 		// Positionieren
/*
	SetHUDElement(pHUD,"Exist");

	var w,h,y,size,yadj,clr_t,clr_s,clr_a;

	if(bActive)
	{
		w = 90;
		h = 60;
		y = -18;
		yadj = 0;
		size = 750;
		SetGraphics("Active");
		clr_t = RGBa(255,255,255,60);
		clr_s = RGBa(255,255,255,0);
		clr_a = RGBa(100,100,100,0);
	}
	else
	{
		w = 60;
		h = 40;
		y = 0;
		yadj = 18000;
		size = 500;
		clr_t = RGBa(200,200,200,120);
		clr_s = RGBa(255,255,255,0);
		clr_a = RGBa(100,100,100,0);
	}

	var dx = pObj->~GetHUDOffsetX() + 45000*(pObj->GetDefCoreVal("Picture", "DefCore", 2))/64;
	var dy = pObj->~GetHUDOffsetY();


	SetObjDrawTransform(size,0,0,0,size,0);

	SetGraphics(0,pObj->GetID(),6,GFXOV_MODE_IngamePicture);
	SetObjDrawTransform(1000,0,dx,0,1000,yadj-5000+dy,6);
	SetClrModulation(clr_s,6);

	var pObj2 = pObj->~GetAkimbo();
	if( pObj2 )
	{
		// leicht schraeg und versetzt dazu zeichnen
		var r = pObj2->~GetAkimboHUDOffsetR() , xoff = pObj2->~GetAkimboHUDOffsetX(), yoff = pObj2->~GetAkimboHUDOffsetY();
		var fsin=Sin(r, 1000), fcos=Cos(r, 1000);

		SetGraphics(0,pObj2->GetID(),5,GFXOV_MODE_IngamePicture);
		SetObjDrawTransform( +fcos, +fsin,dx +xoff,-fsin,+fcos,yadj-5000+yoff+dy,5);
		SetClrModulation(clr_a,5);
	}

	SetShape(0,y,w,h);
	SetClrModulation(clr_t);

	// Fuellstand anzeigen 
	if(pObj->~IsWeapon())
	{
		//TODO SetGraphics(0,1HUD,3,GFXOV_MODE_IngamePicture);
		//TODO SetGraphics("Row",1HUD,4,GFXOV_MODE_IngamePicture);
		SetObjDrawTransform(600,0,60000,0,800,33*1000+yadj,3);

		var iCharge = pObj->~GetCharge();
		SetObjDrawTransform(iCharge*6/10,0,60000-30*(1000-iCharge),0,800,33*1000+yadj,4);
	}
*/
	pTarget = pObj;
}

public func HUDDisplayMessage( object pHUD, string szMessage )
{
	SetHUDElement(pHUD,"Message");
	PlayerMessage(GetOwner(),Format("@%s",szMsg));

	AddEffect("DisplayMsg",this,50,1,this,nil,szMessage);
}

public func FxDisplayMsgStart(object pTarget, int iEffectNumber, int iTemp, szMsg, var2, var3)
{
	//TODO EffectVar(0,pTarget,iEffectNumber) = szMsg;
}

public func FxDisplayMsgTimer(object pTarget, int iEffectNumber, int iEffectTime)
{
	if(iEffectTime > HUD_iMessageTime )
	{
		//Log("RemovedMessage");
		pTarget->~RemoveObject();
		return -1;
	}
	var a = (255*iEffectTime)/HUD_iMessageTime;

	//TODO CustomMessage(Format("        %s",EffectVar(0,pTarget,iEffectNumber)), pTarget, GetOwner(), 0, HUD_iMessageSize, RGBa(216,246,255,a), 0, 0, MSG_NoLinebreak);
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// properties

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// actions

local ActMap = {
Exist = {
	Prototype = Action,
	Name = "Exist",
	Procedure = DFA_FLOAT,
	FacetBase = 1,
	NextAction = "Hold",
},

Message = {
	Prototype = Action,
	Name = "Message",
	Procedure = DFA_FLOAT,
	X = 0,
	Y = 0,
	Wdt = 5,
	Hgt = 5,
	NextAction = "Hold",
},

};
