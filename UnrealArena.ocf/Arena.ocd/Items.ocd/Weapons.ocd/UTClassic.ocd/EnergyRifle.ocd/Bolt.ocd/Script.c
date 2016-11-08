#include Library_Projectile

protected func Initialize()
{
	SetLightRange(10, 20);
	SetLightColor(GetProjectileColor());
	SetColor(GetProjectileColor());
}

private func GetProjectileColor()
{
	if (IsInstaGibConfigured())
	{
		return PROJECTILE_Color_InstaGib;
	}
	else
	{
		return PROJECTILE_Color_EnergyRifle;
	}
}

private func OnLaunch()
{
	SetAction("Travel");
	SetPhase(Random(4));
}

public func IsProjectileTarget(object projectile, object shooter)
{
	return projectile->GetID() == Projectile_ShockBeam;
}

public func OnHit(int damage, int damage_type, object projectile, id weapon)
{
	Detonate(true);
}

public func OnHitObject(object target)
{
	Detonate(false);
}

public func OnHitLandscape()
{
	Detonate(false);
}

private func Detonate(bool large)
{
	Sound("Weapon::Classic::shock-exp");
	var factor = (1 + large);
	Explode(20 * factor, true, damage * factor);
}

public func DoDamageObject(object target)
{
	// do nothing, because the explosion handles everything
}

public func ExplosionEffect(int level, int x, int y, int smoothness)
{
	ExplosionEffectBlast(level, x, y, smoothness);
}


public func GetExplosionColor()
{
	var color = GetProjectileColor();
	var color_start = RGBa( 8 * GetRGBaValue(color, RGBA_RED) / 10,
							8 * GetRGBaValue(color, RGBA_GREEN) / 10,
							8 * GetRGBaValue(color, RGBA_BLUE) / 10,
							175);

	return {
 				Center = SetRGBaValue(color, RandomX(125, 175), RGBA_ALPHA),
 				AmbientStart = color_start,
 				AmbientEnd = SetRGBaValue(color, 125, RGBA_ALPHA),
 		   };
}

private func Particles_Plasma()
{
	return
	{
		Prototype = Particles_Glimmer(),
	    R = GetRGBaValue(GetProjectileColor(), RGBA_RED),
	    G = GetRGBaValue(GetProjectileColor(), RGBA_GREEN),
	    B = GetRGBaValue(GetProjectileColor(), RGBA_BLUE),
	    Alpha = PV_Random(255, 0, 3),
		BlitMode = GFX_BLIT_Additive,
	};
}


local ActMap = {
Travel = {
	Prototype = Action,
	Name = "Travel",
	Procedure = DFA_NONE,
	Length = 4,
	Delay = 4,
	FacetBase = 0,
	X = 0, Y = 0, Wdt = 15, Hgt = 15,
	NextAction = "Travel",
	PhaseCall = "Travelling",
},
};


func CorpsePhysicsElasticityConstant(){ return 1000;}
