/*--- Gore ---*/

local BorderBound= 7;
local blood_type;

func Hit()
{
	if (blood_type == "Machine")
	{
		Sound("Hit*");
	}
	else
	{
		Sound("gibP*");
	}
}

func Launch(string type)
{
	if (MOD_NoBlood())
		return RemoveObject();
	
	blood_type = type;
	SetGraphics(type);
	
	SetAction("Flying");
	SetDir(Random(5));
	
	ScheduleCall(this, this.Splatter, 1, 1);
}

func Splatter()
{
	if (MOD_NoBlood()) return RemoveObject();
	
	if (GetActTime() > 200)
	{
		if (!IsDisintegrating()) Disintegrate(20, 50, -10);
	}
	else
	{
		EffectBloodSpray(1, 0, 0, 0);
		//CreateParticle("Blood",0,0,0,0,10+Random(30),RGBaRandom(BloodFXColor(blood_type)[0],BloodFXColor(blood_type)[1]));	
		ScheduleCall(this, this.Splatter, 1, 1);
	}
}

func ContactBottom()
{
	if (!Random(3))
		SetXDir(GetXDir() / 2);
	SetYDir(GetYDir() / -2);
	return BouncePhysics();
}

func ContactTop()
{
	SetYDir(GetYDir() / -2);
	return BouncePhysics();
}

func ContactLeft()
{
	SetXDir(GetXDir() / -2);
	return BouncePhysics();
}

func ContactRight()
{
	SetXDir(GetXDir() / -2);
	return BouncePhysics();
}

func BouncePhysics()
{
	SetRDir(GetRDir() * -2);
	if (GBackSolid(5, 0) || GBackSolid(-5, 0))
		SetXDir(GetXDir() / -2);
	if (GBackSolid(0, 5) || GBackSolid(0, -5))
		SetYDir(GetYDir() / -2);
	if (GBackSolid(10, 0) || GBackSolid(-10, 0))
		SetXDir(GetXDir() / -2);
	if (GBackSolid(0, 10) || GBackSolid(0, -10))
		SetYDir(GetYDir() / -2);
	return true;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// actions

local ActMap = {
Flying = {
	Prototype = Action,
	Name = "Flying",
	Directions = 5,
	Delay = 0,
	X = 0,
	Y = 0,
	Wdt = 9,
	Hgt = 7
},

};
