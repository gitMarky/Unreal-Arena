#include Library_Projectile

protected func Initialize()
{
	SetObjectBlitMode(GFX_BLIT_Additive);
}

public func OnLaunch()
{	
	SetAction("TravelBallistic");
}

public func OnLaunched()
{
	CreateTrail(0, 0, 2, 80);	
}