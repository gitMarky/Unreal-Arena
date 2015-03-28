#include Library_Projectile

protected func Initialize()
{
	SetObjectBlitMode(GFX_BLIT_Additive);
}

public func OnLaunched()
{
	SetAction("Travel");
}