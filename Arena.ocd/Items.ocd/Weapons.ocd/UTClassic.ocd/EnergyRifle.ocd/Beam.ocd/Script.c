#include Library_Projectile

local beam_x_old, beam_y_old, beam_x, beam_y;

protected func Initialize()
{
	SetObjectBlitMode(GFX_BLIT_Additive);
}

private func OnLaunch()
{
	beam_x_old = GetX();
	beam_y_old = GetY();
	beam_x = beam_x_old;
	beam_y = beam_y_old;
	SetAction("Travel");
}

private func OnLaunched()
{
//	CreateTrail(0, 0);
//	trail->SetGraphics("Beam");
//	trail->SetFadeSpeed(60);
//
//	SetLightRange(50, 30);
//	SetLightColor(ProjectileColor());
}


private func ProjectileColor(int time)
{
	return RGBa(80, 0, 255, 255);
}

private func TrailColor(int time)
{
	return ProjectileColor(time);
}

private func Travelling()
{
	_inherited();
	DrawTrail();
}

private func DrawTrail()
{
	beam_x_old = beam_x;
	beam_y_old = beam_y;
	beam_x = GetX();
	beam_y = GetY();

	var dx = beam_x_old - beam_x;
	var dy = beam_y_old - beam_y;
	
	var num = Max(1, Distance(0, 0, dx, dy));
	
	for (var i = 0; i < num; i++)
	{
		CreateParticle("Magic", i * dx / num, i * dy / num, 0, 0, 10, Particles_ShockBeam(), 1);
	}
}

private func Particles_ShockBeam()
{
	return
	{
		Size = PV_Linear(4, 0),
	    ForceY = 0,
		DampingY = 0,
		DampingX = 0,
		OnCollision = PC_Die(),
		CollisionVertex = 500,
	    R = 80,
	    G = 0,
	    B = 255,
	    Alpha = PV_Linear(255,0),
		BlitMode = GFX_BLIT_Additive,
	};
}