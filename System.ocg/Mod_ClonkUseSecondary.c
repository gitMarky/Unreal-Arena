/**
 Enables a secondary use for one object.
 @author Marky
 */

#appendto Library_ClonkControl

/** Get the 'i'th item in hands.
    These are the items that will be used with use-commands. (Left mouse click, etc...)
 @note modified, so that it always gets the selected object, for alt-using it
 */
public func GetHandItem(int i)
{
	// i is valid range
	//if (i >= GetLength(use_objects))
	//	return nil;
	if (i < 0) return nil;

	// carrying a carry heavy item always returns said item. (he holds it in both hands, after all!)
	if (IsCarryingHeavy())
		return GetCarryHeavy();
		
	return GetItem(use_objects[0]); // changed from 'i' to '0'
}

/*
// Control use redirected to script
private func ControlUse2Script(int ctrl, int x, int y, int strength, bool repeat, bool release, object obj)
{
	// click on secondary cancels primary and the other way round
	if (using)
	{
		if (ctrl == CON_Use && alt || ctrl == CON_UseAlt && !alt
		||  ctrl == CON_UseDelayed && alt || ctrl == CON_UseAltDelayed && !alt)
		{
			CancelUseControl(x, y);
			return true;
		}
	}
	
	// standard use
	if (ctrl == CON_Use || ctrl == CON_UseAlt)
	{
		if (!release && !repeat)
		{
			return StartUseControl(ctrl,x, y, obj);
		}
		else if (release && obj == using)
		{
			return StopUseControl(x, y, obj);
		}
	}
	// gamepad use
	else if (ctrl == CON_UseDelayed || ctrl == CON_UseAltDelayed)
	{
		if (!release && !repeat)
		{
			return StartUseDelayedControl(ctrl, obj);
		}
		else if (release && obj == using)
		{
			return StopUseDelayedControl(obj);
		}
	}
	
	// more use (holding)
	if (ctrl == CON_Use || ctrl == CON_UseAlt || ctrl == CON_UseDelayed || ctrl == CON_UseAltDelayed)
	{
		if (release)
		{
			// leftover use release
			CancelUse();
			return true;
		}
		else if (repeat && !noholdingcallbacks)
		{
			return HoldingUseControl(ctrl, x, y, obj);
		}
	}
		
	return false;
}
*/


private func GetUseCallString(string action)
{
	// Control... or Contained...
	var control = "Control";
	if (using_type == C4D_Structure) control = "Contained";
	// ..Use.. or ..UseAlt...
	var estr = "";
	if (alt) estr = "Alt";
	// Action
	if (!action) action = "";
	
	return Format("~%sUse%s%s",control,estr,action);
}

/*
// no need to modify this
private func StartUseControl(int ctrl, int x, int y, object obj)
{
	using = obj;
	obj->SetController(GetController());
	using_type = DetermineUsageType(obj);
	// alt = ctrl != CON_Use;
	alt = ctrl != CON_Use;
	
	var start = "Start";
	
	if (alt) start = "AltStart";
	
	Log("Starting use: %d, use = %d, alt = %d, alt value = %v", ctrl, CON_Use, CON_UseAlt, alt);
	
	var hold_enabled = obj->Call("~HoldingEnabled");
	
	if (hold_enabled)
		SetPlayerControlEnabled(GetOwner(), CON_Aim, true);

	// first call UseStart. If unhandled, call Use (mousecontrol)
	var cll = GetUseCallString(start);
	var handled = obj->Call(cll,this,x,y); //*
	if (!handled)
	{
		Log("Did not handle %s", cll);
		handled = obj->Call(GetUseCallString(),this,x,y);
		noholdingcallbacks = handled;
	}
	else
	{
		Log("Handled %s", cll);
	}
	if (!handled)
	{
		using = nil;
		using_type = nil;
		if (hold_enabled)
			SetPlayerControlEnabled(GetOwner(), CON_Aim, false);
		return false;
	}
		
	return handled;
}
*/