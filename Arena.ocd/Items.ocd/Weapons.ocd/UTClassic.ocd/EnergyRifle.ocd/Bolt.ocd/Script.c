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
	Hit();
}

public func OnHitObject(object target)
{
	CreateImpactEffect(this.damage, 0, 0, "Magic", Particles_Plasma());
}

public func OnHitLandscape()
{
		Sound("shock-exp");
		CreateImpactEffect(this.damage, 0, 0, "Magic", Particles_Plasma());
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