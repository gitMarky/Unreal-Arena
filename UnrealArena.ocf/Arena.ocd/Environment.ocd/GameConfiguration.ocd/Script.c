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

protected func OnRoundEnd()
{
	Sound("Goal_WinRound", true);
}

protected func PreconfigureBots()
{
	var missing_players = Max(0, GetDefaultPlayerAmount() - GetPlayerCount());

	if (missing_players > 0) AddBots(missing_players);
}

protected func GetDefaultPlayerAmount()
{
	return 4;
}

func ContainCrew(object crew)
{
	var container = _inherited(crew, ...);

	if (container)
	{
		container->SetPosition(LandscapeWidth() / 2, LandscapeHeight() / 2);
	}
}
