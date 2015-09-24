/**
 Dead Clonks disintegrate!
 @version 0.1
 */

#appendto Clonk

protected func Death(int killed_by)
{
	var self = this;

	// this must be done first, before any goals do funny stuff with the clonk
	_inherited(killed_by,...);

	// prevent doing stuff with non-existing objects
	if (!self) return;
	
	Schedule(this, "Disintegrate(20, 50, -10)", 100);	
}


