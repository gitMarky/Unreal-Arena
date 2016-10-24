func OnWeaponDamageShooter(object projectile, int damage_amount, int damage_type)
{
	var is_headshot = false;
	var remaining_health = GetEnergy() - damage_amount;

// TODO
//	if (projectile)
//	{
//		LastDamageWeapon(pProjectile->~GetWeaponID(), true);
//	}
	
	// hit in the head?
	if (damage_type & DMG_Headshot)
	{
		if (Inside(projectile->GetX() - GetX(), -7, 7) && Inside(projectile->GetY() - GetY(), -10, -6))
			is_headshot = true;
	}
	
	// gore effects
	if (damage_amount > (this.MaxEnergy / 10000))
	{
		if (MOD_MoreGore())
			CastGore(ID_Gore_Chunk, 1, 60 + Abs(damage_amount) - Random(Abs(damage_amount)));
	}
	
	if (damage_amount > (this.MaxEnergy / 5000))
	{
		if (MOD_MoreGore()) 		CastGore(ID_Gore_Chunk, 1, 60 + Abs(damage_amount) - Random(Abs(damage_amount)));
		if (MOD_MoreGore() > 10)	CastGore(ID_Gore_Chunk, 1, 60 + Abs(damage_amount) - Random(Abs(damage_amount)));
		if (MOD_MoreGore() > 18)	CastGore(ID_Gore_Chunk, 1, 60 + Abs(damage_amount) - Random(Abs(damage_amount)));

		CreateObject(Effect_BlazingFlame, 0, 0, -1)->~SetUpFlame(this, damage_amount, projectile->GetX() - GetX(), projectile->GetY() - GetY());
	}
	
	if (!MOD_NoBlood())
	{
		var divisor = 3 * (1 + MOD_FastBullets());
		var type = BloodFXColor(CrewGetBlood(this));
		CastParticles
		(
			"Blood",
			damage_amount * 3,
			30,
			projectile->GetX() - GetX(),
			projectile->GetY() - GetY(),
			10,
			40,
			type[0],
			type[1]
		);
		FlingGore
		(
			ID_Gore_BloodStream,
			projectile->GetXDir() / divisor,
			projectile->GetYDir() / divisor,
			projectile->GetX() - GetX(),
			projectile->GetY() - GetY()
		);
	}
	

	// Treffer: Kopf
	if (is_headshot)
	{
		// TODO: bDeathHeadshot = true;
		remaining_health = 0;
	}
	
	// blow the clonk away if he is alive and it was an explosion
	if (remaining_health > 0)
	{
		if (damage_type & DMG_Explosion)
			if (GetID(projectile) != WC6I)
			{
				var divisor = 1 + MOD_FastBullets();

				SetSpeed(RandomX(-5, 5) + projectile->GetXDir() / divisor,
					     RandomX(-5, 5) + projectile->GetYDir() / divisor);
				SetAction("Tumble");
			}
	}
	else
	{
		// TODO: OnDeathExtended(damage_amount, damage_type, projectile, is_headshot);
	}
}

func MOD_MoreGore()
{
	// TODO
}

func MOD_NoBlood()
{
	// TODO
}

func MOD_FastBullets()
{
	// TODO
}

func CastGore()
{
	// TODO
}

func FlingGore()
{
	// TODO
}

func CrewGetBlood()
{
	// TODO
}