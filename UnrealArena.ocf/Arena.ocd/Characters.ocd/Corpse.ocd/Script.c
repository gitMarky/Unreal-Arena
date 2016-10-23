#include Library_ModularAppearance
#include Clonk_Animations

func Initialize()
{
	_inherited(...);
	this.ActMap = new Clonk.ActMap{};
}

func StartSplatter()
{
	var side = "L"; if (Random(2)) side = "R";
	var name = Format("JumpUp.%s", side);
	var stand = "Stand";
	
	var death = false;
	
	// Copy all animations except for the death slot from the target
	for (var slot = 0; slot < CLONK_ANIM_SLOT_Death; ++slot)
	{
		var number = GetRootAnimation(slot); 
		if (number == nil) continue;

		death = true;
		OverlayDeathAnimation(slot, name, stand);
	}
	
	if (!death)
	{
		OverlayDeathAnimation(CLONK_ANIM_SLOT_Death, name, stand);
	}
	
	// Update carried items
	UpdateAttach();
	// Set proper turn type
	SetTurnType(1);
}

// animation stuff

func OverlayDeathAnimation(int slot, string animation1, string animation2)
{
	PlayAnimation(animation1, slot, Anim_Linear(0, 0, GetAnimationLength(animation1), 20, ANIM_Hold), Anim_Linear(0, 0, 1000, 5, ANIM_Remove));
	PlayAnimation(animation2, slot, Anim_Linear(0, 0, GetAnimationLength(animation2), 40, ANIM_Hold), Anim_Linear(0, 0, 1000, 15, ANIM_Remove));
}

func CopyAnimationPositionFrom(object target)
{
	// Copy all animations except for the death slot from the target
	for (var slot = 0; slot < CLONK_ANIM_SLOT_Death; ++slot)
	{
		var number = target->GetRootAnimation(slot); 
		if (number == nil) continue;
		
		var name = target->GetAnimationName(number);
		if (!name) continue; // for simplicity we skip combination nodes
		
		var position = target->GetAnimationPosition(number);
		
		PlayAnimation(name, slot, Anim_Linear(position, 0, GetAnimationLength(name), 30, ANIM_Remove));
	}
}

// vertex configurations

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