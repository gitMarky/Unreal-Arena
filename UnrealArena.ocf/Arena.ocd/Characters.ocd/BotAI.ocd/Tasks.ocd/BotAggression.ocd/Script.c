#include Task_Template


private func CreateTask()
{
	var task = inherited();
	task->AddFunctions([this.SetEnemy, this.GetEnemy]);
	return task;
}


public func Execute(proplist controller, object bot)
{
	if (DelayBotBySkill(7))
		return TASK_EXECUTION_IN_PROGRESS;

	var target = GetEnemy();
	
	// Forget target?
	if (target && !PathFree(bot->GetX(), bot->GetY(), target->GetX(), target->GetY()))
		target = nil;
	
	// We still have a target, shoot at it
	if (target)
	{
		ExecuteAttack(bot, target);

		if (ObjectDistance(bot, target) < 25 && bot->~CheckEnemy(target, bot, false))
		{
			bot->UTBotAIDodgeJump();
		}
		return TASK_EXECUTION_IN_PROGRESS;
	}
	else
	{
		// Tell the weapon to stop shooting
		if (bot->Contents())
			bot->Contents()->~StopAutoFire();
	}
	
	var weaprange = 400;

// TODO
//	if (Contents())
//		weaprange = Contents()->~GetBotData(BOT_Range);
	
	target = bot->UTBotAIFindEnemy(Max(400, weaprange));
	
	if (target)
	{
		SetEnemy(target);	
	}

	return TASK_EXECUTION_IN_PROGRESS;
}


public func SetEnemy(object enemy)
{
	this.TaskEnemy = enemy;
	return this;
}


public func GetEnemy()
{
	return this.TaskEnemy;
}




// Hier wurde nur der Timer geändert
func SetAggroLevel(int level, int distance, int x, int y, string text)
{
	if (level <= 3)
	{
		/* TODO
		var target = GetEnemy();
		if ((GetAggroLevel() >= 2 && level < 2) && target)
			if (GetMacroCommand(1, 1) == target)
			{
				FinishMacroCommand(1, 0, 1);
				FinishMacroCommand(1);
			}
		*/
		this.TaskAggroLevel = level;
		this.TaskAggroDistance = distance ?? 500;
		
		if (level == 3)
		{
			this.TaskAggroX = x ?? GetX();
			this.TaskAggroX = y?? GetY();
		}
		else
		{
			this.TaskAggroX = x ?? GetX();
			this.TaskAggroX = y?? GetY();
		}
	}
	return this;
}


public func GetAggroLevel()
{
	return this.TaskAggroLevel;
}




private func ExecuteAttack(object bot, object target)
{
	var x = this.TaskAggroX;
	var y = this.TaskAggroY;
	var dist = this.TaskAggroDistance;
	var level = this.TaskAggroLevel;
	var enemy = GetEnemy();
	
	// TODO
	var weaprange = 400; // was 0 before
	//if (Contents())
	//	weaprange = Contents()->~GetBotData(BOT_Range);
	

	// TODO: Vehicle control
//	if (Contained())
//	{
//		if (Contained()->~HandleAggro(this, level, target, dist, x, y))
//			return 1;
//		else
//			return AddCommand("Exit", nil, 0, 0, nil, 0, 0, 0, C4CMD_SilentSub);
//	}
	
	// Target vanished?
	/*if (!this->~CheckTarget(target, this, Max(dist, weaprange), 0, 0, true))
	{

		if (Contents())
			Contents()->~StopAutoFire();
		return;
	}*/
	if (!target)
	{
		return;
	}
	
	// No guns?
	if (!bot->Contents())
	{
		return;
	}

	if (!SelectWeapon(bot, target, level, false))
	{
		// On this level we can leave our path. That means, we can also get a weapon or ammo, if we do not have one
		if (GetAggroLevel() == Aggro_Follow)
		{
			// Get weapons?
			if (bot->~CustomContentsCount("IsWeapon") <= 1)
				return SearchWeapon(bot, Aggro_Shoot);
			// Get ammo?
			return SearchAmmo(bot, Aggro_Shoot);
		}
		// Cannot do anything
		return;
	}
	
	
	// Level 1 - Shoot at the target

	var weapon = bot->Contents();
	
	// The bot does not do this every time it could
	var efficiency = weapon->~GetBotData(BOT_Rating);
	if (DelayBotBySkill(efficiency))
		return;
	
	bot->UTBotAIAdjustDir(target);
	
	// Level 2 - Follow him!
	/* TODO
	if (level >= Aggro_Follow)
		if (GetMacroCommand(1) != "Follow" || GetMacroCommand(1, 1) != target)
			if (GetMacroCommand(0) != "Follow" || GetMacroCommand(0, 1) != target)
			{
				DebugLog("FxAggroFire - Adding Follow command", "aggro");
				AddMacroCommand(nil, "MoveTo", nil, GetX(), GetY(), 0, level);
				AddMacroCommand(nil, "Follow", target, 0, 0, 0, level);
			}
	*/

	bot->UTBotAIAimAt(target);

	// use the gun
	/* TODO: the gun tells us how to use it?
	var override = pWeapon->~GetBotData(BOT_Override);
	if (override)
	{
		pWeapon->~UTBotAIUse(this, target);
		return;
	}
	*/

	// TODO: select firemode
	if (Inside
	(
		ObjectDistance(bot, target),
		weapon->~GetBotData(BOT_RangeMin, 2),
		weapon->~GetBotData(BOT_Range, 2)
	))
	{
		bot->UTBotAIFire(weapon, 2);
	}
	else
	{
		bot->UTBotAIFire(weapon, 1);
	}
}


func SelectWeapon(object bot, object target, int aggro_level, bool fire_modes)
{
	var arsenal, distance, selection, prio;
	
	distance = ObjectDistance(bot, target);
	
	arsenal = FindObjects(Find_Container(bot), Find_Func("IsShooterWeapon"));
	
	for (var weapon in arsenal)
	{
		/* TODO: selection logic
		if (distance <= (weapon->~GetBotData(BOT_Range)))
			if ((prio < (weapon->~GetBotData(BOT_Priority, 1))) && (distance >= (weapon->~GetBotData(BOT_Priority, 1))))
			{
				prio = weapon->~GetBotData(BOT_Priority, 1);
				
				selection = weapon;
			}
		*/ // take latest weapon
		selection = weapon;
	}

	if (selection)
		bot->ShiftContents(false, selection->GetID());
	return bot->Contents();
}



// Looks for a weapon and lets the clonk run there
func SearchWeapon(object bot, int aggro_level)
{
	for (var spawn in FindObjects(Find_Func("IsSpawnPoint"), Sort_Random())) 
	{
		var type = spawn->GetIDSpawned(); 
		if (type->~IsShooterWeapon() && !FindContents(type) && spawn->HasCollectibleItem(bot))
		{
			// TODO: Add task
			return true; //return SetMacroCommand(nil, "MoveTo", spawn, 0, 0, 0, aggro_level);
		}
	}
}

// Looks for ammo and lets the clonk run there
func SearchAmmo(object bot, int aggro_level)
{
	for (var spawn in FindObjects(Find_Func("IsSpawnPoint"), Sort_Random())) 
	{
		var type = spawn->GetIDSpawned(); 
		if (type->~IsAmmoPacket() && spawn->HasCollectibleItem(bot))
		{
			// TODO: Add task
			return true; //return SetMacroCommand(nil, "MoveTo", spawn, 0, 0, 0, aggro_level);
		}
	}
}

