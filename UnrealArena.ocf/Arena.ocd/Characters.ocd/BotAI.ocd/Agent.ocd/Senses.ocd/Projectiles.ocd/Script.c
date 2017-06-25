/**
	Projectile logic for bots
	
 */


public func Agent_Initialize(object agent)
{
	_inherited(agent);

	agent->Agent_Properties()->AddFunction(this.GetProjectileFindArea);
}


public func Agent_FindProjectiles(object agent)
{
	var found = _inherited(agent);
	var projectiles = agent->FindObjects(Find_Now(agent->Agent_Properties()->GetProjectileFindArea()), Find_Or(Find_Func("IsDangerous4AI"), Find_Func("IsProjectile")), Find_NoContainer(), Sort_Distance());
	
	if (found && GetLength(found) > 0) PushBack(projectiles, found);
	return 	projectiles;
}


public func Agent_ProjectileCanHit(object agent, object projectile, int relative_distance)
{
	// Distance at which projectile will pass clonk should be larger than clonk size (erroneously assumes clonk is a sphere).
	if (relative_distance * agent->GetDefHeight() <= agent->GetCon())
	{
		// If there is no wall between.
		if (PathFree(agent->GetX(), agent->GetY(), projectile->GetX(), projectile->GetY()))
			return true;
	}
	return false;
}


public func Agent_AvoidProjectile(object agent, object projectile)
{
	if (_inherited(agent, projectile))
	{
		return true;
	}

	var dx = projectile->GetX() - agent->GetX();

	// Try to jump away.
	if (dx < 0)
		agent->SetComDir(COMD_Right);
	else
		agent->SetComDir(COMD_Left);
	
	return agent->GetAI()->GetAgent()->~Agent_Jump(agent);
}

/* -- Internals that are added to the agent logic -- */


private func GetProjectileFindArea()
{
	return Find_Later(Global.Find_InRect, -180, -60, 360, 120);
}
