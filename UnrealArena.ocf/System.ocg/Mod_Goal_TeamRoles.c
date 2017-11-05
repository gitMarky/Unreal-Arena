#appendto Library_ConfigurableGoal


public func DoSetup(int round)
{
	_inherited(round);
	Log("Selecting team strategy after setup");
	Bot_Strategy->SelectFactionStrategy(this);
}


public func DoScore(int faction, int change, bool force_negative)
{
	_inherited(faction, change, force_negative);
	Log("Selecting team strategy after score change");
	Bot_Strategy->SelectFactionStrategy(this);
}
