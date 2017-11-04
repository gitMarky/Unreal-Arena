/**
	Bot strategy
	
	Bots may call this in order to "communicate" with each other:
	The stragety possesses knowledge about the bots and assigns roles to them.
	This defines, what kind of tasks the bots will get.
*/

static strategy;

static strategy_levels = 3;


/**
 Select strategies for the teams.
 */
func SelectFactionStrategy()
{
	AssertDefinitionContext();
	
	if (GetType(strategy) != C4V_Array)
		strategy = [];
	
	var goal = GetGoal();
	if (!goal)
	{
		FatalError("Cannot select strategy, because there is no goal");
	}
	
	var goaldiff = [];
	var teamdiff = [];
	
	var max = 0;
	
	var win_score = goal->GetWinScore();
	
	// Determine the individual scores
	for (var faction = 0; faction < goal->GetFactionCount(); ++faction)
	{
		// Choose a random strategy
		strategy[faction] = Random(strategy_levels);
		
		// Determine the difference to the win score, in percent
		var score = goal->GetScore(faction);
		
		goaldiff[faction] = 100 * (win_score - score) / win_score;
		
		// Determine the highest current score
		if (max < score)
			max = score;
	}
	
	// Determine the strategy, based on the other teams' progress
	for (var faction = 0; faction < goal->GetFactionCount(); ++faction)
	{
		var score = goal->GetScore(faction);
		teamdiff[faction] = 100 * (max - score) / max;
		
		// The team becomes more defensive the closer it is to reaching the goal
		for (var other = 0; other < GetTeamCount(); ++other)
			if (other != faction && goaldiff[other] < 20)
			{
				strategy[faction] = 0;
			}
		
		// Become aggressive if we seriously lack behind the leading team
		if (teamdiff[faction] >= 50)
		{
			strategy[faction] = Max(0, strategy_levels - 1);
		}
	}
}


func SelectBotStrategy()
{
}





func GetGoal()
{
	for (var goal in FindObjects(Find_Category(C4D_Goal))) 
	if (goal->~GetWinScore())
	{
		return goal;
	}
	return nil;
}