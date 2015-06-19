#include Library_Projectile

protected func Initialize()
{
	SetObjectBlitMode(GFX_BLIT_Additive);
}

private func OnLaunch()
{
}

private func OnLaunched()
{
}

public func OnHitLandscape()
{
		Sound("BulletHitGround?");
		CreateImpactEffect(this.damage / 3);
}

