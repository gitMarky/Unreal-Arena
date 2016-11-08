#include Library_Projectile


private func OnLaunch()
{
	SetAction("TravelBallistic");
}

protected func Hit()
{
	Detonate();
}


public func HitObject(object obj, bool remove, proplist effect)
{
	Detonate();
}

public func ContactBottom()
{
	Detonate();
}

public func Detonate()
{

	var firemode = weapon_ID.firemode_primary;
	
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
	
	Explode(24, false, damage);
}

public func DoDamageObject(object target)
{
	// do nothing, because the explosion handles everything
}

public func ExplosionEffect(int level, int x, int y, int smoothness)
{
	ExplosionEffectBlast(level, x, y, smoothness);
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


func CorpsePhysicsElasticityConstant(){ return 1000;}
