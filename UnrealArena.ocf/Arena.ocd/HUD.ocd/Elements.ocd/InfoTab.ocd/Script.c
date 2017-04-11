/* HUD */

local pTarget;
local szMsg; // auszugebende Info-Nachricht
local szGraph, iVal, iFlash;

local Visibility = VIS_Owner;

public func SetHUDElement(object pHUD, string szAction)
{
	//TODO Local(0)=Local(1)=0; 		// Parallax auf 0 Setzen

	SetCategory(C4D_Vehicle|C4D_Parallax|C4D_Foreground|C4D_MouseIgnore|C4D_IgnoreFoW);

	SetAction( szAction ); 		// Anzeige-Action setzen

	SetOwner(pHUD->GetOwner());
}

public func AttachToHUD( object pHUD, int iX, int iY, object pObj, bool bActive, szGr )
{
	//iPosX = iX;
	//iPosY = iY;

	SetPosition(iX,iY); 		// Positionieren

	SetHUDElement(pHUD,"Exist");

	pTarget = pObj;

	szGraph = szGr;
	DoGraphics( bActive );
	//AddEffect("Flash",this,50,7,this);
	AddEffect("Flash",this,50,1,this);
}

public func DoGraphics( bActive )
{
	var w,h,y,size,yadj,clr_t,clr_s,clr_h;

	size = 750;

	if(bActive)
	{
		SetGraphics("Active");
		SetGraphics("HexActive", GUI_UA_InfoTab,1,GFXOV_MODE_ExtraGraphics);
		clr_t = RGBa(255,255,255,60);
		clr_h = RGBa(255,255,255,120);
	}
	else
	{
		SetGraphics();
		SetGraphics("Hex",GUI_UA_InfoTab,1,GFXOV_MODE_ExtraGraphics);
		clr_t = RGBa(200,200,200,120);
		clr_h = RGBa(255,255,255,160);
	}
	clr_s = RGBa(200,200,200,0);
	SetGraphics(Format("Bar%s",szGraph),GUI_UA_InfoTab,2,GFXOV_MODE_ExtraGraphics);


	SetObjDrawTransform(size,0,0,0,size,0,0);

	SetClrModulation(clr_t,0);
	SetClrModulation(clr_h,1);
	SetClrModulation(clr_s,2);

}

public func SetNumbers(iAmount)
{
	var iC=0, iX=0, iI=0;
	var size, x, dx, y;

	iC=(iAmount/100)%10;
	iX=(iAmount/10)%10;
	iI=iAmount%10;

	SetGraphics(Format("Number%d",iI),GUI_UA_InfoTab,3,GFXOV_MODE_Base);
	SetGraphics(Format("Number%d",iX),GUI_UA_InfoTab,4,GFXOV_MODE_Base);
	SetGraphics(Format("Number%d",iC),GUI_UA_InfoTab,5,GFXOV_MODE_Base);

	size = 750;
	x = 46;
	dx = 14;
	y = 125*100;

	SetObjDrawTransform(size,-70,(x+2*dx)*1000,0,size,y,3);
	SetObjDrawTransform(size,-70,(x+1*dx)*1000,0,size,y,4);
	SetObjDrawTransform(size,-70,x*1000,0,size,y,5);

}

public func UpdateVal(i)
{
	if(i != iVal)
	{
		//iFlash = 6;
		iFlash = BoundBy(Abs(i-iVal),10,50);
	}

	iVal = i;
	SetNumbers(i);
}

public func FxFlashTimer()
{
	iFlash = BoundBy(iFlash-1,0,50);
	//DoGraphics( iFlash%2 );
	DoGraphics( !!iFlash );
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

};
