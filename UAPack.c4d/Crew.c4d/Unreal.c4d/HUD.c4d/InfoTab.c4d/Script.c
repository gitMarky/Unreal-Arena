/* HUD */

/*
SetObjDrawTransform(1000,0,50000,0,1000,25000,this,2)
SetGraphics(0,this,GLWP,2,GFXOV_MODE_IngamePicture)


500 / 750

Abstand 60 zwischen kleinen
Abstand 90 zwischen kleine und großen

SetShape(0,0,60,40)
SetShape(0,-18,90,60)

SetClrModulation(RGBa(100,100,100,128))
 */

#strict 2

local pTarget;
local szMsg; // auszugebende Info-Nachricht
local szGraph, iVal, iFlash;

public func SetHUDElement(object pHUD, string szAction)
{
	Local(0)=Local(1)=0; 		// Parallax auf 0 Setzen

	SetCategory(C4D_Vehicle|C4D_Parallax|C4D_Foreground|C4D_MouseIgnore|C4D_IgnoreFoW);

	SetVisibility(VIS_Owner); 	// Nur für den Besitzer sichtbar
	SetAction( szAction ); 		// Anzeige-Action setzen

	SetOwner(GetOwner(pHUD));
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
		SetGraphics("HexActive",0,_HIT,1,GFXOV_MODE_ExtraGraphics);
		clr_t = RGBa(255,255,255,60);
		clr_h = RGBa(255,255,255,120);
	}
	else
	{
		SetGraphics();
		SetGraphics("Hex",0,_HIT,1,GFXOV_MODE_ExtraGraphics);
		clr_t = RGBa(200,200,200,120);
		clr_h = RGBa(255,255,255,160);
	}
	clr_s = RGBa(200,200,200,0);
	SetGraphics(Format("Bar%s",szGraph),0,_HIT,2,GFXOV_MODE_ExtraGraphics);


	SetObjDrawTransform(size,0,0,0,size,0,this,0);

	SetClrModulation(clr_t,this,0);
	SetClrModulation(clr_h,this,1);
	SetClrModulation(clr_s,this,2);

}

public func SetNumbers(iAmount)
{
	var iC=0, iX=0, iI=0;
	var size, x, dx, y;

	iC=(iAmount/100)%10;
	iX=(iAmount/10)%10;
	iI=iAmount%10;

	SetGraphics(Format("Number%d",iI),0,_HIT,3,GFXOV_MODE_Base);
	SetGraphics(Format("Number%d",iX),0,_HIT,4,GFXOV_MODE_Base);
	SetGraphics(Format("Number%d",iC),0,_HIT,5,GFXOV_MODE_Base);

	size = 750;
	x = 46;
	dx = 14;
	y = 125*100;

	SetObjDrawTransform(size,-70,(x+2*dx)*1000,0,size,y,this,3);
	SetObjDrawTransform(size,-70,(x+1*dx)*1000,0,size,y,this,4);
	SetObjDrawTransform(size,-70,x*1000,0,size,y,this,5);

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
