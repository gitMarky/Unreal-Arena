/*-- Der Clonk --*/

#include Chain

/*
local pMaster, bNoMaster, bMoveFree, fixed_r, timer_r, fixed_d, timer_d, blood, gfx, type;
local iRSpeed;
local dismembered;
*/

/*
public func IsBulletTarget( id idBullet, object pProjectile, object pShooter)
{
	if(MOD_NoBlood()) return false;
	if(idBullet == XPLO ) return true;
	return false;
}
*/

/*
func FxAttachToTimer()
{
	if (!pMaster)
	{
		if (bNoMaster)
			return Splatter();
		else
			return RemoveObject();
	}
	
	SetDir(pMaster->GetDir());
	SetCon(pMaster->GetCon());
	SetColor(pMaster->GetColor());
	
	var iOvrl;
	if (gfx == "Head")
	{
		SetR(pMaster->GetR());
		iOvrl = 2;
	}
	else
		iOvrl = 3;
	
	SetPosition();
	
	SetGraphics(nil, nil, iOvrl, GFXOV_MODE_Object, nil, nil, this);
	
	var bRet;
	
	var r1 = Normalize(GetR(), -180);
	var r2 = Normalize(pMaster->GetR());
	
	var r = r1 - r2;
	if (r < -30)
	{
		SetRDir(+iRSpeed);
		bRet = true;
		SetR(r2 - 30);
	}
	if (r > 30)
	{
		SetRDir(-iRSpeed);
		bRet = true;
		SetR(r2 + 30);
	}
	
	if (bRet && iRSpeed > 0)
		iRSpeed--;
}

func CreateCorps(object master, string szGfx, int iPhase)
{
	SetOwner(master->GetOwner());
	SetColor(GetPlrColor(GetOwner()));
	type = PlrGetBlood(GetOwner());
	SetMaster(master);
	blood = pMaster.ch_blood;
	SetGraphics(szGfx);
	gfx = szGfx;
	SetAction("Dead");

	if (gfx != "Head")
		SetPhase(iPhase);
	
	if (gfx == "Body")
	{
	}
	else if (gfx == "Head")
	{
		SetVertex(0, 1, -5, this, 2);
		SetVertex(1, 1, -8, this, 2);
		SetVertex(2, 1, -8, this, 2);
	}
	else
	{
		SetVertex(0, 1, 8, this, 2);
		SetVertex(1, 1, 3, this, 2);
		SetVertex(2, 1, 3, this, 2);
		AddVertex(0, -4);
	}
	
	iRSpeed = 4;
	
	AddEffect("AttachTo", this, 50, 1, this);
}

func SetMaster(object master)
{
	SetOwner(GetOwner(master));
	if (!master)
	{
		bNoMaster = true;
		if (IsHead())
		{
			SetObjDrawTransform(1000, 0, 0, 0, 1000, 6000);
			SetGraphics("GoreHead", Corpse, 5, GFXOV_MODE_ExtraGraphics);
			SetClrModulation(BloodFXColor(type)[0], 5);
		}
		if (IsBody())
		{
			if (pMaster)
			{
				SetGraphics("GoreLegs", Corpse, 5, GFXOV_MODE_ExtraGraphics);
				pMaster->SetClrModulation(BloodFXColor(type)[0], 5);
			}
			SetGraphics("GoreBody", Corpse, 5, GFXOV_MODE_ExtraGraphics);
			SetClrModulation(BloodFXColor(type)[0], 5);
		}
	}
	else
	{
	}
	pMaster = master;
}

func IsHead()
{
	return gfx == "Head";
}

func IsBody()
{
	return gfx == "Body";
}

func IsFeet()
{
	return gfx == nil;
}

func Splatter()
{	
	dismembered++;
	
	if (!MOD_NoBlood() && dismembered < 50)
	{
		/*
		CreateParticle
		(
			BloodName(),
			0,
			0,
			0,
			0,
			10 + Random(30),
			RGBaRandom(BloodFXColor(type)[0], BloodFXColor(type)[1])
		);
		/
	}
	
	var r = Normalize(GetR(), -180);
	if (Inside(r, -10, 10) && !GetRDir())
	{
		if (r < 0)
			SetRDir(-5);
		else if (r > 0)
			SetRDir(5);
		else
			SetRDir(-5 + 10 * Random(2));
		
		if (GetContact(this, -1))
			SetPosition(GetX(), GetY() - 1);
	}
	
	if (dismembered > 200 && !IsDisintegrating())
	{
		Disintegrate(20, 50, -30, true);
	}
	//RemoveObject();
}


func BloodName()
{
	if (blood == 1)
		return "BloodAlien";
	if (blood == 2)
		return "BloodMachine";
	return "Blood";
}


func Hit()
{
	if (pMaster)
		Sound("Breakbone_0*");
}

func ContactBottom()
{
	if (!Random(3))
		SetXDir(GetXDir() / 2);
	return PhysicalJumping();
}
func ContactTop()
{
	return PhysicalJumping();
}
func ContactLeft()
{
	return PhysicalJumping();
}
func ContactRight()
{
	return PhysicalJumping();
}

func PhysicalJumping()
{
	if (!dismembered || (!GetXDir() && !GetYDir())) return false;
	SetRDir(GetRDir() * -2);

	if (dismembered < 90)
		CastParticles("Blood", 12, 30, 0, 0, 10, 40, BloodFXColor(type)[0], BloodFXColor(type)[1]);
	
	if (GBackSolid(5, 0) || GBackSolid(-5, 0))
		SetXDir(GetXDir() / -2);
	if (GBackSolid(0, 5) || GBackSolid(0, -5))
		SetYDir(GetYDir() / -2);
	if (GBackSolid(10, 0) || GBackSolid(-10, 0))
		SetXDir(GetXDir() / -2);
	if (GBackSolid(0, 10) || GBackSolid(0, -10))
		SetYDir(GetYDir() / -2);
	return true;
}

func SetMoveFree(bSet)
{
	return bMoveFree = bSet;
}

func OnHit(int iDmg, int iType, object pFrom)
{
	if (IsDisintegrating())
		return;
	CastObjectsCall([ID_Gore_Chunk, GetCon() / 10, 60 + iDmg / 2, 0, 3], "Launch", type);
	//CastObjects(ID_Gore_Chunk,GetCon()/10,60+iDmg/2,0,-3);
	CastParticles("Blood", iDmg * 6, 60, 0, 0, 10, 40, BloodFXColor(type)[0], BloodFXColor(type)[1]);
	RemoveObject();
	/*
	if( ch_race == 6 )
	{
		Sound("gibr*");
	}
	else
	{
		Sound("gib*");
	}/
}
*/
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// properties

local Name = "$Name$";
local Description = "$Description$";
local NoBurnDecay = 1;

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// actions

local ActMap = 
{
	Dead:
	{
		Prototype: Action,
		Name: "Dead",
		Procedure: DFA_NONE,
		Directions: 2,
		FlipDir: 1,
		Length: 3,
		Delay: 0,
		X: 0,
		Y: 0,
		Wdt: 16,
		Hgt: 20,
		NextAction: "Hold"
	}
};
