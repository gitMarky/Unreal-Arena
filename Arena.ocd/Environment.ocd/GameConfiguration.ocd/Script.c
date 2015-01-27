/**
 Overloads/implements callbacks from the game configuration object.
 @author Marky
 @version 0.1
 */

#include Environment_Configuration

protected func CreateMainMenu(object player)
{
	CreateDefaultMenu(player, Icon_Configuration, "$MenuCaption$");

	CreateMainMenuGoal(player, Goal_Random);

	player->AddMenuItem("$Finished$", "ConfigurationFinished", Icon_Ok, 0, 0, "$Finished$");
}

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