
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


protected func Death(int killed_by)
{
	// Do not announce the death as usual!
	this.silent_death = true;

	// This must be done first, before any goals do funny stuff with the clonk
	_inherited(killed_by,...);
		
	// The broadcast could have revived the clonk.
	if (GetAlive()) return;
	
	// Make him a corpse
	if (!IsCorpse())
	{
		OnDeathExtended(0, DMG_Melee, this, false);
	}

	// Custom death announcement?
	DeathAnnounceExtended(GetOwner(), GetKiller());
	RemoveObject();
}


func DeathAnnounceExtended(int plr, int killplr)
{
// TODO
/*
	if (GetEffect("NoAnnounce", this)) return;

	if (killplr == NO_OWNER) return;
	if ( bDeathHeadshot )
	{
		bDeathHeadshot = false;
		UA_Announcer( "an_aw_headshot", GetKiller() );
	}

	// suicide?
	if(plr == killplr)
		  HHKS->SKMsg(plr);
	else
		HHKS->KTMsg(killplr, plr, clonk->~LastDamageType(),clonk->~LastDamageWeapon());


	//Killstatistik.
	//Zwar ungenau wenn mehrere Clonks eines Spielers im Spiel sind,
	//aber wann ist das schon der Fall?
	HHKS->KillStat(GetCursor(killplr),plr);
*/
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


func OnDeathExtended(int damage_amount, int damage_type, object projectile)
{
	if (IsCorpse()) return;
	lib_player_death.is_corpse = true;
	
	//------------------------------------------
	// cleanup stuff
	
	DeathThrowWeapon(projectile);

	// In einem Fahrzeug? Rausschmeissen!
	if (Contained()->~IsVehicle())
	{
		Contained()->~ExitPassenger(this);
	}

	//------------------------------------------
	// determine settings for effects

	var divisor = 1 + MOD_FastBullets();
	
	var is_blast_weapon = false;
	if (damage_type & DMG_Explosion)
	{
		is_blast_weapon = true;
		Log("BlastAttack");
	}
	
	var bodyshot, feetshot;
	

	if (is_blast_weapon)
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
	
	//------------------------------------------
	// set up the corpse

	var rdir_base = (projectile->GetXDir() + projectile->GetYDir());

	var cl_body, cl_head, cl_legs;
	
	cl_head = CreateObject(CLBD, 0, 0, NO_OWNER);
	cl_legs = CreateObject(CLBD, 0, 0, NO_OWNER);
	cl_body = CreateObject(CLBD, 0, 0, NO_OWNER);
	
	cl_legs->SetPosition(GetX(), GetY());
	cl_body->SetPosition(GetX(), GetY());
	cl_head->SetPosition(GetX(), GetY());
	
	cl_legs->~CreateCorps(this, nil);
	cl_body->~CreateCorps(cl_legs, "Body");
	cl_head->~CreateCorps(cl_body, "Head");
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
	
	 // TODO: this is somewhat stupid because it is overriden by the values below?
	if (is_blast_weapon)
	{
		cl_legs->SetSpeed(RandomX(-5, +5) + projectile->GetXDir() / divisor,
						  RandomX(-5, +5) + projectile->GetYDir() / divisor);
		var rdir_legs = cl_legs->GetXDir() + cl_legs->GetYDir();
		cl_legs->SetRDir(rdir_legs);
		cl_head->SetRDir(rdir_legs / 2);
		cl_body->SetRDir(rdir_legs / 2);
	}
	else
	{
		cl_legs->SetRDir(rdir_base / (30 * divisor));
		cl_head->SetRDir(rdir_base / (50 * divisor));
		cl_body->SetRDir(rdir_base / (50 * divisor));
	}
	
	/* Teile anpassen */
	var deathcam_obj = cl_legs;
	
	var xdir_corpse = GetXDir() + projectile->GetXDir() / (3 * divisor);
	var ydir_corpse = GetYDir() + projectile->GetYDir() / (3 * divisor);
	var rdir_corpse = rdir_base / (10 * divisor);
	
	var ydir_variance = 10;
	
	if (headshot)
	{
		cl_head->SetPosition(GetX(), GetY() - 5);
		cl_head->SetSpeed(xdir_corpse, ydir_corpse - Random(ydir_variance));
		cl_head->SetRDir(rdir_corpse);
		cl_head->~SetMaster();
	}
	if (bodyshot)
	{
		deathcam_obj = cl_body;
		cl_body->~SetMaster();
		cl_head->SetSpeed(xdir_corpse, ydir_corpse - Random(ydir_variance));
		cl_body->SetSpeed(xdir_corpse, ydir_corpse - Random(ydir_variance));
		if (!MOD_NoBlood())
		{
			EffectGoreChunk(RandomX(-3, +3), RandomX(-3, +3), xdir_corpse, ydir_corpse - Random(ydir_variance));
			EffectGoreChunk(RandomX(-3, +3), RandomX(-3, +3), xdir_corpse, ydir_corpse - Random(ydir_variance));
			EffectGoreChunk(RandomX(-3, +3), RandomX(-3, +3), xdir_corpse, ydir_corpse - Random(ydir_variance));
		}
		cl_body->SetRDir(rdir_corpse);
		cl_head->SetRDir(rdir_corpse);
	}
	if (feetshot)
	{
		deathcam_obj = cl_body;
		cl_body->~SetMaster();
		cl_legs->SetSpeed(xdir_corpse, ydir_corpse - Random(ydir_variance));
		cl_legs->SetRDir(rdir_corpse);
		
		if (!MOD_NoBlood())
		{
			EffectGoreChunk(RandomX(-3, +3), RandomX(-3, +3), xdir_corpse, ydir_corpse - Random(ydir_variance));
			EffectGoreChunk(RandomX(-3, +3), RandomX(-3, +3), xdir_corpse, ydir_corpse - Random(ydir_variance));
			EffectGoreChunk(RandomX(-3, +3), RandomX(-3, +3), xdir_corpse, ydir_corpse - Random(ydir_variance));
		}
	}
	
	//------------------------------------------
	// sound effects

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

	//------------------------------------------
	// follow the corpse

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
