/**
 * Pseudo lift plate. Moves up when someone steps on it, moves to collision, waits 2 seconds, moves down again.
 */


private func GetAutoSpeed() { return 80; }

protected func Initialize()
{
	AddEffect("CheckAutoMove", this, 1, 5, this);
	SetAction("Travel");
}

protected func ContactTop()
{
	Halt();
	SetAction("Wait");
	return;
}

protected func ContactBottom()
{
	Halt();
	SetAction("Check");
	return;
}

private func Halt()
{
	// Clear speed.
	SetYDir();
	return;
}

private func MoveDown()
{
	SetMoveDirection(COMD_Down);
}


private func SetMoveDirection(int dir)
{	
	// no change?
	if (dir == COMD_Up && (GetYDir() < 0)) return;
	if (dir == COMD_Down && (GetYDir() > 0)) return;
	
	// already reached top/bottom?
	if (GetContact(-1, CNAT_Bottom) && dir == COMD_Down)
		return;
	if (GetContact(-1, CNAT_Top) && dir == COMD_Up)
		return;
	if (dir == COMD_Stop) 
		return Halt();
	
	var speed = GetAutoSpeed();

	
	if (dir == COMD_Down)
		SetYDir(speed);
	else if (dir == COMD_Up)
		SetYDir(-speed);

	SetAction("Travel");
	SetComDir(COMD_None);
	return;
}


func FxCheckAutoMoveTimer(object target, effect, int time)
{
	if (GetAction() != "Check") return FX_OK;
	
	// look for Clonks at shaft
	var additional = 20;
	var x = -additional;
	var w = 2*additional;
	var y = -29;
	var h = 20;
	var clonk, best;

	for (clonk in FindObjects(Find_InRect(x, y, w, h), Find_OCF(OCF_CrewMember), Find_OCF(OCF_Alive), Find_NoContainer(), Sort_Distance(), Sort_Reverse()))
	{
		var proc = clonk.Action.Procedure;
		if (clonk->GetComDir() != COMD_Stop && !((proc == "SWIM") && Inside(clonk->GetXDir(), -5, 5)))
			continue;
		if (proc != "WALK" && proc != "PUSH" && proc != "SCALE" && proc != "HANGLE" && proc != "SWIM") continue;
		if (clonk->GetY() < GetY() - 20 && !PathFree(GetX(), GetY() - 10, GetX(), clonk->GetY()))
				continue;
		if (clonk->GetY() > GetY() + 7 && !PathFree(GetX(), GetY(), GetX(), clonk->GetY()))
				continue;
		if ((clonk->GetY() > GetY()) && GetContact(-1, CNAT_Bottom)) 
				continue;
		
		
		// Priority rules: Cursor is better than no cursor, nearer is better than farer (Sort_Distance() & Sort_Reverse() do this)
		// So unlike in CR's elevator, no distance check has to be done because later cycles are always nearer clonks
		if (!best) 
			best = clonk;
		else if (GetCursor(clonk->GetController()) == clonk)
			best = clonk;
		else if (GetCursor(best->GetController()) != best)
			best = clonk;
	}
	if (best)
		SetMoveDirection(COMD_Up);
	return FX_OK;
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// properties

local Name = "$Name$";
local Description = "$Description$";
local ContactCalls = true;

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// actions

local ActMap = {
Travel = {
	Prototype = Action,
	Name = "Travel",
	Procedure = DFA_FLOAT,
	Length = 7,
	Delay = 5,
	FacetBase = 1,
	NextAction = "Travel",
},


Check = {
	Prototype = Action,
	Name = "Check",
	Procedure = DFA_FLOAT,
	Length = 1,
	Delay = 100,
	FacetBase = 1,
	NextAction = "Check",
},

Wait = {
	Prototype = Action,
	Name = "Wait",
	Procedure = DFA_FLOAT,
	Length = 1,
	Delay = 70,
	FacetBase = 1,
	NextAction = "Travel",
	EndCall = "MoveDown",
},

};
