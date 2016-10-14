#include Library_Projectile

static const PROJECTILE_Impact_Size_EnergyRifle = 15;

protected func Initialize()
{
	SetObjectBlitMode(GFX_BLIT_Additive);
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

public func OnHitObject(object target)
{
	CreateImpactEffect(PROJECTILE_Impact_Size_EnergyRifle, 0, 0, "Magic", Particles_Plasma());
}

public func OnHitLandscape()
{
		Sound("Weapon::Classic::shock-exp");
		CreateImpactEffect(PROJECTILE_Impact_Size_EnergyRifle, 0, 0, "Magic", Particles_Plasma());
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

public func OnHitScan(int x_start, int y_start, int x_end, int y_end)
{
	var laser = CreateObject(LaserEffect, x_start - GetX(), y_start - GetY(), NO_OWNER);
	
	laser->Line(x_start, y_start, x_end, y_end)
		 ->SetWidth(6)
		 ->SetLifetime(20)
		 ->Color(GetProjectileColor())
		 ->Activate();
}
