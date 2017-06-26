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
	var projectile_speed = 100; //TODO this->Agent_GetProjectileSpeed(agent, weapon);
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
	if (false) //TODO this->Agent_FireDirect())
	{
		// For straight shots it is just the angle to the target if the path is free.
		if (PathFree(x, y, tx, ty))
			shooting_angle = Angle(x, y, tx, ty, 10);
	}
	// For ballistic shots get the angle (path free check is done inside).
	// The lower of the two angles is preferentially returned.
	else
	{
		shooting_angle = this->GetBallisticAngle(x, y, tx, ty, projectile_speed, 160);
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
