
local lib_player_death;

func Initialize()
{
	_inherited(...);
	
	lib_player_death = 
	{
		is_corpse = false,
	};
}

func IsCorpse()
{
	return lib_player_death.is_corpse;
}

/**
 Ejects the currently selected weapon, removes the rest of the inventory.
 
 @par projectile The projectile that hit the user.
 */
func DeathThrowWeapon(object projectile)
{
	var weapon = Contents();
	if (weapon)
	{
		weapon->Exit();
		var xdir = -Random(10) + GetXDir();
		var ydir = -Random(10) + GetYDir();
		var rdir = RandomX(-5, 5);
		if (projectile)
		{
			xdir += projectile->GetXDir() / 3;
			ydir += projectile->GetYDir() / 3;
			rdir += (projectile->GetXDir() + projectile->GetYDir()) / 10;
		}
		xdir /= 2;
		ydir /= 2;
		
		weapon->SetSpeed(xdir, ydir);
		weapon->SetRDir(rdir);
		weapon->SetCategory(C4D_Vehicle); // the weapon should not hurt objects
		//TODO: this should not be necessary Schedule("Disintegrate(0,20,50,-10)", 100, 0, weapon);
	}
	
	var contents = FindObjects(Find_Container(this));
	for (var item in contents) 
		item->RemoveObject();
}


func OnDeathExtended(int damage_amount, int iType, object projectile, bool headshot)
{
	if (IsCorpse()) return;
	lib_player_death.is_corpse = true;
	
	DeathThrowWeapon(projectile);
	
	if (headshot)
	{
		projectile->DoEnergy(-1000, this);
	}

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
	{
		Contained()->~ExitPassenger(this);
	}

	if (fBlastWeapon)
	{
		if (Inside(projectile->GetY() - GetY(), -6, 1))
			bodyshot = true;
		if (Inside(projectile->GetY() - GetY(), 1, 10))
			feetshot = true;
	}
	
	// Projektil sehr schnell: zurückschleudern;
	if (Abs(projectile->GetXDir()) > 140)
	{
		SetSpeed(GetXDir() + projectile->GetXDir() / (4 * divisor),
		         GetYDir() + projectile->GetYDir() / (4 * divisor));
	}
	else
	{
		
		SetSpeed(GetXDir() + projectile->GetXDir() / (8 * divisor),
				 GetYDir() + projectile->GetYDir() / (8 * divisor));
	}
	
	// MoreGore aktiviert?
	if (!MOD_NoBlood())
	{
		EffectCastBloodStream(MOD_MoreGore() * 2, 40 + Random(Abs(damage_amount)));
		EffectCastGore(MOD_MoreGore() / 3, 60 + Random(Abs(damage_amount)));
		
		if (headshot)
		{
			for (var amount = MOD_MoreGore(); amount > 0; amount -= 2)
			{
				CastGoreHeadshot();
			}
		}
	}
	
	/* Todesanimation bestimmen */
	var iPhase = 0;
	
	if (!GetDir())
	{
		if (projectile->GetX() > GetX())
		{
			iPhase = 1;
		}
		else if (projectile->GetX() < GetX())
		{
			iPhase = 2;
		}
	}
	else
	{
		if (projectile->GetX() < GetX())
		{
			iPhase = 1;
		}
		else if (projectile->GetX() > GetX())
		{
			iPhase = 2;
		}
	}
	

	var cl_body, cl_head, cl_legs;
	
	cl_head = CreateObject(CLBD, 0, 0, NO_OWNER);
	cl_legs = CreateObject(CLBD, 0, 0, NO_OWNER);
	cl_body = CreateObject(CLBD, 0, 0, NO_OWNER);
	
	cl_legs->SetPosition(GetX(), GetY());
	cl_body->SetPosition(GetX(), GetY());
	cl_head->SetPosition(GetX(), GetY());
	
	cl_legs->~CreateCorps(this, 0, iPhase);
	cl_body->~CreateCorps(cl_legs, "Body", iPhase);
	cl_head->~CreateCorps(cl_body, "Head", iPhase);
	cl_legs->~SetMaster();
	cl_body->SetDir(GetDir());
	cl_head->SetDir(GetDir());
	cl_legs->SetDir(GetDir());
	
	cl_legs->SetSpeed(GetXDir(), GetYDir());
	SetSpeed(); // TODO: unknown what this is good for...
	this.Visibility = VIS_None; // TODO: possibly not necessary anymore.
	
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
			projectile->GetXDir() / divisor - 5 + Random(11),
			projectile->GetYDir() / divisor - 5 + Random(11)
		);
		var amount = cl_legs->GetXDir() + cl_legs->GetYDir();
		cl_legs->SetRDir(amount);
		cl_head->SetRDir(amount / 2);
		cl_body->SetRDir(amount / 2);
	}
	else
	{
		cl_legs->SetRDir((projectile->GetXDir() + projectile->GetYDir()) / (30 * divisor));
		cl_head->SetRDir((projectile->GetXDir() + projectile->GetYDir()) / (50 * divisor));
		cl_body->SetRDir((projectile->GetXDir() + projectile->GetYDir()) / (50 * divisor));
	}
	
	/* Teile anpassen */
	var deathcam_obj = cl_legs;
	
	if (headshot)
	{
		
		SetPosition(GetX(), GetY() - 5, cl_head);
		cl_head->SetSpeed(GetXDir() + projectile->GetXDir() / (3 * divisor), -Random(10) + GetYDir() + projectile->GetYDir() / (3 * divisor));
		cl_head->SetRDir((projectile->GetXDir() + projectile->GetYDir()) / (10 * divisor));
		cl_head->~SetMaster();
	}
	if (bodyshot)
	{
		cl_body->~SetMaster();
		deathcam_obj = cl_body;
		cl_head->SetSpeed(GetXDir() + projectile->GetXDir() / (3 * divisor), -Random(10) + GetYDir() + projectile->GetYDir() / (3 * divisor));
		cl_body->SetSpeed(GetXDir() + projectile->GetXDir() / (3 * divisor), -Random(10) + GetYDir() + projectile->GetYDir() / (3 * divisor));
		if (!MOD_NoBlood())
		{
			EffectGoreChunk(RandomX(-3, +3), RandomX(-3, +3), GetXDir() + projectile->GetXDir() / (3 * divisor), -Random(10) + GetYDir() + projectile->GetYDir() / (3 * divisor),);
			EffectGoreChunk(RandomX(-3, +3), RandomX(-3, +3), GetXDir() + projectile->GetXDir() / (3 * divisor), -Random(10) + GetYDir() + projectile->GetYDir() / (3 * divisor));
			EffectGoreChunk(RandomX(-3, +3), RandomX(-3, +3), GetXDir() + projectile->GetXDir() / (3 * divisor), -Random(10) + GetYDir() + projectile->GetYDir() / (3 * divisor),);
		}
		cl_body->SetRDir((projectile->GetXDir() + projectile->GetYDir()) / (10 * divisor));
		cl_head->SetRDir((projectile->GetXDir() + projectile->GetYDir()) / (10 * divisor));
	}
	if (feetshot)
	{
		cl_body->~SetMaster();
		deathcam_obj = cl_body;
		cl_legs->SetSpeed(GetXDir() + projectile->GetXDir() / (3 * divisor), -Random(10) + GetYDir() + projectile->GetYDir() / (3 * divisor));
		cl_legs->SetRDir((projectile->GetXDir() + projectile->GetYDir()) / (10 * divisor));
		
		if (!MOD_NoBlood())
		{
			EffectGoreChunk(RandomX(-3, +3), RandomX(-3, +3), GetXDir() + projectile->GetXDir() / (3 * divisor), -Random(10) + GetYDir() + projectile->GetYDir() / (3 * divisor));
			EffectGoreChunk(RandomX(-3, +3), RandomX(-3, +3), GetXDir() + projectile->GetXDir() / (3 * divisor), -Random(10) + GetYDir() + projectile->GetYDir() / (3 * divisor));
			EffectGoreChunk(RandomX(-3, +3), RandomX(-3, +3), GetXDir() + projectile->GetXDir() / (3 * divisor), -Random(10) + GetYDir() + projectile->GetYDir() / (3 * divisor));
		}
	}
	
	var death_sound = nil;
	if (!death_sound && bodyshot && !Random(3))
	{
		death_sound = Format("%s_medic", CrewGetVoice(this));
	}
	if (!death_sound && feetshot && !Random(3))
	{
		death_sound = Format("%s_cant_feel_my_legs", CrewGetVoice(this));
	}
	if (!headshot)
	{
		DeathSound(death_sound);
	}

	// "Death-Cam" an
	SetPlrView(GetOwner(), cl_body);
	if (GetPlayerType(GetOwner()) == C4PT_User)
	{
		AddEffect("DeathCam", deathcam_obj, 200, 1, nil, Library_UA_PlayerDeath, GetOwner());
	}
}

func CastGoreHeadshot()
{
	EffectGoreChunk(0, -7, RandomX(-10, +10) + GetXDir() / 5, RandomX(-10, -35) + GetYDir() / 5 - 10);
}

func DeathSound()
{
	// TODO
}

func CrewGetVoice()
{
	// TODO
}
