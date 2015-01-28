/**
 Overloads/implements callbacks from the game configuration object.
 @author Marky
 @version 0.1
 */

#include Environment_Configuration

/**
 Nice sound effect when closing the menu.
 */
protected func OnCloseMainMenu()
{
	SelectionMajor();
}

protected func SelectionMajor()
{
	Sound("SelectMajor");
}

protected func SelectionMinor()
{
	Sound("SelectMinor");
}