#include Library_Projectile

local laser_beam;

public func OnHitObject(object target)
{
	HitEffect();
}

public func OnHitLandscape()
{
	HitEffect();
}

private func HitEffect()
{
	CreateImpactEffect(7, 0, 0, "Magic", Particles_Plasma());
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

public func OnHitScan(int x_start, int y_start, int x_end, int y_end)
{
	if (!laser_beam) FatalError("This function should not be called if the weapon has no beam active");

	laser_beam->SetPosition(x_start, y_start);
	laser_beam->Line(x_start, y_start, x_end, y_end);
	laser_beam->Update();
}

public func SetBeam(object beam)
{
	laser_beam = beam;
}
