

global func SmokeX(int _x, int _y, int con, int move_style, int time, int dwColor1, int dwColor2,int effect_style)
{
	return CreateObject(Effect_SmokeCustom, 0, 0, NO_OWNER)
	       ->Activate(GetX() + _x, GetY() + _y, con / 2, move_style, time,
	                  dwColor1, dwColor2, effect_style);
}


local movement,effect,max_con,max_time,r1,g1,b1,a1,r2,g2,b2,a2;

/*
movement

0 = keine Bewegung
1 = Standard-Clonk Rauch
2 = Zufallsbewegung auf der Stelle

*/

/*
effect

0 = keine Farbübergänge
1 = Farbübergang von rgba1 nach rgba2
2 = Zufallsfarbe zwischen rgba1 und rgba2
3 = wie 2,nur wechselnd

*/

func Activate(int _x, int _y, int con, int move_style, int time, int dwColor1, int dwColor2, int effect_style)
{
	SetAction("Smoke");
	SetCon(75 * con / 100);
	max_con = con;
	
	max_time = 32;
	if (time)
		max_time = time;
	
	if (max_time < 1)
		max_time = 1;

	movement = move_style;
	effect = effect_style;
	
	SetPosition(_x, _y);
	
	SetDir(Random(4));
	SetPhase(Random(4));
	
	r1 = GetRGBaValue(dwColor1, 1);
	g1 = GetRGBaValue(dwColor1, 2);
	b1 = GetRGBaValue(dwColor1, 3);
	a1 = GetRGBaValue(dwColor1, 0);
	
	r2 = GetRGBaValue(dwColor2, 1);
	g2 = GetRGBaValue(dwColor2, 2);
	b2 = GetRGBaValue(dwColor2, 3);
	a2 = GetRGBaValue(dwColor2, 0);
	
	if (!a1 && !a2)
	{
		a1 = 200;
		a2 = 0;
	}
	
	if (!effect)
		SetClrModulation(RGBa(255, 255, 255, 123));
	
	Move2Move();
	AddEffect("IntMove", this, 1, 1, this);
}

public func FxIntMoveTimer()
{
	Move2Move();
}

func Move2Move()
{
	if (movement == 1)
	{
		SetYDir(-20);
		SetXDir(-10 + Random(21));
	}
	if (movement == 2)
	{
		SetYDir(-10 + Random(21));
		SetXDir(-10 + Random(21));
	}
	
	var _x, _y;
	
	_x = GetX();
	_y = GetY();
	
	SetCon
	(
		75 * max_con / 100 + GetActTime() * 45 * max_con / (100 * max_time)
	);

	SetPosition(_x, _y);

	if (effect == 1)
	{
		SetClrModulation
		(
			RGBa
			(
				r1 + GetActTime() * (r2 - r1) / max_time,
				g1 + GetActTime() * (g2 - g1) / max_time,
				b1 + GetActTime() * (b2 - b1) / max_time,
//				255 - (a1 + GetActTime() * (a2 - a1) / max_time)
				a1 + GetActTime() * (a2 - a1) / max_time
			)
		);
	}
	else if (effect == 3)
	{
		var i = Random(100);
		
		SetClrModulation
		(
			RGBa
			(
					Min(r1, r2) + i * Abs(r2 - r1) / 100,
					Min(g1, g2) + i * Abs(g2 - g1) / 100,
					Min(b1, b2) + i * Abs(b2 - b1) / 100,
//					255 - (Min(a1, a2) + i * Abs(a2 - a1) / 100)
					Min(a1, a2) + i * Abs(a2 - a1) / 100
			)
		);
	}
	
	if (GetActTime() > max_time)
		Remove();
}

func Remove()
{
	return RemoveObject();
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// properties

local Name = "$Name$";
local Description = "$Description$";
local Collectible = 0;

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// actions

local ActMap = {
Smoke = {
	Prototype = Action,
	Name = "Smoke",
	Procedure = DFA_FLOAT,
	Directions = 4,
	Length = 4,
	Delay = 0,
	X = 0,
	Y = 0,
	Wdt = 64,
	Hgt = 64,
	NextAction = "Hold",
},

Smoke2 = {
	Prototype = Action,
	Name = "Smoke2",
	Procedure = DFA_FLOAT,
	Directions = 4,
	Length = 4,
	Delay = 0,
	X = 0,
	Y = 0,
	Wdt = 64,
	Hgt = 64,
	NextAction = "Hold",
},

};
