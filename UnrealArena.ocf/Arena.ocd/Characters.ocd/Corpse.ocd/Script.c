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
	//SetShape(-4, 0, 8, 10);
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
	//SetShape(-4, -5, 8, 10);
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
	var position = +4 * GetXDirection();
	VertexSetupArm(position);
}

func VertexSetupArmR()
{
	var position = -4 * GetXDirection();
	VertexSetupArm(position);
}

func VertexSetupArm(int position)
{
	var permanent = 2;

	for (var i = 0; i < 9; ++i) SetVertex(i, VTX_X, position, permanent);

	SetVertex(0, VTX_Y, -3, permanent);
	SetVertex(2, VTX_Y, 0, permanent);
	SetVertex(5, VTX_Y, -4, permanent);
	SetVertex(6, VTX_Y, -1, permanent);
	SetVertex(7, VTX_Y, -5, permanent);
	SetVertex(8, VTX_Y, -6, permanent);
}



// override functions that are expected by clonk animations

func IsWalking(){ return false;}
func StartSearchLadder(){}
func PlaySoundIdle(){}

func GetXDirection()
{
	return -1 + 2 * GetDir();
}

// contact and physics

local ContactCalls = true;

func ContactBottom()
{
	return BouncePhysics();
}

func ContactTop()
{
	return BouncePhysics();
}

func ContactLeft()
{
	return BouncePhysics();
}

func ContactRight()
{
	return BouncePhysics();
}

func BouncePhysics()
{
	//if(!dismembered) return(0);
	if (!GetXDir() && !GetYDir()) return;

	SetRDir(-2 * GetRDir());

//	if(dismembered<90) CastParticles("Blood",12,30,0,0,10,40,BloodFXColor(type)[0],BloodFXColor(type)[1] );

	//if (GBackSolid(5,0) || GBackSolid(-5,0)) SetXDir(GetXDir()/-2);
	//if (GBackSolid(0,5) || GBackSolid(0,-5)) SetYDir(GetYDir()/-2);
	//if (GBackSolid(10,0) || GBackSolid(-10,0)) SetXDir(GetXDir()/-2);
	//if (GBackSolid(0,10) || GBackSolid(0,-10)) SetYDir(GetYDir()/-2);

	return true;
}

protected func Hit(int dx, int dy)
{
	if (dy > 1)
	{
		SetYDir(dy * 3 / -4, 100);
	}
}


