#include Library_Projectile

protected func Initialize()
{
	SetLightRange(10, 20);
	SetLightColor(RGB(80,0,255));
	SetColor(RGB(80,0,255));
}

private func OnLaunch()
{
	SetAction("Travel");
	SetPhase(Random(4));
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
	    R = 80,
	    G = 0,
	    B = 255,
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