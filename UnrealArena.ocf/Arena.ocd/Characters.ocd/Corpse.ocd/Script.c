#include Library_ModularAppearance
#include Clonk_Animations

func Initialize()
{
	_inherited(...);
	SetAction("Be");
}

local animation_main;
local animation_side;


/**
 Starts the corpse effects.
 
 @par animation_speed Linear value, where 1000 is the normal animation speed.
                      The speed is scaled by the gravity value additionally,
 @par on_ground If true, the corpse will play the normal death animation.
 */
func StartSplatter(int animation_speed, bool on_ground)
{
	var animation_length;

	if (on_ground)
	{
		animation_main = "Dead";
		animation_side = nil;
		animation_length = 20;
	}
	else
	{
		var side = "L"; if (Random(2)) side = "R";
		animation_main = Format("JumpUp.%s", side);
		animation_side = "Stand";
		animation_length = 40;
	}
	
	var death = false;
	
	//var animation_duration = animation_length * Sqrt(100 / Max(1, GetGravity()));
	var animation_duration = animation_length * Sqrt(10000 / Max(1, GetGravity())) / 10;
	animation_duration = animation_speed * animation_duration / 1000;
	
	// Bsp: 20
	// Gravity 100
	// duration = 20 * sqrt(1); ...
	// Gravity 50 => 20 * Sqrt (2) => 34
	// Gravity 200 => 20 * Sqrt (100/200) = ...
	// Gravity 200 => 20 * Sqrt(10000 / 200) = 20 * Sqrt (50) / 10

	// Overlay death animations except for the death slot
	for (var slot = 0; slot < CLONK_ANIM_SLOT_Death; ++slot)
	{
		var number = GetRootAnimation(slot); 
		if (number == nil) continue;

		death = true;
		OverlayDeathAnimation(slot, animation_duration, animation_main, animation_side);
	}
	
	if (!death) // add the animation on the lowest slot, so that blending is not disturbed
	{
		OverlayDeathAnimation(CLONK_ANIM_SLOT_Movement, animation_duration, animation_main, animation_side);
	}
	
	// Update carried items
	UpdateAttach();
	// Set proper turn type
	SetTurnType(1);
}

// animation stuff

func OverlayDeathAnimation(int slot, int animation_duration, string animation1, string animation2)
{
	if (animation1 == nil)
	{
		FatalError("You have to pass at least one animation");
	}
	
	var weight = 500;
	if (animation2) weight = 300;

	// Play the animations

	PlayAnimation(animation1, slot, Anim_Linear(0, 0, GetAnimationLength(animation1), animation_duration, ANIM_Hold), Anim_Const(weight));

	if (animation2)
	{
		PlayAnimation(animation2, slot, Anim_Linear(0, 0, GetAnimationLength(animation2), animation_duration * 2, ANIM_Hold), Anim_Const(weight));
	}
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
		
		var range = 30;
		PlayAnimation(name, slot, Anim_Linear(position, 0, GetAnimationLength(name), range, ANIM_Hold), Anim_Linear(0, 1000, range, ANIM_Remove));
	}
}

func Flinch(int duration)
{
	var stand = animation_side ?? animation_main ?? "Stand";
	var duration = 20;
	PlayAnimation(stand, CLONK_ANIM_SLOT_Movement, Anim_Linear(0, 0, GetAnimationLength(stand), duration ?? 2, ANIM_Remove));
}

local ActMap = {
Be = {
	Prototype = Action,
	Name = "Be",
	Procedure = DFA_NONE,
	Directions = 2,
	FlipDir = 0,
	Length = 1,
	Delay = 0,
	X = 0,
	Y = 0,
	Wdt = 8,
	Hgt = 20,
},
};

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
	
	ApplyOffset(0, -8000);
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
	
	ApplyOffset(0, -3000);
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
	
	ApplyOffset(0, 3000);
}

func VertexSetupArmL()
{
	var position = +1 * GetXDirection();
	VertexSetupArm(position);
}

func VertexSetupArmR()
{
	var position = -1 * GetXDirection();
	VertexSetupArm(position);
}

func VertexSetupArm(int position)
{
	var permanent = 2;

	for (var i = 0; i < 9; ++i) SetVertex(i, VTX_X, position, permanent);

	SetVertex(0, VTX_Y,  0, permanent);
	SetVertex(1, VTX_Y, -3, permanent);
	SetVertex(2, VTX_Y, +3, permanent);
	SetVertex(3, VTX_Y, -2, permanent);
	SetVertex(4, VTX_Y, +2, permanent);
	SetVertex(5, VTX_Y, -1, permanent);
	SetVertex(6, VTX_Y, +1, permanent);
	SetVertex(7, VTX_Y, -1, permanent);
	SetVertex(8, VTX_Y, +1, permanent);
	
	ApplyOffset(0, -5000);
}

func ApplyOffset(int x, int y)
{
}

func GetXDirection()
{
	return -1 + 2 * GetDir();
}

// override functions that are expected by clonk animations

func IsWalking(){ return false;}
func StartSearchLadder(){}
func PlaySoundIdle(){}


// contact and physics
//
// TODO: The looks could be improved greatly if the CNAT are set correctly and
// - the RDir is updated if you contact bottom and have xdir; maybe calculate a vector and make it into angular speed?

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

	SetRDir(-(3 * GetRDir()) / 2);

//	if(dismembered<90) CastParticles("Blood",12,30,0,0,10,40,BloodFXColor(type)[0],BloodFXColor(type)[1] );

	return true;
}

protected func Hit(int dx, int dy)
{
	if (dy > 1)
	{
		SetYDir(dy / -4, 100);
	}
	
	Flinch();
	BouncePhysics();
}
