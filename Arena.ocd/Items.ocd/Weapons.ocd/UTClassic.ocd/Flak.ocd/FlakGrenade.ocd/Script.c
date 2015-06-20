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
	
	var firemode = weapon_ID.fire_modes.primary;
	
	// launch the single projectiles
	for (var i = 0; i < firemode.projectile_number; i++)
	{
		var projectile = CreateObject(firemode.projectile_id, 0, 0, user->GetController());
	
		projectile->Shooter(user)
				  ->Weapon(weapon_ID)
				  ->DamageAmount(firemode.damage)
		          ->DamageType(firemode.damage_type)
		          ->Velocity(RandomX(30, 40))
				  ->Range(firemode.projectile_range)
		          ->Launch(RandomX(-180, 180));
	}
	
	
	RemoveObject();
	return true;
}

protected func Travelling()
{
	var self = this;
	
	_inherited();
	
	if (self)
	{
		var x = +Sin(GetR(), -2);
		var y = -Cos(GetR(), -2);
	
		var amount = 5 - Max(0, GetActTime() - 5) / 5;

		if (amount > 0) CreateParticle("Fire", PV_Random(x - 2, x + 2), PV_Random(y -2, y + 2), GetXDir()/2, GetYDir()/2 -1, PV_Random(5, 15), Particles_Glimmer(), amount);
	}
}