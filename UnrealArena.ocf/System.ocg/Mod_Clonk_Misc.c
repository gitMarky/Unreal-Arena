/**
 Several small modifications to the clonk can be found here.
 @version 0.1.0
 */

#appendto Clonk

private func Recruitment()
{
	// Remove actions that are not intended
	if(this.ActMap == this.Prototype.ActMap)
		this.ActMap = {Prototype = this.Prototype.ActMap};
	this.ActMap["Hangle"] = nil;
	this.ActMap["Scale"] = nil;
	
	// Add AI
	if (GetPlayerType(GetOwner()) == C4PT_Script)
	{
		BotAI->AddAI(this);
	}

	return _inherited(...);
}
