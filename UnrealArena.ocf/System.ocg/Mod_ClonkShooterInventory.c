/**
 * Shooter styled inventory:@br
 * 10 items max., 1 with InstaGib
 * 
 * @author Marky
 */

#appendto Clonk


public func OnRelaunched()
{
	if (IsInstaGibConfigured())
	{
		SetMaxContentsCount(1);
	}
	else
	{
		SetMaxContentsCount(10);
	}
}
