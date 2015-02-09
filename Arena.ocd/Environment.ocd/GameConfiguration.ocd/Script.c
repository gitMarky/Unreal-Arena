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

protected func OnOpenMainMenu()
{
	SelectionMinor();
}

protected func OnMenuChooseGoal()
{
	SelectionMinor();
}

protected func OnMenuConfigureBots()
{
	SelectionMinor();
}

protected func OnMenuConfigureGoal()
{
	SelectionMinor();
}

protected func OnMenuConfigureItems()
{
	SelectionMinor();
}

protected func OnMenuConfigureItemsCustom()
{
	SelectionMinor();
}

protected func OnMenuConfigureItemSlot()
{
	SelectionMinor();
}

protected func OnMenuConfigureRules()
{
	SelectionMinor();
}

protected func OnMenuConfigureTeams()
{
	SelectionMinor();
}


protected func SelectionMajor()
{
	Sound("SelectMajor", true);
}

protected func SelectionMinor()
{
	Sound("SelectMinor", true);
}