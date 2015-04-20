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
		SetXDir(-GetXDir()/2);
		SetYDir(-GetYDir()/2);
		reflections--;
	}
	
	if(self)
	{	
		OnHitLandscape();
		
		if (remove_on_hit) RemoveObject();
	}
}

/**
public func ContactBottom()
{
	Hit();
}
*/