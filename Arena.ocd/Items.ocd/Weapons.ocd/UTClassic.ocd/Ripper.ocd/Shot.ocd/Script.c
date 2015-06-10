#include Library_Projectile

local explodes = false;

protected func Initialize()
{
	CreateTrail(30, 3);
}

public func SetSecondary()
{
	explodes = true;
}

private func OnLaunch()
{
	SetAction("Travel");
}

public func OnHitObject(object target)
{
	if (explodes)
	{
		Detonate();
	}
	//CreateImpactEffect(this.damage);
}

public func OnHitLandscape()
{
	if (explodes)
	{
		Detonate();
	}
	else
	{
		Sound("ripper-hit");
		CreateImpactEffect(this.damage);
	}
}

private func Detonate()
{
	Explode(10);
}
