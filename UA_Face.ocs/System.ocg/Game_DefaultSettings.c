/**
 Overloads/implements callbacks from the game configuration object.
 @author Marky
 @version 0.1
 */
 
 #appendto player_index

protected func GetDefaultRules()
{
	return [];
}

protected func GetAvailableGoals()
{
	return [Goal_CaptureTheFlagEx, Goal_DeathMatch];
}