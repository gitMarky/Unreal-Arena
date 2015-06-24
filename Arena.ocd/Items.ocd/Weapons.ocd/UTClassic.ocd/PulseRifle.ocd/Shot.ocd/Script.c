#include Library_Projectile

protected func Initialize()
{
	SetLightRange(10, 20);
	SetLightColor(RGB(0,255,0));
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
		Sound("pulse-exp");
		CreateImpactEffect(this.damage, 0, 0, "Magic", Particles_Plasma());
}

private func Particles_Plasma()
{
	return
	{
		Prototype = Particles_Glimmer(),
	    R = 0,
	    G = PV_Linear(255, 128),
	    B = 0,
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
	Delay = 3,
	FacetBase = 0,
	X = 0, Y = 0, Wdt = 10, Hgt = 10,
	NextAction = "Travel",
	PhaseCall = "Travelling",
	Sound = "PulseFly",
},

};