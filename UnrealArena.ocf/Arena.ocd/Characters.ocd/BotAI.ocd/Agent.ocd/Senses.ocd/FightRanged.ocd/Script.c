/**
	AI Ranged Weapons
	Functionality that helps the AI use ranged weapons. Handles:
	 * Bow
	 * Blunderbuss
	 * Grenade launcher

	@author Sven2, Maikel
*/


/*-- General Ranged Weapon --*/

public func Agent_FightAggroTarget(object agent, object target)
{
	// Target still in guard range?
	if (!this->Agent_IsAggroTarget(agent, target)) return false;
	// Has a weapon?
	if (!this->Agent_HasWeapon(agent)) return false;
	// Look at target.
	this->Agent_LookAt(agent, target);
	// Make sure we can shoot.
	var item = this->Agent_FindInventoryWeapons(agent)[0];
	this->Agent_SelectItem(agent, item);
	var weapon = this->Agent_GetCurrentWeapon(agent);
	if (!weapon || weapon->~RejectUse(agent))
	{
		return;
	}
	// Calculate offset to target. Take movement into account.
	var x = agent->GetX();
	var y = agent->GetY();
	var tx = target->GetX();
	var ty = target->GetY();
	var aim_for_head =  true;
	// Also aim for the head (y-4) so it's harder to evade by jumping.
	if (aim_for_head)
	{
		ty -= 4;
	}
	var d = Distance(x, y, tx, ty);
	var projectile_speed = this->Agent_GetProjectileSpeed(agent, weapon) ?? 100;
	// Projected travel time of the arrow.
	var dt = d * 10 / projectile_speed; 
	tx += this->GetTargetXDir(target, dt);
	ty += this->GetTargetYDir(target, dt);
	// Take gravity into account
	if (!target->GetContact(-1))
	{
		if (!target->GetCategory() & C4D_StaticBack)
			ty += GetGravity() * dt * dt / 200;
	}
	// Get shooting angle.
	var shooting_angle;
	if (this->Agent_FireDirect(agent, target, weapon))
	{
		// For straight shots it is just the angle to the target if the path is free.
		if (PathFree(x, y, tx, ty))
			shooting_angle = Angle(x, y, tx, ty, 10);
	}
	// For ballistic shots get the angle (path free check is done inside).
	// The lower of the two angles is preferentially returned.
	else
	{
		var projectile_range = Agent_GetProjectileRange(agent, weapon);
		shooting_angle = this->Agent_GetBallisticAngle(x, y, tx, ty, projectile_speed, projectile_range, 160);
	}
	// If we have a valid shooting angle we can proceed.
	if (shooting_angle != nil)
	{
		// No ally on path? Also search for allied animals, just in case.
		// Ignore this if requested or if no friendly fire rules is active.
		var ally;
		// TODO if (!fx.ignore_allies || FindObject(Find_ID(Rule_NoFriendlyFire)))
		//	ally = FindObject(Find_OnLine(0, 0, tx - x, ty - y), Find_Exclude(agent), Find_OCF(OCF_Alive), Find_Owner(agent->GetOwner()));
		if (ally)
		{
			// Try to jump, if not possible just wait.
			this->Agent_Jump(agent);
		}
		else
		{
			// Aim / shoot there.
			x = +Sin(shooting_angle, 1000, 10);
			y = -Cos(shooting_angle, 1000, 10);
			weapon->ControlUseHolding(agent, x, y);
		}
	}
	else
	{
		// Might also change target if current is unreachable.
		/*
		var new_target;
		if (!Random(3))
			if (new_target = this->FindTarget(fx))
				target = new_target;
		*/
		// stop using?
		weapon->ControlUseStop(agent, agent->GetCalcDir() * 100, 0);
	}
	return true;
}

// Turns around the AI such that it looks at its target.
public func Agent_LookAt(object agent, object target)
{
	// Set direction to look at target, we can assume this is instantaneous.
	if (target->GetX() > agent->GetX())
		agent->SetDir(DIR_Right);
	else
		agent->SetDir(DIR_Left);
}


// Find out whether the agent should fire directly at the object, or with ballistic aiming
public func Agent_FireDirect(object agent, object target, object weapon)
{
	var firemode = weapon->~GetFiremode();
	return !firemode.ai_fire_ballistic;
}


// Find out what the projectile speed is
public func Agent_GetProjectileSpeed(object agent, object weapon)
{
	var firemode = weapon->~GetFiremode();
	var speed = firemode.projectile_speed;

	if (GetType(speed) == C4V_Array)
	{
		return (speed[0] + speed[1]) / 2;
	}
	else if (GetType(speed) == C4V_Int)
	{
		return speed;
	}
	else
	{
		FatalError(Format("Expected int or array, got %v", GetType(speed)));
	}
}



// Find out the projectile range
public func Agent_GetProjectileRange(object agent, object weapon)
{
	var firemode = weapon->~GetFiremode();
	return firemode.projectile_range;
}



// Helper function: Convert target coordinates and projectile out speed to desired shooting angle. Because
// http://en.wikipedia.org/wiki/Trajectory_of_a_projectile says so. No SimFlight checks to check upper
// angle (as that is really easy to evade anyway) just always shoot the lower angle if sight is free.
private func Agent_GetBallisticAngle(int x, int y, int tx, int ty, int v, int range, int max_angle)
{
	var dx = tx - x;
	var dy = ty - y;	
	// The variable v is in 1/10 pix/frame.
	// The variable gravity is in 1/100 pix/frame^2.
	var g = GetGravity();
	// Correct vertical distance to account for integration error. Engine adds gravity after movement, so
	// targets fly higher than they should. Thus, we aim lower. we don't know the travel time yet, so we
	// assume some 90% of v is horizontal (it's ~2px correction for 200px shooting distance).
	dy += Abs(dx) * g * 10 / (v * 180);
	// The variable q is in 1/10000 (pix/frame)^4 and dy is negative up.
	var q = v**4 - g * (g * dx * dx - 2 * dy * v * v);
	 // Check if target is out of range.
	if (q < 0)
		return nil;
	// Return lower angle if possible.
	var lower_angle = (Angle(0, 0, g * dx, Sqrt(q) - v * v, 10) + 1800) % 3600 - 1800;
	if (Inside(lower_angle, -10 * max_angle, 10 * max_angle) && this->Agent_CheckBallisticPath(x, y, tx, ty, v, range, lower_angle))
		return lower_angle;
	// Otherwise return upper angle if that one is possible.
	var upper_angle = (Angle(0, 0, g * dx, -Sqrt(q) - v * v, 10) + 1800) % 3600 - 1800;
	if (Inside(upper_angle, -10 * max_angle, 10 * max_angle) && this->Agent_CheckBallisticPath(x, y, tx, ty, v, range, upper_angle))
		return upper_angle;	
	// No possible shooting angle.
	return nil;
}

// Returns whether the ballistic path is free.
public func Agent_CheckBallisticPath(int x, int y, int tx, int ty, int v, int range, int angle)
{
	// The projected flight time is now known.
	var vx = Sin(angle, v * 10, 10);
	var vy = -Cos(angle, v * 10, 10);
	var flight_time = 100 * Abs(tx - x) / Max(1, Abs(vx));
	// Check the flight time
	if (range != PROJECTILE_Range_Infinite)
	{
		var lifetime = (PROJECTILE_Default_Velocity_Precision * range / Max(v, 1));
		if (flight_time >= lifetime) // Exclude the last lifetime frame, too, for safety
		{
			return false;
		}
	}
	// Simulate the flight and see if the flight time corresponds to the expected time.
	var flight = this->SimFlight(x, y, vx, vy, nil, nil, flight_time, 100);
	//Log("(%d, %d)->(%d, %d) with v = (%d, %d) and angle = %d in t = %d %v", x, y, tx, ty, vx, vy, angle / 10, flight_time, flight);
	// Both the projected and the simulated flight times should be the same for a free path.
	return -flight[4] == flight_time;
}
