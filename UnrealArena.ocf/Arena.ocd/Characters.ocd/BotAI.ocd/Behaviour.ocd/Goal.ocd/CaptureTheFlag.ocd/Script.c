#include Task_Template


static const ai_max_flag_base_distance = 250;

public func Execute(proplist controller, object bot)
{
	var logic = controller->GetAgent();
	var owner = bot->GetOwner();
	var role = Bot_Strategy->GetPlayerRole(owner);

	// Initialize some variables
	var enemy_carrier = GetEnemyFlagCarrier(owner);
	var flag = GetFriendlyFlag(owner);
	if (!enemy_carrier && !flag->IsAtBase())
	{
		enemy_carrier = flag;
	}
	var friendly_carrier = GetFriendlyFlagCarrier(owner);
	var flagbase = GetOwnFlagBase(owner);
	var enemy_flag = GetEnemyFlag(owner);
	
	var team_has_players = GetTeamPlayerCount(GetPlayerTeam(owner)) > 1;
	
	// Prevent tasks while in a spawn point
	if (bot->GetProcedure() != DFA_WALK)
	{
		return TASK_EXECUTION_IN_PROGRESS;
	}

	
	// Flag carrier returns home
	if (bot == friendly_carrier)
	{
		// Return to own flag if own flag is not stolen or he is too far from the flag base
		if (!enemy_carrier || ObjectDistance(bot, flagbase) > ai_max_flag_base_distance)
		{
			// If there are more than one players per team
			if (team_has_players || !enemy_carrier)
			{
				Task_MoveAlongPath->AddTo(bot, TASK_PRIORITY_URGENT)->SetStart(bot)->SetDestination(flagbase)->SetDescription("Returning home!");
				return TASK_EXECUTION_IN_PROGRESS;
			}
			// You are the last guy in your team, so hunt the enemy carrier
			else if (enemy_carrier)
			{
				Task_FollowAlongPath->AddTo(bot, TASK_PRIORITY_IMMEDIATE)->SetTarget(enemy_carrier)->SetDescription("Attacking enemy flag carrier");
				return TASK_EXECUTION_IN_PROGRESS;
			}
		}
		else
		{
			// I'm not the only one
			if (team_has_players)
			{
				Guard(controller, bot);
				return TASK_EXECUTION_IN_PROGRESS;
			}
			else if (enemy_carrier)
			{
				Task_FollowAlongPath->AddTo(bot, TASK_PRIORITY_IMMEDIATE)->SetTarget(enemy_carrier)->SetDescription("Attacking enemy flag carrier");
				return TASK_EXECUTION_IN_PROGRESS;
			}
		}
	}
	else // The bot is not carrying the flag
	{
		// Flag was stolen. Everyone who can intercept, does that
		var intercept = false;
		if (enemy_carrier)
		{
			intercept = logic->Agent_IsInterceptor(bot, enemy_flag, enemy_carrier);
			if (intercept)
			{
				bot->Message("Intercept");
				Task_FollowAlongPath->AddTo(bot, TASK_PRIORITY_URGENT)->SetTarget(enemy_carrier)->SetDescription("Intercepting enemy flag carrier!");
				return TASK_EXECUTION_IN_PROGRESS;
			}
		}
		
		// Otherwise we do the normal routine
		if (!intercept)
		{
			// Friendly flag carrier around?
			if (friendly_carrier && !enemy_carrier)
			{
				bot->Message("Flag carrier");
				var follower;
				// I am a sniper, so I don't leave my spot!!
				if (role = ROLE_Sniper)
				{
					follower = false;
				}
				else
				{
					follower = logic->Agent_IsInterceptor(bot, flagbase, friendly_carrier);
				}
				// between flagger and own flag: follow flagger
				if (follower)
				{
					Task_FollowAlongPath->AddTo(bot, TASK_PRIORITY_HIGH)->SetTarget(enemy_carrier)->SetDescription("Escorting friendly flag carrier!");
					return TASK_EXECUTION_IN_PROGRESS;
				}
			}
			
			if (role == ROLE_Assault || role == ROLE_Support)
			{
				bot->Message("Assaulting");
				// Just go to enemy flag/enemy flag carrier
				if (!enemy_carrier)
				{
					// TODO: Move tasks need to override the one with the least priority
					Task_MoveAlongPath->AddTo(bot, TASK_PRIORITY_HIGH)->SetStart(bot)->SetDestination(enemy_flag)->SetDescription("Get enemy flag");
				}
				else
				{
					Task_FollowAlongPath->AddTo(bot, TASK_PRIORITY_URGENT)->SetTarget(enemy_carrier)->SetDescription("Hunting down enemy flag carrier!");
				}
			}
			else if (role == ROLE_Defend)
			{
				Guard(controller, bot);
			}
			else if (role == ROLE_Sniper)
			{
				if (!controller->HasPriorityTask(Task_Sniper))
				{
					Task_Sniper->AddTo(bot, TASK_PRIORITY_HIGH);
				}
			}
		}
	}

	// Nothing to do.
	return TASK_EXECUTION_IN_PROGRESS;
}


private func Guard(proplist controller, object bot)
{
	bot->Message("Defending");
	if (!controller->HasPriorityTask(Task_MoveAlongPath))
	{
		var defense_points = FindObjects(Find_Func("IsWaypoint"), Find_Func("IsDefensePoint", GetPlayerTeam(bot->GetOwner())));
		var destination = defense_points[Random(GetLength(defense_points))];
		
		if (destination)
		{
			Task_MoveAlongPath->AddTo(bot, TASK_PRIORITY_NORMAL)->SetStart(bot)->SetDestination(destination)->SetDescription("Move to defense point");
		}
	}
}


// Returns the enemy flag carrier if any
private func GetEnemyFlagCarrier(int player)
{
	var flag = GetFriendlyFlag(player);
	return flag->GetFlagCarrier();
}


// Returns the friendly flag carrier if any. 
private func GetFriendlyFlagCarrier(int player)
{
	// The flag...
	for (var flag in GetFlags()) 
	{
		// ... that is attached to a clonk...
		if (WildcardMatch(flag->GetAction(), "*Attach*"))
		{
			var flagger = flag->GetActionTarget();
			var flagger_team = GetPlayerTeam(flagger->GetOwner());
			// ... which is in our team ...
			if (flagger_team == GetPlayerTeam(player))
			{
				// ...his flag
				return flagger;
			}
		}
	}
	return nil;
}


private func GetFriendlyFlag(int player)
{
	// The flag...
	for (var flag in GetFlags()) 
	{
		// that is not ours
		if (flag->GetTeam() == GetPlayerTeam(player))
		{
			return flag;
		}
	}
	return nil;
}


// Returns the flag of a random enemy
private func GetEnemyFlag(int player)
{
	var flags = CreateArray();
	// A flag...
	for (var flag in GetFlags()) 
	{
		// that is not ours
		if (flag->GetTeam() != GetPlayerTeam(player))
		{
			flags[GetLength(flags)] = flag;
		}
	}
	// Choose a flag randomly
	return flags[Random(GetLength(flags))];
}


// Returns the own flag base
private func GetOwnFlagBase(int player)
{
	// The flag...
	for (var flag in GetFlags()) 
	{
		// ... of our team ...
		if (flag->GetTeam() == GetPlayerTeam(player))
		{
			// ...its base
			return flag->GetFlagBase();
		}
	}
	return nil;
}


private func GetFlags()
{
	return FindObjects(Find_ID(Goal_CaptureTheFlagEx->GetFlagID()));
}
