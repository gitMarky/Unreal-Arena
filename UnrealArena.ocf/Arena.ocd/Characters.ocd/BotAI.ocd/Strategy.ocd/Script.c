/**
	Bot strategy

	Bots may call this in order to "communicate" with each other:
	The stragety possesses knowledge about the bots and assigns roles to them.
	This defines, what kind of tasks the bots will get.
*/


// Roles are strict
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
func SelectFactionStrategy(object goal)
{
	AssertDefinitionContext();
	if (!goal)
	{
		FatalError("Cannot select strategy, because there is no goal");
	}

	if (GetType(strategy) != C4V_Array)
	{
		strategy = [];
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
		teamdiff[faction] = 100 * (max - score) / Max(max, 1);

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

		Log("* final strategy for faction %d: %d", faction, strategy[faction]);
	}

	// Update each player's role
	for (var i = 0; i < GetPlayerCount(); ++i)
	{
		SelectPlayerRole(goal, GetPlayerByIndex(i));
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


public func SelectPlayerRole(object goal, int player)
{
	var faction = goal->GetFactionByPlayer(player);
	var size = goal->GetFactionSize(faction);

	var total =   TeamQuota(faction, ROLE_Player);
	var quota_assault = TeamQuota(faction, ROLE_Assault);
	// It's the rest anyway - var support = TeamQuota(faction, ROLE_Support);
	var quota_defend =  TeamQuota(faction, ROLE_Defend);
	var quota_sniper =  TeamQuota(faction, ROLE_Sniper);

	var role = nil;
	if (GetPlayerType(player) == C4PT_User)
	{
		role = ROLE_Player;
	}
	else
	{
		var count_sniper = GetRoleCount(goal, faction, ROLE_Sniper);
		var count_defender = GetRoleCount(goal, faction, ROLE_Defend);
		var count_assault = GetRoleCount(goal, faction, ROLE_Assault);

		var demand_sniper = Max(1, quota_sniper * size / total);
		var demand_defender = Max(1, quota_defend * size / total);
		var demand_assault = Max(1, quota_assault * size / total);

		if (count_assault < demand_assault && !role) role = ROLE_Assault; // was last priority
		if (count_sniper < demand_sniper && !role) role = ROLE_Sniper;
		if (count_defender < demand_defender && !role) role = ROLE_Defend;
		if (!role) role = ROLE_Support;

		Log("Choosing role for player %d (team %d): choice = %d; sniper = %d/%d, defender = %d/%d, assault = %d/%d", player, faction, role, count_sniper, demand_sniper, count_defender, demand_defender, count_assault, demand_assault);
	}
	SetPlayerRole(player, role);
}


public func GetRoleCount(object goal, int faction, int role)
{
	var count = 0;
	for (var i = 0; i < GetPlayerCount(); i++)
	{
		var player = GetPlayerByIndex(i);
		if (GetPlayerRole(player) == role && goal->GetFactionByPlayer(player) == faction) 
		{
			++count;
		}
	}

	return count;
}


private func InitPlayerRoles()
{
	if (!roles) roles = [];
}

/*
private func GetGoal()
{
	for (var goal in FindObjects(Find_Category(C4D_Goal))) 
	if (goal->~GetWinScore())
	{
		return goal;
	}
	return nil;
}*/


// Tells how many bots should take a certain role, in percent
private func TeamQuota(int faction, int role)
{
	var team_strategy = strategy[faction];

	if(team_strategy == 2) // Agressive: 80% attack, 20% defend
	{
		if (role == ROLE_Assault) return 30;
		if (role == ROLE_Support) return 50;
		if (role == ROLE_Sniper) return 10;
		if (role == ROLE_Defend) return 10;
	}
	else if(team_strategy == 1) // Balanced: 60% attack, 40% defend
	{
		if (role == ROLE_Assault) return 20;
		if (role == ROLE_Support) return 40;
		if (role == ROLE_Sniper) return 20;
		if (role == ROLE_Defend) return 20;
	}
	else if (team_strategy == 0) // Defensive: 40% attack, 60% defend
	{
		if (role == ROLE_Assault) return 20;
		if (role == ROLE_Support) return 20;
		if (role == ROLE_Sniper) return 30;
		if (role == ROLE_Defend) return 30;
	}

	return 100; // Fallback for unhandled cases
}


/**
 This function can be added to waypoints.
 */
public func IsDefensePoint(int team)
{
	return this.team == team;
}

/**
 This function can be added to waypoints.
 */
public func IsSniperPoint(int team)
{
	return this.team == team;
}
