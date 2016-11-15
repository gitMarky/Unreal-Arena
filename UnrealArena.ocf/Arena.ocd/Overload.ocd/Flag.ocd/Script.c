/**
 Modified flag for CTF.
 
 @author Marky (modified script)
 @credits Maikel
 @version 0.1
--*/

#include Goal_Flag


protected func FxFlagCarriedStart(object target, proplist effect, int temp)
{
	if (temp) return;
	
	effect.x=target->GetX();
	effect.y=target->GetY();
	var trans = Trans_Mul(Trans_Translate(-17000, 0, 0), Trans_Rotate(90, 1, 0, 0));
	effect.mesh_id = target->AttachMesh(this, "pos_back1", "main", trans);
	this.Visibility = VIS_None;	
	return true;
}

// Checks whether the carrier has reached its base.
protected func FxFlagCarriedTimer(object target, proplist effect)
{
	var controller = target->GetController();
	var ctrl_team = GetPlayerTeam(controller);

	// Search for nearby base to drop flag and score a point.
	var base = FindObject(Find_ID(Goal_CaptureTheFlagEx->GetFlagBaseID()), Find_Func("FindTeam", ctrl_team), Find_Distance(20));
	if (base && base->IsBaseWithFlag()) 
	{
		var goal = FindObject(Find_ID(Goal_CaptureTheFlagEx));
		if (goal)
			goal->DoScore(ctrl_team, 1);
		Log("$MsgFlagCaptured$", GetTaggedTeamName(ctrl_team));
		BeamFlag(false);
		return FX_Execute_Kill;
	}
	
	return FX_OK;
}


// Reduces physicals by 80%.
private func ReducePhysicals(object clonk, effect)
{
	return;
}

// Resets physicals.
private func ResetPhysicals(object clonk, effect)
{
	return;
}

// Create a new flag on destruction.
protected func Destruction()
{
	var base = FindObject(Find_ID(Goal_CaptureTheFlagEx->GetFlagBaseID()), Find_Func("FindTeam", GetTeam()));
	if (base)
	{
		var flag = CreateObject(GetID(), 0, 0, GetOwner());
		flag->SetTeam(GetTeam());
		SetAction("AttachBase", base);
		Log("$MsgFlagRestored$", GetTaggedTeamName(team));
	}
	return;
}

// Returns whether the flag is at its base.
public func IsAtBase()
{
	if (GetAction() == "AttachBase")
		return true;
	return false;
}

private func BeamFlag(bool msg)
{
	if (IsAtBase())
		return;
	if (msg)
		Log("$MsgFlagBeamed$", GetTaggedTeamName(team));
	var base = FindObject(Find_ID(Goal_CaptureTheFlagEx->GetFlagBaseID()), Find_Func("FindTeam", team));
	if (base)
		SetAction("AttachBase", base);
	else 
		RemoveObject();
	return;
}
