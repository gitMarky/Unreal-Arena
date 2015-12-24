/**
 * Shooter styled inventory:@br
 * 10 items max., 1 with InstaGib
 * 
 * @author Marky
 */

#appendto Clonk


public func MaxContentsCount()
{
	if (IsInstaGibConfigured())
	{
		return 1;
	}
	else
	{
		return 10;
	}
}