/**
 Flame effect. Can attach to an object or fall freely.
 @author Marky
 @version 0.1.0
 */


local master, master_angle, pos_x, pos_y , pos_distance, pos_angle;

/**
 Attaches the flame to an object.
 @par target The flame will be attached to this object.
 @par size The flame will be this large, in 1/10 of a pixel.
 @par x The position of the flame, in x direction, relative to the object.
 @par y The position of the flame, in y direction, relative to the object.
 */
public func SetUpFlame(object target, int size, int x, int y)
{
	pos_x = x;
	pos_y = y;

	pos_distance = Distance(0, 0, pos_x, pos_y);
	pos_angle = Angle(0, 0, pos_x, pos_y);

	master_angle -= target->GetR();

	SetCon(2 * size);

	master = target;
	SetAction("Hover", master);
	SetObjectLayer(this);
}

/**
 Lets the flame fall freely.
 @par size The flame will be this large, in 1/10 of a pixel.
 @par xdir The velocity of the flame, in x direction.
 @par ydir The velocity of the flame, in y direction.
 */
public func SetUpSpark(int size, int xdir, int ydir)
{
	SetCon(2 * size);

	SetXDir(xdir);
	SetYDir(ydir);

	SetAction("Fall");
	SetObjectLayer(this);
}

private func Hit()
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

private func Hover()
{
	if (!master) return RemoveObject();

	var _x,_y,r;

	r = pos_angle + master_angle + master->GetR();

	_x = +Sin(r, pos_distance);
	_y = -Cos(r, pos_distance);

	SetXDir();
	SetYDir();

	SetPosition(master->GetX() + pos_x, master->GetY() + pos_y);

	DoFlameParticle();
	if (GetActTime() > 30) DoCon(-1);
}

private func Fall()
{
	DoFlameParticle();
	if (GetActTime() > 30) DoCon(-2);
}

private func DoFlameParticle()
{
	var size = Max(1, GetCon() * 7 / 100);
	
	var redness = PV_Random(100, 255);

	CreateParticle("Thrust", PV_Random(-size/2, size/2),
						     PV_Random(-size/2, size/2),
						     GetXDir()/2, GetYDir()/2,
						     PV_Random(8, 16),
						     {    Prototype = Particles_Thrust(),
							      R = 255,
							      G = redness,
							      B = redness,
							      Alpha = PV_Linear(160, 0),
							      Size = PV_Linear(PV_Random(2 * size / 3, size), PV_Random(size, 3 * size / 2)),
						     ForceX = PV_KeyFrames(10, 0, PV_Random(-12, 12), 333, PV_Random(-8, 8), 666, PV_Random(-6, 6), 1000, PV_Random(-6, 6)),
						     ForceY = PV_KeyFrames(10, 0, PV_Random(-16, -2), 333, PV_Random(-24, -3), 666, PV_Random(-32, -4), 1000, PV_Random(-40, -5))},
						     RandomX(3, 5));
}

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
	FacetBase = 1,
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
	FacetBase = 1,
	NextAction = "Fall",
	StartCall = "Fall",
},

};
