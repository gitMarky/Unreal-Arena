
func OnWeaponDamageShooter(object projectile, int damage_amount, int damage_type)
{
	var is_headshot = false;
	var remaining_health = GetEnergy() - damage_amount;
	
	var hit_x = projectile->GetX() - GetX();
	var hit_y = projectile->GetY() - GetY();
	
	Log("Damaged for %d at %d/%d", damage_amount, hit_x, hit_y);
	

// TODO
//	if (projectile)
//	{
//		LastDamageWeapon(pProjectile->~GetWeaponID(), true);
//	}
	
	// hit in the head?
	if (damage_type & DMG_Headshot)
	{
		if (Inside(hit_x, -7, 7) && Inside(hit_y, -10, -6))
		{
			is_headshot = true;
			remaining_health = 0;
			// TODO: bDeathHeadshot = true;
		}
	}
	
	DoGoreEffects(projectile, damage_amount, is_headshot);	

	// blow the clonk away if he is alive and it was an explosion
	if (remaining_health > 0)
	{
		if (damage_type & DMG_Explosion)
		{
			var divisor = 1 + MOD_FastBullets();

			SetSpeed(RandomX(-5, 5) + projectile->GetXDir() / divisor,
				     RandomX(-5, 5) + projectile->GetYDir() / divisor);
			SetAction("Tumble");
		}
	}
	else
	{
		this->~OnDeathExtended(damage_amount, damage_type, projectile, is_headshot);
		
		// Kill him for sure
		DoEnergy(-100000, false, FX_Call_DmgScript, projectile->GetController());
	}
}


func CrewGetBlood()
{
	// TODO
}

func DoGoreEffects(object projectile, int damage_amount, bool is_headshot)
{
	if (MOD_NoBlood()) return;

	var hit_x = projectile->GetX() - GetX();
	var hit_y = projectile->GetY() - GetY();

	var is_heavy_hit = damage_amount > this.MaxEnergy / 10000;
	var is_critical_hit = damage_amount > this.MaxEnergy / 5000;

	// gore effects
	if (is_heavy_hit && MOD_MoreGore())
	{
		CastGore(damage_amount);
	}

	if (is_critical_hit)
	{
		CreateObject(Effect_BlazingFlame, 0, 0, NO_OWNER)->~SetUpFlame(this, damage_amount, hit_x, hit_y);

		if (MOD_MoreGore()) 		CastGore(damage_amount);
		if (MOD_MoreGore() > 10)	CastGore(damage_amount);
		if (MOD_MoreGore() > 18)	CastGore(damage_amount);
	}
	
	// blood effects
	var divisor = 3 * (1 + MOD_FastBullets());
	EffectBloodSpray(damage_amount, 10, hit_x, hit_y);
	EffectBloodStream(hit_x, hit_y, projectile->GetXDir() / divisor, projectile->GetYDir() / divisor);
	
	if (is_headshot)
	{
		for (var amount = MOD_MoreGore(); amount > 0; amount -= 2)
		{
			CastGoreHeadshot();
		}
	}
	
}

func CastGore(int damage_amount)
{
	EffectCastGore(1, 60 + Abs(damage_amount) - Random(Abs(damage_amount)));
}

func CastGoreHeadshot()
{
	EffectGoreChunk(0, -7, RandomX(-10, +10) + GetXDir() / 5, RandomX(-10, -35) + GetYDir() / 5 - 10);
}
