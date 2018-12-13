#appendto Library_Firearm

local is_selected = false;

//----------------------------------------------------------------------------------------------------------------
//
// selecting and deselecting the weapon

/**
 Callback from the inventory system: The weapon was selected.
 Has it's own callback {@link Library_Weapon#OnSelection}, so
 that specific behaviour can be overloaded without messing
 with the rest of the functionality.

 @par user The object that is using the weapon.
 @version 0.3.0
 */
public func Selection(object user)
{
	is_selected = true;
	this->~OnSelection(user, GetFiremode());
	return _inherited(user, ...);
}


/**
 Callback from the inventory system: The weapon was deselected.
 Has it's own callback {@link Library_Weapon#OnDeselection}, so
 that specific behaviour can be overloaded without messing
 with the rest of the functionality.

 @par user The object that is using the weapon.
 @version 0.3.0
 */
public func Deselection(object user)
{
	is_selected = false;
	this->~OnDeselection(user, GetFiremode());
	return _inherited(user, ...);
}
