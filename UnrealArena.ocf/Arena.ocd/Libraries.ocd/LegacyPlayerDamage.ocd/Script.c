local bDeathHeadshot, bNoCorpse, LastDmgWeapon;

// TODO - start 
static const ID_Gore_Chunk = nil; 

func MOD_MoreGore()
{
	return 0;
}

func MOD_NoBlood()
{
	return false;
}

func MOD_FastBullets()
{
	return false;
}

func CrewGetBlood()
{
	return 0;
}

func CrewGetVoice()
{
}

func DeathThrowWeapon()
{
}

static const CLBD = nil;

static const ID_Gore_BloodStream = nil;

static const WC6I = nil;

// TODO - end

// Waffe des Killers speichern!
func LastDamageWeapon(id idWeap, bool bOverride)
{
	if (idWeap || bOverride)
		LastDmgWeapon = idWeap;
	
	return LastDmgWeapon;
}


global func BloodFXColor(string szType)
{
	if (szType == "Machine")
		return [RGBa(90, 0, 200, 40), RGBa(90, 0, 200, 160)];
	if (szType == "Alien")
		return [RGBa(0, 220, 0, 40), RGBa(0, 220, 0, 160)];
	return [RGBa(255, 0, 0, 40), RGBa(200, 0, 0, 160)];
}

func OnDmg(int iDmg, int iType)
{
	var armor = GetUTArmor(this);
	var shield = GetUTShield(this);
	var dmg = iDmg;
	
	// zuerst vom Schild abziehen
	shield = Min(shield, dmg);
	DoUTShield(-shield);
	dmg -= shield;
	if (shield)
		Sound("FieldHit*");
	
	// jetzt von der Rüstung,
	// die fängt aber nur halben Schaden!
	armor = Min(armor, dmg);
	DoUTArmor(-armor);
	dmg -= armor / 2;
	if (armor)
		Sound("ArmorHit*");
	
	// Prozent des gefangenen Schadens angeben
	var res = (iDmg - dmg) * 100 / iDmg;
	return res;
}


func OnHit(int iDmg, int iType, object pFrom)
{
	var bHeadshot = false;
	var iEnergy = GetEnergy() - iDmg;
	var pProjectile = pFrom;
	
	if (pProjectile)
	{
		LastDamageWeapon(pProjectile->~GetWeaponID(), true);
	}
	
	// Headshot?
	if (iType & DMG_Headshot)
	{
		if (Inside(GetX(pFrom) - GetX(), -7, 7) && Inside(GetY(pFrom) - GetY(), -10, -6))
			bHeadshot = true;
	}
	

	// Gore-Effekte
	if (iDmg > (this.MaxEnergy / 10000))
	{
		if (MOD_MoreGore())
			CastGore(ID_Gore_Chunk, 1, 60 + Abs(iDmg) - Random(Abs(iDmg)));
	}
	

	if (iDmg > (this.MaxEnergy / 5000))
	{
		if (MOD_MoreGore())
			CastGore(ID_Gore_Chunk, 1, 60 + Abs(iDmg) - Random(Abs(iDmg)));
		if (MOD_MoreGore() > 10)
			CastGore(ID_Gore_Chunk, 1, 60 + Abs(iDmg) - Random(Abs(iDmg)));
		if (MOD_MoreGore() > 18)
			CastGore(ID_Gore_Chunk, 1, 60 + Abs(iDmg) - Random(Abs(iDmg)));
		
		CreateObject(Effect_BlazingFlame, 0, 0, -1)->~SetUpFlame(this, iDmg, GetX(pProjectile) - GetX(), GetY(pProjectile) - GetY());
	}
	
	if (!MOD_NoBlood())
	{
		var divisor = 3 * (1 + MOD_FastBullets());
		var type = BloodFXColor(CrewGetBlood(this));
		CastParticles
		(
			"Blood",
			iDmg * 3,
			30,
			GetX(pProjectile) - GetX(),
			GetY(pProjectile) - GetY(),
			10,
			40,
			type[0],
			type[1]
		);
		FlingGore
		(
			ID_Gore_BloodStream,
			GetXDir(pProjectile) / divisor,
			GetYDir(pProjectile) / divisor,
			GetX(pProjectile) - GetX(),
			GetY(pProjectile) - GetY()
		);
	}
	

	// Treffer: Kopf
	if (bHeadshot)
	{
		bDeathHeadshot = true;
		iEnergy = 0;
	}
	
	// Explosion: Wegschleudern
	if (iEnergy > 0)
	{
		if (iType & DMG_Explosion)
			if (GetID(pFrom) != WC6I)
			{
				var divisor = 1 + MOD_FastBullets();
				
				SetSpeed
				(
					GetXDir(pFrom) / divisor - 5 + Random(11),
					GetYDir(pFrom) / divisor - 5 + Random(11)
				);
				SetAction("Tumble");
			}
		_inherited(iDmg, iType, pFrom);
	}
	else
		OnDeathExtended(iDmg, iType, pFrom, bHeadshot);
}

func OnDeathExtended(int iDmg, int iType, object pProjectile, bool headshot)
{
	if (bNoCorpse)
		return;
	bNoCorpse = true;
	
	DeathThrowWeapon(pProjectile);
	
	if (headshot)
		pProjectile->DoEnergy(-1000, this);
	
	var divisor = 1 + MOD_FastBullets();
	
	var fBlastWeapon = false;
	if (iType & DMG_Explosion)
	{
		fBlastWeapon = true;
		Log("BlastAttack");
	}
	
	var bodyshot, feetshot;
	
	// In einem Fahrzeug? Rausschmeissen!
	if (Contained()->~IsVehicle())
		Contained()->~ExitPassenger(this);
	

	if (fBlastWeapon)
	{
		if (Inside(GetY(pProjectile) - GetY(), -6, 1))
			bodyshot = 1;
		if (Inside(GetY(pProjectile) - GetY(), 1, 10))
			feetshot = 1;
	}
	
	// Projektil sehr schnell: zurückschleudern;
	if (Abs(GetXDir(pProjectile)) > 140)
	{
		SetSpeed
		(
			GetXDir() + GetXDir(pProjectile) / (4 * divisor),
			GetYDir() + GetYDir(pProjectile) / (4 * divisor)
		);
	}
	else
	{
		
		SetSpeed
		(
			GetXDir() + GetXDir(pProjectile) / (8 * divisor),
			GetYDir() + GetYDir(pProjectile) / (8 * divisor)
		);
	}
	

	// MoreGore aktiviert?
	if (!MOD_NoBlood())
	{
		CastGore(ID_Gore_BloodStream, MOD_MoreGore() * 2, 40 + Abs(iDmg) - Random(Abs(iDmg)));
		CastGore(ID_Gore_Chunk, MOD_MoreGore() / 3, 60 + Abs(iDmg) - Random(Abs(iDmg)));
		
		if (headshot)
		{
			if (MOD_MoreGore() > 18)
				FlingGore(ID_Gore_Chunk, -10 + Random(21) + GetXDir() / 5, GetYDir() / 5 - 10 - Random(25));
			if (MOD_MoreGore() > 16)
				FlingGore(ID_Gore_Chunk, -10 + Random(21) + GetXDir() / 5, GetYDir() / 5 - 10 - Random(25));
			if (MOD_MoreGore() > 12)
				FlingGore(ID_Gore_Chunk, -10 + Random(21) + GetXDir() / 5, GetYDir() / 5 - 10 - Random(25));
			if (MOD_MoreGore() > 8)
				FlingGore(ID_Gore_Chunk, -10 + Random(21) + GetXDir() / 5, GetYDir() / 5 - 10 - Random(25));
			if (MOD_MoreGore() > 6)
				FlingGore(ID_Gore_Chunk, -10 + Random(21) + GetXDir() / 5, GetYDir() / 5 - 10 - Random(25));
			if (MOD_MoreGore() > 4)
				FlingGore(ID_Gore_Chunk, -10 + Random(21) + GetXDir() / 5, GetYDir() / 5 - 10 - Random(25));
			if (MOD_MoreGore() > 2)
				FlingGore(ID_Gore_Chunk, -10 + Random(21) + GetXDir() / 5, GetYDir() / 5 - 10 - Random(25));
			if (MOD_MoreGore() > 0)
				FlingGore(ID_Gore_Chunk, -10 + Random(21) + GetXDir() / 5, GetYDir() / 5 - 10 - Random(25));
		}
	}
	
	/* Todesanimation bestimmen */
	var iPhase = 0;
	
	if (!GetDir())
	{
		if (GetX(pProjectile) > GetX())
			iPhase = 1;
		else if (GetX(pProjectile) < GetX())
			iPhase = 2;
	}
	else
	{
		if (GetX(pProjectile) < GetX())
			iPhase = 1;
		else if (GetX(pProjectile) > GetX())
			iPhase = 2;
	}
	

	var cl_body, cl_head, cl_legs;
	
	cl_head = CreateObject(CLBD, 0, 0, -1);
	cl_legs = CreateObject(CLBD, 0, 0, -1);
	cl_body = CreateObject(CLBD, 0, 0, -1);
	
	SetPosition(GetX(), GetY(), cl_legs);
	SetPosition(GetX(), GetY(), cl_body);
	SetPosition(GetX(), GetY(), cl_head);
	
	cl_legs->~CreateCorps(this, 0, iPhase);
	cl_body->~CreateCorps(cl_legs, "Body", iPhase);
	cl_head->~CreateCorps(cl_body, "Head", iPhase);
	cl_legs->~SetMaster();
	cl_body->SetDir(GetDir());
	cl_head->SetDir(GetDir());
	cl_legs->SetDir(GetDir());
	
	cl_legs->SetSpeed(GetXDir(), GetYDir());
	SetSpeed();
	this.Visibility = VIS_None;
	
	// Flammen-Effekte auf die Leiche übertragen
	var flame, flames = FindObjects(Find_ID(Effect_BlazingFlame), Find_Action("Hover"), Find_ActionTarget(this));
	for (flame in flames) 
	{
		flame->~SetMaster(cl_legs);
	}
	
	if (fBlastWeapon)
	{
		cl_legs->SetSpeed
		(
			GetXDir(pProjectile) / divisor - 5 + Random(11),
			GetYDir(pProjectile) / divisor - 5 + Random(11)
		);
		var amount = GetXDir(cl_legs) + GetYDir(cl_legs);
		cl_legs->SetRDir(amount);
		cl_head->SetRDir(amount / 2);
		cl_body->SetRDir(amount / 2);
	}
	else
	{
		cl_legs->SetRDir((GetXDir(pProjectile) + GetYDir(pProjectile)) / (30 * divisor));
		cl_head->SetRDir((GetXDir(pProjectile) + GetYDir(pProjectile)) / (50 * divisor));
		cl_body->SetRDir((GetXDir(pProjectile) + GetYDir(pProjectile)) / (50 * divisor));
	}
	
	/* Teile anpassen */
	var deathcam_obj = cl_legs;
	
	if (headshot)
	{
		
		SetPosition(GetX(), GetY() - 5, cl_head);
		SetSpeed
		(
			GetXDir() + GetXDir(pProjectile) / (3 * divisor),
			-Random(10) + GetYDir() + GetYDir(pProjectile) / (3 * divisor),
			cl_head
		);
		SetRDir((GetXDir(pProjectile) + GetYDir(pProjectile)) / (10 * divisor), cl_head);
		cl_head->~SetMaster();
	}
	if (bodyshot)
	{
		cl_body->~SetMaster();
		deathcam_obj = cl_body;
		SetSpeed
		(
			GetXDir() + GetXDir(pProjectile) / (3 * divisor),
			-Random(10) + GetYDir() + GetYDir(pProjectile) / (3 * divisor),
			cl_head
		);
		SetSpeed
		(
			GetXDir() + GetXDir(pProjectile) / (3 * divisor),
			-Random(10) + GetYDir() + GetYDir(pProjectile) / (3 * divisor),
			cl_body
		);
		if (!MOD_NoBlood())
		{
			FlingGore
			(
				ID_Gore_Chunk,
				GetXDir() + GetXDir(pProjectile) / (3 * divisor),
				-Random(10) + GetYDir() + GetYDir(pProjectile) / (3 * divisor),
				-3 + Random(7),
				-3 + Random(7)
			);
			FlingGore
			(
				ID_Gore_Chunk,
				GetXDir() + GetXDir(pProjectile) / (3 * divisor),
				-Random(10) + GetYDir() + GetYDir(pProjectile) / (3 * divisor),
				-3 + Random(7),
				-3 + Random(7)
			);
			FlingGore
			(
				ID_Gore_Chunk,
				GetXDir() + GetXDir(pProjectile) / (3 * divisor),
				-Random(10) + GetYDir() + GetYDir(pProjectile) / (3 * divisor),
				-3 + Random(7),
				-3 + Random(7)
			);
		}
		SetRDir((GetXDir(pProjectile) + GetYDir(pProjectile)) / (10 * divisor), cl_body);
		SetRDir((GetXDir(pProjectile) + GetYDir(pProjectile)) / (10 * divisor), cl_head);
	}
	if (feetshot)
	{
		cl_body->~SetMaster();
		deathcam_obj = cl_body;
		SetSpeed
		(
			GetXDir() + GetXDir(pProjectile) / (3 * divisor),
			-Random(10) + GetYDir() + GetYDir(pProjectile) / (3 * divisor),
			cl_legs
		);
		SetRDir((GetXDir(pProjectile) + GetYDir(pProjectile)) / (10 * divisor), cl_legs);
		
		if (!MOD_NoBlood())
		{
			FlingGore
			(
				ID_Gore_Chunk,
				GetXDir() + GetXDir(pProjectile) / (3 * divisor),
				-Random(10) + GetYDir() + GetYDir(pProjectile) / (3 * divisor),
				-3 + Random(7),
				-3 + Random(7)
			);
			FlingGore
			(
				ID_Gore_Chunk,
				GetXDir() + GetXDir(pProjectile) / (3 * divisor),
				-Random(10) + GetYDir() + GetYDir(pProjectile) / (3 * divisor),
				-3 + Random(7),
				-3 + Random(7)
			);
			FlingGore
			(
				ID_Gore_Chunk,
				GetXDir() + GetXDir(pProjectile) / (3 * divisor),
				-Random(10) + GetYDir() + GetYDir(pProjectile) / (3 * divisor),
				-3 + Random(7),
				-3 + Random(7)
			);
		}
	}
	
	if (bodyshot)
		if (!Random(3))
			DeathSound(Format("%s_medic", CrewGetVoice(this)));
	if (feetshot)
		if (!Random(3))
			DeathSound(Format("%s_cant_feel_my_legs", CrewGetVoice(this)));
	if (!headshot)
		DeathSound();
	
	// "Death-Cam" an
	SetPlrView(GetOwner(), cl_body);
	if (GetPlayerType(GetOwner()) == C4PT_User)
		AddEffect("DeathCam", deathcam_obj, 200, 1, 0, UA_Clonk, GetOwner());
}


func DeathSound(string szSound)
{
	if (GetEffect("NoDeathSound", this))
		return;
	AddEffect("NoDeathSound", this, 1, 0, this);
	if (szSound)
		return Sound(szSound);
	return Sound(Format("%s_death0*", CrewGetVoice(this)));
}

func Hurt()
{
	Sound(Format("%s_hit*", CrewGetVoice(this)));
}


func CastGore(id idGore, int iAmount, int iRadius, int iX, int iY)
{
	CastObjectsCall([idGore, iAmount, iRadius, iX, iY], "Launch", CrewGetBlood(this));
}

func FlingGore(id idGore, int iXDir, int iYDir, int iX, int iY)
{
	var chunk = CreateObject(idGore, iX, iY, -1);
	SetSpeed(iXDir, iYDir, chunk);
	chunk->~Launch(CrewGetBlood(this));
}


func FxDeathCamStart(object pTarget, proplist iEffectNumber, int iTemp, int iOwner)
{
	iEffectNumber.var0 = iOwner;
}

func FxDeathCamTimer(object pTarget, proplist iEffectNumber, int iEffectTime)
{
	if (iEffectTime > 90)
	{
		Log("DeathCamTimout");
		return -1;
	}
	if (!pTarget)
	{
		Log("DeathCamTarget");
		return -1;
	}
	var g_iPlrViewRange = 700;
	var i = (g_iPlrViewRange * Cos(iEffectTime, 100)) / 100;
	SetPlrViewRange(i, pTarget);
	SetPlrView(iEffectNumber.var0, pTarget);
}

func FxDeathCamStop(object pTarget, proplist iEffectNumber, int iReason)
{
	SetPlrView(iEffectNumber.var0, GetHiRank(iEffectNumber.var0));
}

