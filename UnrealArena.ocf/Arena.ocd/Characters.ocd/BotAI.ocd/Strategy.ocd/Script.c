/**
	Bot strategy
	
	Bots may call this in order to "communicate" with each other:
	The stragety possesses knowledge about the bots and assigns roles to them.
	This defines, what kind of tasks the bots will get.
*/


// Roles can be mixed, e.g. in a class system
static const ROLE_Player = 0;
static const ROLE_Assault = 1;
static const ROLE_Support = 2;
static const ROLE_Sniper = 4;
static const ROLE_Defend = 8;

static const strategy_levels = 3;

static strategy;
static roles;



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


public func SetPlayerRole(int player, int role)
{
	AssertDefinitionContext();
	InitPlayerRoles();
	
	roles[player] = role;
}


public func GetPlayerRole(int player)
{
	AssertDefinitionContext();
	InitPlayerRoles();
	
	return roles[player];
}


private func InitPlayerRoles()
{
	if (!roles) roles = [];
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