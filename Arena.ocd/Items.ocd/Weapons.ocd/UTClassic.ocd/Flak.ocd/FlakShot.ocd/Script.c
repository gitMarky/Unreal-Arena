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
	var self = this;
	
	if(reflections)
	{
		if (!GetXDir()) SetXDir(RandomX(-10, 10));
		if (!GetYDir()) SetYDir(RandomX(-10, 10));

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

public func ContactBottom()
{
	Hit();
}