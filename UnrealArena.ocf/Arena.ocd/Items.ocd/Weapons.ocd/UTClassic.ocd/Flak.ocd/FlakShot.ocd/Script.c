#include Library_Projectile

local reflections;

private func OnLaunch()
{
	SetAction("TravelBallistic");
	reflections = 5;
	remove_on_hit = false;
	SetLightRange(5, 5);
}

protected func Hit()
{
}

private func Reflect()
{
	var self = this;
	
	if(reflections)
	{
		if (!GetXDir()) SetXDir(RandomX(-10, 10));
		if (Abs(GetYDir()) < 5) SetYDir(RandomX(-10, 10));

		SetXDir(-GetXDir()/2);
		SetYDir(-GetYDir()/2);
		reflections--;
		
		if (reflections <= 0) remove_on_hit = true;
	}
	
	if(self)
	{	
		OnHitLandscape();
		
		if (remove_on_hit) RemoveObject();
	}
}

protected func ContactBottom()
{
	Reflect();
}

protected func Travelling()
{
	var self = this;
	
	_inherited();
	
	if (self && GetActTime() < 10)
	{
		var x = +Sin(GetR(), -2);
		var y = -Cos(GetR(), -2);
	
		CreateParticle("Fire", PV_Random(x - 2, x + 2), PV_Random(y -2, y + 2), GetXDir()/2, GetYDir()/2 -1, PV_Random(5, 15), Particles_Glimmer());
	}
}

local ContactCalls = true;