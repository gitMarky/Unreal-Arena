#include Library_Projectile

protected func Initialize()
{
	SetObjectBlitMode(GFX_BLIT_Additive);
}

private func OnLaunch()
{
	SetAction("Travel");
}

private func OnLaunched()
{
	CreateTrail(0, 0);
	trail->SetGraphics("Beam");
	trail->SetFadeSpeed(60);	
}


private func ProjectileColor(int time)
{
	return RGBa(80, 0, 255, 255);
}

private func TrailColor(int time)
{
	return ProjectileColor(time);
}
