/*--
		Flag Base
		Author: Maikel
		
		A base for the CTF goal, flags can be stolen here, and players must bring flags of opposing teams here.
--*/

#include Goal_FlagBase

// Returns whether there is a flag at this base.
public func IsBaseWithFlag()
{
	var flag = FindObject(Find_ID(Goal_CaptureTheFlagEx->GetFlagID()), Find_Func("FindTeam", team));
	if (flag->GetAction() != "AttachBase")
		return false;
	if (flag->GetActionTarget() != this)
		return false;
	return true;
}