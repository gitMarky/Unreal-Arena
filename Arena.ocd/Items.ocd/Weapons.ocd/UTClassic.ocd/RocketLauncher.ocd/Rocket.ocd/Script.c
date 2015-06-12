#include Library_Projectile

local is_grenade = false;

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


public func HitObject(object obj, bool no_remove)
{
	Detonate();
}

public func ContactBottom()
{
	if(is_grenade)
	{
		Sound("Hit?");
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
	//Explosion( this,  64, 24, false );
	Explode(30); // dummy explosion
	return true;
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

private func HandleTrail()
{
	var distance = Distance(0, 0, GetXDir(), GetYDir());
	var max_x = +Sin(GetR(), distance / 10);
	var max_y = -Cos(GetR(), distance / 10);
	var particle_distance = 50;
    
	for(var i=0; i < distance; i += particle_distance)
	{
		var x = -max_x * i / distance;
		var y = -max_y * i / distance;

		var rand = RandomX(-5, 5);
		var xdir = +Sin(GetR() + rand, 20);
		var ydir = -Cos(GetR() + rand, 20);
		
		CreateParticle("Thrust", x, y, GetXDir()/2,GetYDir()/2, PV_Random(5, 10), {Prototype = Particles_Thrust(), Size = PV_Random(8, 12)});
		CreateParticle("Smoke2", x, y, xdir, ydir, PV_Random(30, 40), {Prototype = Particles_Thrust(), Size = PV_Random(12, 14)});
	}
}