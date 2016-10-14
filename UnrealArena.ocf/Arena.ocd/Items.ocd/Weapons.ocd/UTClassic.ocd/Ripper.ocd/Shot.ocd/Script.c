#include Library_Projectile

local explodes = false;
local reflections = 3;
local ContactCalls = true;

protected func Initialize()
{
	CreateTrail(30, 3);
	var scale = 1500;
	this.MeshTransformation = Trans_Scale(scale, scale, scale);
}

public func SetSecondary()
{
	explodes = true;
}

private func OnLaunch()
{
	SetAction("Travel");
	StayOnHit();
}

public func OnHitObject(object target)
{
	if (explodes)
	{
		Detonate();
	}
	else
	{
		RemoveObject();
	}
}

public func OnHitLandscape()
{
	if (explodes)
	{
		Detonate();
	}
	else
	{
		Reflect();
	}
}

private func Detonate()
{
	Explode(10, false, damage);
}

public func ExplosionEffect(int level, int x, int y, int smoothness)
{
	ExplosionEffectBlast(level, x, y, smoothness);
}

private func Reflect()
{
	Sound("Weapon::Classic::ripper-hit");
	CreateImpactEffect(5);
	
	if(reflections)
	{
		var distance = 3;
		if (GBackSolid(-distance, 0) || GBackSolid(+distance, 0)) velocity_x = -velocity_x;
		if (GBackSolid(0, -distance) || GBackSolid(0, +distance)) velocity_y = -velocity_y + RandomX(-160, 150);
		
		ControlSpeed();
		//if (GetAction() != "TravelBallistic") SetAction("TravelBallistic");

		reflections--;
		if (reflections <= 0) RemoveOnHit();
	}
}
