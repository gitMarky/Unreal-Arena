#include Library_ModularAppearance
#include Clonk_Animations

func Initialize()
{
	_inherited(...);
	this.ActMap = new Clonk.ActMap{};
	
	// override animation start/end calls
	for (var property in GetProperties(this.ActMap))
	{
		this.ActMap[property].StartCall = nil;
		this.ActMap[property].EndCall = nil;
	}
}

func StartSplatter()
{
	var side = "L"; if (Random(2)) side = "R";
	PlayAnimation(Format("JumpUp.%s", side), CLONK_ANIM_SLOT_Death, Anim_Linear(0, 0, GetAnimationLength("Dead"), 20, ANIM_Hold), Anim_Linear(0, 0, 1000, 5, ANIM_Remove));
	// Update carried items
	UpdateAttach();
	// Set proper turn type
	SetTurnType(1);
}

func VertexSetupLegs()
{
	var permanent = 2;
	SetVertex(1, VTX_Y, 2, permanent);
	SetVertex(3, VTX_Y, 2, permanent);
	SetVertex(4, VTX_Y, 2, permanent);
	SetVertex(5, VTX_X,-2, permanent);
	SetVertex(6, VTX_X, 2, permanent);
	SetVertex(5, VTX_Y, 4, permanent);
	SetVertex(6, VTX_Y, 4, permanent);
}

func VertexSetupBody()
{
	var permanent = 2;
	SetVertex(1, VTX_Y, -4, permanent);
	SetVertex(2, VTX_Y, 2, permanent);
	SetVertex(5, VTX_X, 0, permanent);
	SetVertex(6, VTX_X, 0, permanent);
	SetVertex(7, VTX_Y, 0, permanent);
	SetVertex(8, VTX_Y, 0, permanent);
}

func VertexSetupHead()
{
	var permanent = 2;
	SetVertex(1, VTX_Y, -9, permanent);
	SetVertex(0, VTX_Y, -8, permanent);
	SetVertex(2, VTX_Y, -7, permanent);

	SetVertex(3, VTX_Y, -8, permanent);
	SetVertex(4, VTX_Y, -8, permanent);
	SetVertex(5, VTX_Y, -8, permanent);
	SetVertex(6, VTX_Y, -8, permanent);
	SetVertex(7, VTX_Y, -8, permanent);
	SetVertex(8, VTX_Y, -8, permanent);

	SetVertex(5, VTX_X, -2, permanent);
	SetVertex(6, VTX_X, +2, permanent);
}

func VertexSetupArmL()
{
	var permanent = 2;
	SetVertex(2, VTX_Y, 2, permanent);
	SetVertex(7, VTX_Y, 0, permanent);
	SetVertex(8, VTX_Y, 0, permanent);
}

func VertexSetupArmR()
{
	var permanent = 2;
	SetVertex(2, VTX_Y, 2, permanent);
	SetVertex(7, VTX_Y, 0, permanent);
	SetVertex(8, VTX_Y, 0, permanent);
}



// override functions that are expected by clonk animations

func IsWalking(){ return false;}
func StartSearchLadder(){}
func PlaySoundIdle(){}