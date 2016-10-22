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

// override functions that are expected by clonk animations

func IsWalking(){ return false;}
func StartSearchLadder(){}
func PlaySoundIdle(){}