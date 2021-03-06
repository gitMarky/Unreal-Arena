
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
		death_physics = nil,
		animation_speed = 1000,
	};
}

func IsCorpse()
{
	return GetCorpseData().is_corpse;
}


func IsHeadshot(object projectile, int damage_type)
{
    var hit_x = projectile->GetX() - GetX();
    var hit_y = projectile->GetY() - GetY();

	return (damage_type & DMG_Headshot)
	    && Inside(hit_x, -7, 7)
	    && Inside(hit_y, -10, -6);
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
		this->OnDeathGoreEffects();
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

		GetCorpseData().corpse_headshot = IsHeadshot(projectile, damage_type);

		if (damage_type & DMG_Explosion)
		{
			if (Inside(projectile->GetY() - GetY(), -6, 1))
			{
				GetCorpseData().corpse_blasted_body = true;
				GetCorpseData().corpse_lost_arm_l = !Random(4);
				GetCorpseData().corpse_lost_arm_r = !Random(4);
			}
			else if (Inside(projectile->GetY() - GetY(), 1, 10))
			{
				GetCorpseData().corpse_blasted_legs = true;
				GetCorpseData().corpse_lost_arm_l = !Random(12);
				GetCorpseData().corpse_lost_arm_r = !Random(12);
			}

			GetCorpseData().corpse_blasted = true;
		}

		GetCorpseData().death_physics = Calc_ProjectileCollision(this, projectile, BoundBy(projectile->~CorpsePhysicsElasticityConstant(), 0, 1000));

	}

	if (GetContact(-1) & CNAT_Bottom)
	{
		GetCorpseData().on_ground = true;
	}

	GetCorpseData().animation_speed = 1000 - 3 * Distance(GetCorpseData().hit_xdir, GetCorpseData().hit_ydir);

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
 Handles all gore effects when the player dies.
 */
func OnDeathGoreEffects()
{
	if (IsCorpse()) return;

	EffectCastBloodStream(MOD_MoreGore() * 2, 40 + Random(GetCorpseData().damage_amount));
	EffectCastGore(MOD_MoreGore() / 3, 60 + Random(GetCorpseData().damage_amount));

	if (GetCorpseData().corpse_headshot)
	{
		for (var amount = MOD_MoreGore(); amount > 0; amount -= 2)
		{
			EffectGoreChunk(0, -5, RandomX(-10, 10) + GetXDir() / 5, RandomX(-10, -35) + GetYDir() / 5);
		}
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
	if (GetCorpseData().corpse_headshot)
	{
		HandleCorpseHeadshot();
	}
	else if (GetCorpseData().corpse_blasted)
	{
		HandleCorpseBlasted();
	}
	else // Standard death
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

	var cl_legs = nil;


	// Flammen-Effekte auf die Leiche uebertragen
	var flame, flames = FindObjects(Find_ID(Effect_BlazingFlame), Find_Action("Hover"), Find_ActionTarget(this));
	for (flame in flames) 
	{
		flame->~SetMaster(cl_legs);
	}

	/* Teile anpassen */
	var deathcam_obj = cl_legs;

	//------------------------------------------
	// follow the corpse

	// "Death-Cam" an
	SetPlrView(GetOwner(), deathcam_obj);
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
	corpse->SetDir(GetDir());
	corpse->SetPhase(GetPhase());
	corpse->SetColor(GetColor());
	corpse->CopyAnimationPositionFrom(this);
	return corpse;
}


func HandleCorpsePhysics(array parts)
{
	var data = GetCorpseData().death_physics;

	for (var part in parts)
	{
		// Copy the physics data
		if (data)
		{
			part->SetXDir(data.XDir, data.Precision);
			part->SetYDir(data.YDir, data.Precision);
			part->SetRDir(5 * data.RDir, data.Precision);
		}
		else
		{
			part->SetXDir(GetXDir());
			part->SetYDir(GetYDir());
		}
	}
}


/**
 The default death "animation".
 */
func HandleCorpseDefault()
{
	// Create the corpse
	var corpse = CreateCorpse();
	HandleCorpsePhysics([corpse]);

	// Copy the whole skin to the corpse
	for (var slot = 0; slot <= PLAYER_SKIN_SLOT_HEAD; ++slot)
	{
		corpse->AddAppearance(slot, this->RemoveAppearance(slot));
	}

	// Start the animation
	corpse->StartSplatter(GetCorpseData().animation_speed, GetCorpseData().on_ground);
}


func HandleCorpseHeadshot()
{
	// Create the corpse
	var corpse = CreateCorpse();
	var head = CreateCorpse();

	// Copy the physics data
	HandleCorpsePhysics([corpse, head]);

	// Copy the skin data for the body
	for (var slot = 0; slot < PLAYER_SKIN_SLOT_HEAD; ++slot)
	{
		corpse->AddAppearance(slot, this->RemoveAppearance(slot));
	}

	// Copy the skin data for the head
	head->AddAppearance(0, this->RemoveAppearance(PLAYER_SKIN_SLOT_HEAD));

	// Start the animation
	corpse->StartSplatter(GetCorpseData().animation_speed, GetCorpseData().on_ground);
	head->StartSplatter(GetCorpseData().animation_speed, false);
	head->VertexSetupHead();

	// Additional physics
	var divisor = 1 + MOD_FastBullets();
	var rdir = (GetCorpseData().hit_xdir + GetCorpseData().hit_ydir) / (10 * divisor);

	head->AddSpeed(0, -Random(20) * 100, 1000);
	head->SetRDir(rdir);
}


func HandleCorpseBlasted()
{
	// Create the corpse
	var legs = CreateCorpse();
	var body = CreateCorpse();
	var parts = [legs, body];

	var arml, armr;
	if (GetCorpseData().corpse_lost_arm_l)
	{
		arml = CreateCorpse();
		arml->AddAppearance(0, this->RemoveAppearance(PLAYER_SKIN_SLOT_ARML));
		arml->VertexSetupArmL();
		PushBack(parts, arml);
	}
	if (GetCorpseData().corpse_lost_arm_r)
	{
		armr = CreateCorpse();
		armr->AddAppearance(0, this->RemoveAppearance(PLAYER_SKIN_SLOT_ARMR));
		armr->VertexSetupArmR();
		PushBack(parts, armr);
	}

	// Copy the physics data
	HandleCorpsePhysics(parts);

	// Copy the skin data
	for (var slot = PLAYER_SKIN_SLOT_BODY; slot <= PLAYER_SKIN_SLOT_HEAD; ++slot)
	{
		if (slot == PLAYER_SKIN_SLOT_ARML && GetCorpseData().corpse_lost_arm_l) continue;
		if (slot == PLAYER_SKIN_SLOT_ARMR && GetCorpseData().corpse_lost_arm_r) continue;

		body->AddAppearance(slot, this->RemoveAppearance(slot));
	}

	legs->AddAppearance(0, this->RemoveAppearance(PLAYER_SKIN_SLOT_LEGS));

	// Start the animation
	body->VertexSetupBody();
	legs->VertexSetupLegs();
	legs->StartSplatter(GetCorpseData().animation_speed, GetCorpseData().on_ground);
	body->StartSplatter(GetCorpseData().animation_speed, false);
	if (arml) arml->StartSplatter(GetCorpseData().animation_speed, false);
	if (armr) armr->StartSplatter(GetCorpseData().animation_speed, false);

	// Additional physics
	var ydir_variance = 5;
	var rdir = (GetCorpseData().hit_xdir + GetCorpseData().hit_ydir);
	legs->SetRDir(rdir);
	body->AddSpeed(RandomX(-5, 5), -Random(ydir_variance));
	legs->AddSpeed(RandomX(-5, 5), 0);	
	if (arml) arml->AddSpeed(RandomX(-5, 5), -Random(ydir_variance) - ydir_variance);
	if (armr) armr->AddSpeed(RandomX(-5, 5), -Random(ydir_variance) - ydir_variance);

	var xdir_corpse = GetCorpseData().death_physics.XDir;
	var ydir_corpse = GetCorpseData().death_physics.YDir;

	// Gore effects?
	if (GetCorpseData().corpse_blasted_body)
	{
		// TODO deathcam_obj = cl_body;

		EffectGoreChunk(RandomX(-3, +3), RandomX(-3, +3), xdir_corpse, ydir_corpse - Random(ydir_variance));
		EffectGoreChunk(RandomX(-3, +3), RandomX(-3, +3), xdir_corpse, ydir_corpse - Random(ydir_variance));
		EffectGoreChunk(RandomX(-3, +3), RandomX(-3, +3), xdir_corpse, ydir_corpse - Random(ydir_variance));
	}
	if (GetCorpseData().corpse_blasted_legs)
	{
		// TODO deathcam_obj = cl_body;

		EffectGoreChunk(RandomX(-3, +3), RandomX(-3, +3), xdir_corpse, ydir_corpse - Random(ydir_variance));
		EffectGoreChunk(RandomX(-3, +3), RandomX(-3, +3), xdir_corpse, ydir_corpse - Random(ydir_variance));
		EffectGoreChunk(RandomX(-3, +3), RandomX(-3, +3), xdir_corpse, ydir_corpse - Random(ydir_variance));
	}
}
