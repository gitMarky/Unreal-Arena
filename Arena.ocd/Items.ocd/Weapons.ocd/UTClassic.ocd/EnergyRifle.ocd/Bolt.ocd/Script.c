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
		Sound("pulse-exp");
		CreateImpactEffect(this.damage, 0, 0, "Magic", Particles_Plasma());
}

private func Particles_Plasma()
{
	return
	{
		Size = PV_Linear(2, 0),
	    ForceY = GetGravity(),
		DampingY = PV_Linear(1000,700),
		DampingX = PV_Linear(1000,700),
		Stretch = PV_Speed(1000, 500),
		Rotation = PV_Direction(),
		OnCollision = PC_Die(),
		CollisionVertex = 500,
	    R = 80,
	    G = 0,
	    B = 255,
	    Alpha = PV_Random(255,0,3),
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