
public func Launch(int lifetime)
{
	local timer, end, change;

	timer = lifetime;
	end = timer / 2;

	change = BoundBy(timer / 18, 1, timer);

	local rgb1, rgb2;

	rgb1 = 255 - Random(30);
	rgb2 = 100 - Random(30);

	SetAction("Travel");
}

private func Travel()
{
	timer = BoundBy(timer - change, end, timer);
	
	SmokeX(0, 0, timer, 2, RandomX(25, 40), RGBa(rgb1, rgb1, rgb1, 190), RGBa(rgb2, rgb2, rgb2, 0), 1);
	
	if (timer <= end) RemoveObject();
}

private func ContactBottom()
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
Travel = {
	Prototype = Action,
	Name = "Travel",
	Procedure = DFA_NONE,
	Directions = 1,
	Length = 1,
	Delay = 1,
	FacetBase = 1,
	NextAction = "Travel",
	StartCall = "Travel",
},

};
