/**
 Lets weapons disintegrate after they leave the inventory
 @version 0.1
 */

#appendto Library_Weapon

public func Departure(object container)
{
	if (!GetEffect("IntDisintegration", this))
	{
		AddEffect("IntDisintegration", this, 1, 1, this);
	}

	_inherited(container);
}


private func FxIntDisintegrationTimer(object target, proplist effect, int timer)
{
	if (target->Contained()) return FX_Execute_Kill;
	
	if (timer > 100)
	{
		target->Disintegrate(20, 50, -5, true);
	}
}