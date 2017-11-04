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

	
	// Flag carrier returns home
	if (bot == friendly_carrier)
	{
		// Return to own flag if own flag is not stolen or he is too far from the flag base
		if (!enemy_carrier || ObjectDistance(bot, flagbase) > ai_max_flag_base_distance)
		{
			// If there are more than one players per team
			if (team_has_players || !enemy_carrier)
			{
				Run(bot, flagbase, "Returning home!");
				return;
			}
			// You are the last guy in your team, so hunt the enemy carrier
			else if (enemy_carrier)
			{
				Task_Follow->AddTo(bot, 1)->SetTarget(enemy_carrier)->SetDescription("Attacking enemy flag carrier");
				return;
			}
		}
		else
		{
			// I'm not the only one
			if (team_has_players)
			{
				bot->Guard(bot, "Guarding our flag base!");
				return;
			}
			else if (enemy_carrier)
			{
				Task_Follow->AddTo(bot, 1)->SetTarget(enemy_carrier)->SetDescription("Attacking enemy flag carrier");
				return;
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
				Task_Follow->AddTo(bot, 1)->SetTarget(enemy_carrier)->SetDescription("Intercepting enemy flag carrier!");
				return;
			}
		}
		
		// Otherwise we do the normal routine
		if (!intercept)
		{
			// Friendly flag carrier around?
			if (friendly_carrier && !enemy_carrier)
			{
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
					Task_Follow->AddTo(bot, 1)->SetTarget(enemy_carrier)->SetDescription("Escorting friendly flag carrier!");
					return;
				}
			}
			
			if (role == ROLE_Assault || role == ROLE_Support)
			{
				// Just go to enemy flag/enemy flag carrier
				if (!enemy_carrier)
				{
					Run(bot, enemy_flag, "Get enemy flag");
				}
				else
				{
					Task_Follow->AddTo(bot, 1)->SetTarget(enemy_carrier)->SetDescription("Hunting down enemy flag carrier!");
				}
			}
			else if (role == ROLE_Defend)
			{
			/* TODO
				if (!(bot->GetCommand() || bot->GetMacroCommand()))
				{
					var wps = FindObjects(Find_ID(WAYP), Find_Func("CheckInfo", WPInfo_Defend, GetPlayerTeam(owner)));
					var wp = wps[Random(GetLength(wps))];
					
					if (wp)
					{
						Run(bot, wp, "Move to defense point"); //SetMacroCommand(this, "MoveTo", wp, 0, 0, 0, Aggro_Follow);
					}
				}
			*/
			}
			else if (role == ROLE_Sniper)
			{
			/*
				if (!(bot->GetCommand() || bot->GetMacroCommand()))
				{
					// TODO: make this a task
					Sniper(bot);
				}
			*/
			}
		}
	}

	// Nothing to do.
	return TASK_EXECUTION_IN_PROGRESS;
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

private func Run(object bot, object destination, string description)
{
	bot->GetAI()->GetAgent()->Agent_MoveTo(bot, destination);
}


// Sniper
public func Sniper(object bot)
{
	var owner = bot->GetOwner();

	var wps = FindObjects(Find_Func("IsWaypoint"), Find_Func("CheckInfo")); // TODO , WPInfo_Sniper,GetPlayerTeam(owner)));
	var snipe;

 	DebugLog("Found sniping points %v","tactics",wps);
 	for (var wp in wps)
 	{
		if (ObjectDistance(bot, wp) < 5)
		{
			snipe = true;
			break;
		}
	}
	if (!snipe)
	{
		wp = wps[Random(GetLength(wps))];

		if (wp) Run(bot, wp, "Going to sniper point");
	}
	else
	{
		//TODO bot->SetMacroCommand(0, "Wait",	0,0,0,0,35);

		var target;
		if (!bot->GetAggroTarget()) target = bot->~UTBotAIFindEnemy(1500);
		if (target)
		{
			bot->SetAggroLevel(Aggro_Shoot);
			bot->SetAggroTarget(target);
			Message("@Sniping,...",bot);
			Message("@Sn: %d",target,ObjectNumber(bot));
		}
		else
		{
			bot->SetAggroLevel(Aggro_Nothing);
			Message("@Waiting,...",bot);
		}
	}
}
