/**
 Enables a secondary use for one object.
 @author Marky
 */

#appendto Library_ClonkControl


private func GetUseCallString(string action)
{
	// Control... or Contained...
	var control = "Control";
	if (this.control.using_type == C4D_Structure) control = "Contained";
	// ..Use.. or ..UseAlt...
	var estr = "";
	if (this.control.alt) estr = "Alt";
	// Action
	if (!action) action = "";
	
	return Format("~%sUse%s%s", control, estr, action);
}

// override control throw
public func ObjectControl(int plr, int ctrl, int x, int y, int strength, bool repeat, bool release)
{
	if (ctrl == CON_Throw)
	{
		return inherited(plr, CON_UseAlt, x, y, strength, repeat, release);
	}
	else
	{
		return inherited(plr, ctrl, x, y, strength, repeat, release);
	}
}
