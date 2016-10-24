/**
 Several small modifications to the clonk can be found here.
 @version 0.1.0
 */

#appendto Clonk

func Recruitment()
{
	// remove actions that are not intended
	if(this.ActMap == this.Prototype.ActMap)
		this.ActMap = {Prototype = this.Prototype.ActMap};
	this.ActMap["Hangle"] = nil;
	this.ActMap["Scale"] = nil;
	
	if (GetPlayerType(GetOwner()) == C4PT_Script)
	{
		AI->AddAI(this);
	}

	return _inherited(...);
}