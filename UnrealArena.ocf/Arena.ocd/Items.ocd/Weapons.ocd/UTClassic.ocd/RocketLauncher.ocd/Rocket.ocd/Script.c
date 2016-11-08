#include Library_Projectile

local is_grenade = false;
local ContactCalls = true;

public func Grenade()
{
	if (!ActIdle())
	{
		FatalError("This function should be called only when the object is idle");
	}
	
	SetGraphics("Grenade");
	is_grenade = true;
	
	SetR(Random(360));
	SetRDir(Random(30));

	ScheduleCall(this, "Detonate", 70);
}

private func OnLaunch()
{
	if (is_grenade) SetAction("TravelBallistic");
	else SetAction("Travel");
}

protected func Hit()
{
	//Detonate();
}


public func HitObject(object obj, bool remove, proplist effect)
{
	Detonate();
}

public func ContactBottom()
{
	if(is_grenade)
	{
		Sound("Weapon::Classic::rocket-grenade-floor");
		if (GBackSolid(5,0) || GBackSolid(-5,0)) SetXDir(GetXDir()/-2);
		if (GBackSolid(0,5) || GBackSolid(0,-5)) SetYDir(GetYDir()/-2);
 	}
	else
	{
		Detonate();
	}
}

public func Detonate()
{
	Explode(32, false, damage);
}


public func DoDamageObject(object target)
{
	// do nothing, because the explosion handles everything
}


public func ExplosionEffect(int level, int x, int y, int smoothness)
{
	ExplosionEffectSmoke(level, x, y, smoothness);
}


protected func Travelling()
{
	var self = this;
	
	_inherited();
	
	if (self)
	{
		if (!is_grenade)
		{
			HandleTrail();
			this.MeshTransformation = Trans_Rotate(GetActTime() % 360, 0, 1, 0);
		}
		else
		{
			this.MeshTransformation = Trans_Mul(Trans_Rotate(GetActTime() % 360, 1, 0, 0), Trans_Rotate(10, 0, 0, 1));
		}
	}
}

protected func ControlSpeed()
{
	if (!is_grenade)
	{
		_inherited();
	}
}

local logged_distance;

private func HandleTrail()
{
	var distance = Distance(0, 0, GetXDir(), GetYDir());
	var max_x = +Sin(GetR(), distance / 10);
	var max_y = -Cos(GetR(), distance / 10);
	var off_x = max_x / 2;
	var off_y = max_y / 2;
	var particle_distance = 25;

	for(var i=0; i < distance; i += particle_distance)
	{
		var x = -max_x * i / distance;
		var y = -max_y * i / distance;

		var rand = RandomX(-5, 5);
		var xdir = +Sin(GetR() + rand, 20);
		var ydir = -Cos(GetR() + rand, 20);
		
		var size_thrust = RandomX(6, 8);
		var size_smoke = RandomX(8, 12);

		CreateParticle("Thrust", x + off_x, y + off_y, GetXDir()/2, GetYDir()/2, PV_Random(5, 10), {Prototype = Particles_Thrust(), Size = PV_KeyFrames(0, 0, 4, 100, size_thrust, 1000, size_thrust)});
		CreateParticle("Smoke2", 2 * x - off_x, 2 * y - off_y + 1, xdir, ydir, PV_Random(30, 40), {Prototype = Particles_Thrust(), Size = PV_Linear(size_thrust, size_smoke)});
	}
}
