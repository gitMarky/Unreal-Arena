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
	return [Goal_DeathMatchEx];
}