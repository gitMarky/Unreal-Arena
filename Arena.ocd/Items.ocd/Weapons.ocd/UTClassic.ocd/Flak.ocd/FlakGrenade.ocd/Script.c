#include Library_Projectile


private func OnLaunch()
{
	SetAction("TravelBallistic");
}

protected func Hit()
{
	Detonate();
}


public func HitObject(object obj, bool no_remove)
{
	Detonate();
}

public func ContactBottom()
{
	Detonate();
}

public func Detonate()
{
	//Explosion( this,  64, 24, false );
	
	var firemode = weapon_ID.fire_modes.default;
	
	// launch the single projectiles
	for (var i = 0; i < firemode.projectile_number; i++)
	{
		var projectile = CreateObject(firemode.projectile_id, 0, 0, user->GetController());
	
		projectile->Shooter(user)
				  ->Weapon(weapon_ID)
				  ->Damage(firemode.damage)
		          ->DamageType(firemode.damage_type)
		          ->Velocity(firemode.projectile_speed)
				  ->Range(firemode.projectile_range)
		          ->Launch(RandomX(-180, 180));
	}
	
	
	RemoveObject();
	return true;
}
