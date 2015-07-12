#appendto Library_ClonkControl

public func ControlJump()
{
	//Normal jump
	if (GetAction() == "Jump"
	 && Inside(GetActTime(), 12, 24)
	 && !GetEffect("ExtraJump", this))
	{
		SetAction("Jump");
		SetYDir(-this.JumpSpeed * GetCon(), 100 * 100);
		AddEffect("ExtraJump", this, 1, 25, this);
		return true;
	}

	return _inherited();
}

// Called when CON_Left/Right/Up/Down controls are issued/released
// Return whether handled
public func ObjectControlMovement(int plr, int ctrl, int strength, bool release, bool repeat)
{
	if (!this) return false;
	
	// movement is only possible when not contained
	if (Contained()) return false;

	// this is for controlling movement with Analogpad
	if(!release)
		if(strength != nil && strength < CON_Gamepad_Deadzone)
			return true;
	
	// Some specific movement controls
	if (!release && ctrl == CON_Jump)
	{
		if(GetAction() == "Jump")
		{
			this->ObjectCommand("Jump");
			return true;
		}
	}
	return _inherited(plr, ctrl, strength, release, repeat, ...);
}
