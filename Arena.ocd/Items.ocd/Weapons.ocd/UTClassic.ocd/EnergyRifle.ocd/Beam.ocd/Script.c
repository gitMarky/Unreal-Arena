#include Library_Projectile

local beam_x_old, beam_y_old, beam_x, beam_y;

protected func Initialize()
{
	SetObjectBlitMode(GFX_BLIT_Additive);
}

public func OnHitObject(object target)
{
	CreateImpactEffect(this.damage / 3, 0, 0, "Magic", Particles_Plasma());
}

public func OnHitLandscape()
{
		Sound("shock-exp");
		CreateImpactEffect(this.damage / 3, 0, 0, "Magic", Particles_Plasma());
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

public func OnHitScan(int x_start, int y_start, int x_end, int y_end)
{
	var laser = CreateObject(LaserEffect, x_start - GetX(), y_start - GetY(), NO_OWNER);
	
	laser->Line(x_start, y_start, x_end, y_end)
		 ->SetWidth(6)
		 ->SetLifetime(20)
		 ->Color(RGB(80, 0, 255))
		 ->Activate();
}
