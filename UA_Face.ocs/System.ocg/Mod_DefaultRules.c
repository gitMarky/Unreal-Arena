/**
 Overloads/implements callbacks from the game configuration object.
 @author Marky
 @version 0.1
 */
 
 #appendto Environment_Configuration

protected func GetDefaultRules()
{
	return [];
}

protected func GetAvailableGoals()
{
	return [Goal_CaptureTheFlag, Goal_DeathMatch];
}