/**
 * Shooter styled inventory:@br
 * 10 items max., 1 with InstaGib
 * 
 * @author Marky
 */

#appendto Clonk

// callback from the extended relaunch container
public func OnStartRelaunch()
{
	SetInventorySize();
}

// callback from the extended relaunch container
public func OnFinishRelaunch()
{
	SetInventorySize();
}


public func SetInventorySize()
{
	if (IsInstaGibConfigured())
	{
		DebugLog("Set inventory size to 1");
		SetMaxContentsCount(1);
	}
	else
	{
		DebugLog("Set inventory size to 10");
		SetMaxContentsCount(10);
	}
}
