
local lib_player_death;


func Initialize()
{
	_inherited(...);
	
	lib_player_death = 
	{
		is_corpse = false,
		on_ground = false,
		corpse_headshot = false,
		corpse_blasted = false,
		corpse_blasted_body = false,
		corpse_blasted_legs = false,
		corpse_lost_arm_r = false,
		corpse_lost_arm_l = false,
		damage_amount = 0,
		damage_type = DMG_Suicide,
		hit_x = 0,
		hit_y = 0,
		hit_xdir = 0,
		hit_ydir = 0,
	};
}

func IsCorpse()
{
	return GetCorpseData().is_corpse;
}


func IsHeadshot()
{
	return (GetCorpseData().damage_type & DMG_Headshot)
	    && Inside(GetCorpseData().hit_x, -7, 7)
	    && Inside(GetCorpseData().hit_y, -10, -6);
}


func GetCorpseData()
{
	return lib_player_death;
}


func DeathSound(string sound)
{
	if (IsCorpse()) return;

	Sound(sound ?? Format("%s_death0*", CrewGetVoice()));
}


func CrewGetVoice()
{
	// TODO
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// Death and events on death

func Death(int killed_by)
{
	// Do not announce the death as usual!
	this.silent_death = true;

	// This must be done first, before any goals do funny stuff with the clonk
	_inherited(killed_by,...);
		
	// The broadcast could have revived the clonk.
	if (GetAlive()) return;
	
	// Make him a corpse
	OnDeathBecomeCorpse();

	// Custom death announcement?
	DeathAnnounceExtended(GetOwner(), GetKiller());
	RemoveObject();
}


/**
 This is what happens when the player becomes a corpse.
 
 @par projectile The object that caused death.
 @par damage_amount The amount of damage that caused death.
 @par damage_type The damage type that caused death.
 */
func OnDeathBecomeCorpse(object projectile, int damage_amount, int damage_type)
{
	if (!IsCorpse()) // Do this only if you are not yet a corpse, so that multiple hits per round, etc., cannot mess with the stats
	{
		this->OnDeathDetermineCorpseData(projectile, damage_amount, damage_type);
		this->OnDeathExitVehicle();
		this->OnDeathThrowWeapon();
		this->OnDeathSound();
		this->OnDeathHandleCorpse(); // this is where the player actually becomes a corpse
	}
}


/**
 Determines the way the corpse will look.
 
 @par projectile The object that caused death.
 @par damage_amount The amount of damage that caused death.
 @par damage_type The damage type that caused death.
 */
func OnDeathDetermineCorpseData(object projectile, int damage_amount, int damage_type)
{
	if (projectile)
	{
		// Get all basic info from the projectile first
	
		GetCorpseData().hit_x = projectile->GetX() - GetX();
		GetCorpseData().hit_y = projectile->GetY() - GetY();
		GetCorpseData().hit_xdir = projectile->GetXDir();
		GetCorpseData().hit_ydir = projectile->GetYDir();
		GetCorpseData().damage_amount = Abs(damage_amount);
		GetCorpseData().damage_type = damage_type ?? DMG_Suicide;

		// Determine advanced info
		
		GetCorpseData().corpse_headshot = IsHeadshot();
		
		if (damage_type & DMG_Explosion)
		{
			if (Inside(projectile->GetY() - GetY(), -6, 1))
			{
				GetCorpseData().corpse_blasted_body = true;
			}
			else if (Inside(projectile->GetY() - GetY(), 1, 10))
			{
				GetCorpseData().corpse_blasted_legs = true;
			}
			
			GetCorpseData().corpse_blasted = true;
		}
		
	}
	
	if (GetContact(-1) & CNAT_Bottom)
	{
		GetCorpseData().on_ground = true;
	}
	
	return GetCorpseData();
}


func DeathAnnounceExtended(int player, int killed_by_player)
{
// TODO
/*
	if (GetEffect("NoAnnounce", this)) return;

	if (killed_by_player == NO_OWNER) return;
	if ( bDeathHeadshot )
	{
		bDeathHeadshot = false;
		UA_Announcer( "an_aw_headshot", GetKiller() );
	}

	// suicide?
	if(plr == killed_by_player)
		  HHKS->SKMsg(player);
	else
		HHKS->KTMsg(killed_by_player, player, clonk->~LastDamageType(),clonk->~LastDamageWeapon());


	//Killstatistik.
	//Zwar ungenau wenn mehrere Clonks eines Spielers im Spiel sind,
	//aber wann ist das schon der Fall?
	HHKS->KillStat(GetCursor(killed_by_player), player);
*/
}


/**
 Ejects the currently selected weapon, removes the rest of the inventory.
 
 @par projectile The projectile that hit the user.
 */
func OnDeathThrowWeapon(object projectile)
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
			xdir += GetCorpseData().hit_xdir / 3;
			ydir += GetCorpseData().hit_ydir / 3;
			rdir += (GetCorpseData().hit_xdir + GetCorpseData().hit_ydir) / 10;
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


/**
 Ejects the Clonk from an open vehicle. Closed vehicles, such as
 a tank, are ignored.
 */
func OnDeathExitVehicle()
{
	if (Contained() && Contained()->~IsOpenVehicle())
	{
		Contained()->~ExitPassenger(this);
	}
}


/**
 Handles the way sounds are played when the clonk dies.
 
 @par corpse_data Contains information on how the corpse looks, which is important for certain effects.
 */
func OnDeathSound()
{
	var death_sound = nil;
	if (!death_sound && GetCorpseData().corpse_blasted_body && !Random(3))
	{
		death_sound = Format("%s_medic", CrewGetVoice());
	}
	if (!death_sound && GetCorpseData().corpse_blasted_legs && !Random(3))
	{
		death_sound = Format("%s_cant_feel_my_legs", CrewGetVoice());
	}
	if (!GetCorpseData().corpse_headshot)
	{
		DeathSound(death_sound);
	}
}


/**
 Creates a corpse effect and handles the way it looks, based on certain parameters.
 */
func OnDeathHandleCorpse()
{
	// Use some safety here, so that the player does not become a corpse multiple times
	if (IsCorpse()) return;
	GetCorpseData().is_corpse = true;
	
	// This has no effect inside a building or vehicle
	if (Contained()) return;
	
	// Handle the different death types; this will differentiate between headshots and so on later
	if (true) // Standard death
	{
		HandleCorpseDefault();
	}
}


// Outdated function, as a reference
func OnDeathHandleCorpseLegacy()
{
	if (IsCorpse()) return;
	GetCorpseData().is_corpse = true;
	
	if (Contained()) return;

	//------------------------------------------
	// determine settings for effects

	var divisor = 1 + MOD_FastBullets();
	
	
	// Projektil sehr schnell: zurückschleudern;
	if (Abs(GetCorpseData().hit_xdir) > 140)
	{
		SetSpeed(GetXDir() + GetCorpseData().hit_xdir / (4 * divisor),
		         GetYDir() + GetCorpseData().hit_ydir / (4 * divisor));
	}
	else
	{
		
		SetSpeed(GetXDir() + GetCorpseData().hit_xdir / (8 * divisor),
				 GetYDir() + GetCorpseData().hit_ydir / (8 * divisor));
	}
	
	// MoreGore aktiviert?
	if (!MOD_NoBlood())
	{
		EffectCastBloodStream(MOD_MoreGore() * 2, 40 + Random(GetCorpseData().damage_amount));
		EffectCastGore(MOD_MoreGore() / 3, 60 + Random(GetCorpseData().damage_amount));
		
	}
	
	//------------------------------------------
	// set up the corpse

	var rdir_base = (GetCorpseData().hit_xdir + GetCorpseData().hit_ydir);

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
	if (GetCorpseData().Blast)
	{
		cl_legs->SetSpeed(RandomX(-5, +5) + GetCorpseData().hit_xdir / divisor,
						  RandomX(-5, +5) + GetCorpseData().hit_ydir / divisor);
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
	
	var xdir_corpse = GetXDir() + GetCorpseData().hit_xdir / (3 * divisor);
	var ydir_corpse = GetYDir() + GetCorpseData().hit_ydir / (3 * divisor);
	var rdir_corpse = rdir_base / (10 * divisor);
	
	var ydir_variance = 10;
	
	if (GetCorpseData().corpse_headshot)
	{
		cl_head->SetPosition(GetX(), GetY() - 5);
		cl_head->SetSpeed(xdir_corpse, ydir_corpse - Random(ydir_variance));
		cl_head->SetRDir(rdir_corpse);
		cl_head->~SetMaster();
	}
	if (GetCorpseData().corpse_blasted_body)
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
	if (GetCorpseData().corpse_blasted_legs)
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
	// follow the corpse

	// "Death-Cam" an
	SetPlrView(GetOwner(), cl_body);
	if (GetPlayerType(GetOwner()) == C4PT_User)
	{
		AddEffect("DeathCam", deathcam_obj, 200, 1, nil, Library_UA_PlayerDeath, GetOwner());
	}
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// Handling the corpse
//
// The concept is as follows:
//
// - the normal death animation is played for everything that is on the ground (i.e. legs and everything connected)
// - the jump death animation is played for everything that flies lose (i.e. a severed head, or the player while he is jumping)
// - the severed parts may rotate freely and have specific vertex layout
// - the severed parts need an effect that draw-transforms them so that they appear to be rotating about their center of gravity
// - the standard death animation parts may either not rotate, or rotate under the circumstance that there is an effect that updates their vertices
//   * said effect would have to move the vertices to certain positions that match the current shape of the animation
//   * the effect should move vertices towards the desired position and go pack one step if it gets stuck (a little ragdoll-like)


/**
 Creates a corpse. The corpse parts have to be assigned with
 */
func CreateCorpse()
{
	var corpse = CreateObject(UA_Clonk_Corpse, 0, 0, NO_OWNER);
	corpse.MeshTransformation = this.MeshTransformation;
	//corpse->AddAppearance(0, skin);
	//corpse->SetSpeed(RandomX(-1, 1) * 30, RandomX(-20, -60));
	//corpse->SetRDir((-1 + 2 * Random(2)) * RandomX(5, 15));
	corpse->SetDir(GetDir());
	corpse->SetPhase(GetPhase());
	corpse->CopyAnimationPositionFrom(this);
	//corpse->StartSplatter();
	return corpse;
}


/**
 The default death "animation".
 */
func HandleCorpseDefault()
{
	// Create the corpse
	var corpse = CreateCorpse();
	
	// Copy the physics data
	corpse->SetXDir(GetXDir());
	corpse->SetYDir(GetYDir());
	
	// Copy the whole skin to the corpse
	for (var slot = 0; slot < PLAYER_SKIN_SLOT_HEAD; ++slot)
	{
		corpse->AddAppearance(slot, this->RemoveAppearance(slot));
	}
	
	// Start the animation
	corpse->StartSplatter(GetCorpseData().on_ground);
}
