#include Projectile_PlasmaBeam

protected func Initialize()
{
	SetLightRange(10, 20);
	SetLightColor(RGB(0, 255, 0));
}

private func OnLaunch()
{
	SetAction("Travel");
	SetPhase(Random(4));
}

private func HitEffect()
{
	CreateImpactEffect(this.damage, 0, 0, "Magic", Particles_Plasma());
}

public func OnHitLandscape()
{
	Sound("Weapon::Classic::pulse-exp");
	_inherited();
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