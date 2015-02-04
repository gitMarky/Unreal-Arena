/**
 Overloads/implements callbacks from the game configuration object.
 @author Marky
 @version 0.1
 */
 
 #appendto Environment_ConfigurationUA

protected func GetDefaultRules()
{
	return [];
}

protected func GetAvailableGoals()
{
	return [Goal_CaptureTheFlagEx, Goal_DeathMatch];
}

protected func GetDefaultItemConfigurations()
{
	return [
	{ name = "UA", icon = Environment_ConfigurationUA, spawnpoints = [], equipment = []},
	{ name = "UA2", icon = Environment_ConfigurationUA, spawnpoints = [], equipment = []},
	{ name = "UA3", icon = Environment_Configuration, spawnpoints = [], equipment = []}
	];
}