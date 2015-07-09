
local master, x, y, rot, dist, ang;

public func Hover()
{
	if(!master) return RemoveObject();

	var _x,_y,r;

	r = ang + rot + master->GetR();

	_x=+Sin(r,dist);
	_y=-Cos(r,dist);

	SetXDir();
	SetYDir();

	if(GetActTime() > 30 ) DoCon(-1);

	SetPosition(master->GetX()+x, master->GetY()+y);

	DoFlameParticle();
}

public func Fall()
{
	DoFlameParticle();
	if(GetActTime() > 30) DoCon(-2);
}

private func DoFlameParticle()
{
	var size = Max(1, GetCon() * 7 / 100);
	
	var redness = PV_Random(100, 255);

	CreateParticle("Thrust", PV_Random(-size/2, size/2),
						     PV_Random(-size/2, size/2),
						     GetXDir()/2, GetYDir()/2,
						     PV_Random(8, 16),
						     {Prototype = Particles_Thrust(),
						      R = 255,
						      G = redness,
						      B = redness,
						      Alpha = PV_Linear(160, 0),
						      //Size = PV_KeyFrames(0, 0, size, 600, size, 1000, size / 3),
						      //Size = PV_Linear(2 * size / 3, 3 * size / 2),
						      Size = PV_Linear(PV_Random(2 * size / 3, size), PV_Random(size, 3 * size / 2)),
						     ForceX = PV_KeyFrames(10, 0, PV_Random(-12, 12), 333, PV_Random(-8, 8), 666, PV_Random(-6, 6), 1000, PV_Random(-6, 6)),
						     ForceY = PV_KeyFrames(10, 0, PV_Random(-16, -2), 333, PV_Random(-24, -3), 666, PV_Random(-32, -4), 1000, PV_Random(-40, -5))},
						     RandomX(3, 5));
}


public func SetUpFlame( object obj, int dam, int _x, int _y)
{
	master = obj;
	x = _x;
	y = _y;

	dist = Distance(0,0,x,y);
	ang = Angle( 0, 0, x, y);

	rot = - master->GetR();

	//Incinerate();

	SetCon(2*dam);

	SetAction("Hover", master);
	SetObjectLayer(this);
}

public func SetMaster( object obj )
{
	master = obj;
	SetAction("Hover",master);
}

public func SetUpSpark( int dam, int _x, int _y)
{
	//Incinerate();

	SetCon(2*dam);

	SetXDir(_x);
	SetYDir(_y);

	SetAction("Fall");
	SetObjectLayer(this);
}

public func Hit()
{
	if(GetAction() == "Fall" && !GetEffect("IntRemove", this))
	{
		if (Random(3))
		{
			RemoveObject();
		}
		else
			AddEffect("IntRemove", this, 1, RandomX(30, 60), this);
	}
}

private func FxIntRemoveTimer()
{
	RemoveObject();
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// properties

local Name = "$Name$";
local Description = "$Description$";
local NoBurnDecay = 1;

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// actions

local ActMap = {
Hover = {
	Prototype = Action,
	Name = "Hover",
	Procedure = DFA_FLOAT,
	Directions = 1,
	Length = 1,
	Delay = 1,
	X = 0,
	Y = 0,
	Wdt = 5,
	Hgt = 5,
	NextAction = "Hover",
	StartCall = "Hover",
},

Fall = {
	Prototype = Action,
	Name = "Fall",
	Procedure = DFA_NONE,
	Directions = 1,
	Length = 1,
	Delay = 1,
	X = 0,
	Y = 0,
	Wdt = 5,
	Hgt = 5,
	NextAction = "Fall",
	StartCall = "Fall",
},

};
