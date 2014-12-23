/*-- Der Clonk --*/

#strict 2

local pMaster, bNoMaster, bMoveFree, fixed_r, timer_r, fixed_d, timer_d, blood, gfx, type;
local iRSpeed;

public func IsBulletTarget( id idBullet, object pProjectile, object pShooter)
{
	if(MOD_NoBlood()) return false;
	if(idBullet == XPLO ) return true;
	return false;
}

public func FxAttachToTimer()
{
	if(!pMaster)
	{
		if(bNoMaster)
			return Splatter();
		else
			return RemoveObject();
	}

	SetDir(GetDir(pMaster));
	SetCon(GetCon(pMaster));
	SetColorDw(GetColorDw(pMaster));

	var iOvrl;
	if(gfx == "Head")
	{
		SetR(GetR(pMaster));
		iOvrl = 2;
	}
	else iOvrl = 3;

	SetPosition();

	SetGraphics(0,pMaster,0,iOvrl,GFXOV_MODE_Object,0,0,this);
	//if( gfx == "Body" ) SetGraphics("Body",this,0,4);


	var bRet;

	//if(GetR()>(GetR(pMaster)+30)) {SetRDir(-iRSpeed); /*SetR(GetR(pMaster));*/ bRet = true;}
	//if(GetR()>=0) if(GetR()<GetR(pMaster)-30) {SetRDir(+iRSpeed); /*SetR(GetR(pMaster));*/ bRet = true;}
	//if(GetR()<0) if((GetR()+360)<GetR(pMaster)-30) {SetRDir(+iRSpeed); /*SetR(GetR(pMaster));*/ bRet = true;}

	var r1 = Normalize(GetR(),-180);
	var r2 = Normalize(GetR(pMaster));

	var r = r1-r2;
	if( r < -30) {SetRDir(+iRSpeed); bRet = true; SetR(r2-30);}
	if( r > 30) {SetRDir(-iRSpeed); bRet = true; SetR(r2+30);}

	if(bRet && iRSpeed > 0) iRSpeed--;
}

public func CreateCorps( object master, string szGfx, int iPhase )
{
	SetOwner(GetOwner(master));
	SetColorDw(GetPlrColorDw(GetOwner()));
	type = PlrGetBlood(GetOwner());
	SetMaster(master);
	blood = LocalN("ch_blood",pMaster);
	SetGraphics( szGfx );
	gfx = szGfx;
	SetAction("Dead");

	if( gfx != "Head" )
		SetPhase(iPhase);

	if(gfx == "Body" )
	{

	}
	else if( gfx == "Head" )
	{
		SetVertex(0,1,-5,this,2);
		SetVertex(1,1,-8,this,2);
		SetVertex(2,1,-8,this,2);

	}
	else
	{
		SetVertex(0,1,8,this,2);
		SetVertex(1,1,3,this,2);
		SetVertex(2,1,3,this,2);
		AddVertex(0,-4);
	}

	iRSpeed = 4;

	AddEffect("AttachTo",this,50,1,this);
}

public func SetMaster( object master)
{
	SetOwner(GetOwner(master));
	if(!master)
	{
		bNoMaster = true;
		if(IsHead())
		{
			SetObjDrawTransform(1000,0,0,0,1000,6000);
			SetGraphics("GoreHead",0,CLBD,5,GFXOV_MODE_ExtraGraphics);
			SetClrModulation(BloodFXColor(type)[0],this,5);
			//SetObjDrawTransform(1000,0,0,0,1000,6000,this,5);
		}
		if(IsBody())
		{
			if(pMaster)
			{
				SetGraphics("GoreLegs",pMaster,CLBD,5,GFXOV_MODE_ExtraGraphics);
				SetClrModulation(BloodFXColor(type)[0],pMaster,5);
			}
			SetGraphics("GoreBody",0,CLBD,5,GFXOV_MODE_ExtraGraphics);
			SetClrModulation(BloodFXColor(type)[0],this,5);
		}
	}
	else
	{
	}
	pMaster = master;
}

public func IsHead(){ if( gfx == "Head" ) return true; return false;}
public func IsBody(){ if( gfx == "Body" ) return true; return false;}
public func IsFeet(){ if( !gfx ) return true; return false;}

public func Splatter()
{
	local dismembered;

	dismembered++;

	//if(IsHead()) SetPhase();

	//if(IsHead()) SetVertex(0,1,-7);
	if(!MOD_NoBlood() && dismembered < 50)
		CreateParticle(BloodName(),0,0,0,0,10+Random(30),RGBaRandom(BloodFXColor(type)[0],BloodFXColor(type)[1]));

	var r = Normalize(GetR(),-180);
	if(Inside(r,-10,10) && !GetRDir())
	{
		if(r < 0)
			SetRDir(-5);
		else if(r > 0)
			SetRDir(5);
		else
			SetRDir(-5+10*Random(2));

		if(GetContact(this,-1)) SetPosition(GetX(),GetY()-1);
	}

	if(dismembered>200 && !IsDisintegrating())
		Disintegrate(this,20,50,-30);
		//RemoveObject();
}


public func BloodName()
{
	if( blood == 1) return("BloodAlien");
	if( blood == 2) return("BloodMachine");

	return("Blood");
}


public func Hit()
{
  if(pMaster) Sound("Breakbone_0*");
}

public func ContactBottom()
{
	if(!Random(3)) SetXDir(GetXDir()/2);
	return PhysicalJumping();
}
public func ContactTop()
{
	return PhysicalJumping();
}
public func ContactLeft()
{
	return PhysicalJumping();
}
public func ContactRight()
{
	return PhysicalJumping();
}

public func PhysicalJumping()
{
	if(!dismembered) return(0);
	if(!GetXDir()) if(!GetYDir()) return(0);
	SetRDir(GetRDir()*-2);

	if(dismembered<90) CastParticles("Blood",12,30,0,0,10,40,BloodFXColor(type)[0],BloodFXColor(type)[1] );

	if(GBackSolid(5,0) || GBackSolid(-5,0)) SetXDir(GetXDir()/-2);
	if(GBackSolid(0,5) || GBackSolid(0,-5)) SetYDir(GetYDir()/-2);
	if(GBackSolid(10,0) || GBackSolid(-10,0)) SetXDir(GetXDir()/-2);
	if(GBackSolid(0,10) || GBackSolid(0,-10)) SetYDir(GetYDir()/-2);

	return(1);
}

public func SetMoveFree( bSet ){ return  bMoveFree=bSet; }

public func OnHit(int iDmg, int iType, object pFrom)
{
	if(IsDisintegrating()) return;
	CastObjectsCall([ID_Gore_Chunk,GetCon()/10,60+iDmg/2,0,3],"Launch",type);
	//CastObjects(ID_Gore_Chunk,GetCon()/10,60+iDmg/2,0,-3);
	CastParticles("Blood",iDmg*6,60,0,0,10,40,BloodFXColor(type)[0],BloodFXColor(type)[1] );
	RemoveObject();
/*
	if( ch_race == 6 )
	{
		Sound("gibr*");
	}
	else
	{
		Sound("gib*");
	}*/
}
