/**
	Aggro Target
	
	Bots can have a target that they want to fight.
 */
 
static const AGGRO_TARGET_MAX_DISTANCE = 400;
 
 
public func Agent_Initialize(object agent)
{
	_inherited(agent);

	agent->Agent_Properties()->AddFunctions([this.SetAggroTarget, this.GetAggroTarget]);
}


public func Agent_HasWeapon(object agent)
{
	return GetLength(Agent_FindInventoryWeapons(agent)) > 0;
}

public func Agent_FindInventoryWeapons(object agent)
{
	return agent->FindObjects(Find_Container(agent), Find_Func("IsShooterWeapon"));
}

public func Agent_GetCurrentWeapon(object agent)
{
	return agent->GetHandItem(0);
}

public func Agent_IsAggroTarget(object agent, object target)
{
	var is_hostile = Hostile(agent->GetOwner(), target->GetOwner(), true);
	var is_in_range = ObjectDistance(agent, target) < AGGRO_TARGET_MAX_DISTANCE;
	return is_hostile && is_in_range;
}

public func Agent_FindAggroTarget(object agent, int distance)
{
	var targets = agent->FindObjects(Find_OCF(OCF_Alive), Find_NoContainer(), Find_Distance(distance ?? AGGRO_TARGET_MAX_DISTANCE), Find_Exclude(agent), Sort_Distance());
	
	for (var target in targets)
	{
		if (Agent_IsAggroTarget(agent, target))
			return target;
	}
	return nil;
}


public func Agent_IsInterceptor(object interceptor, object aggressor, object precious)
{
	if (_inherited(interceptor, aggressor, precious))
	{
		return true;
	}
	else // Can the object intercept, because it is between aggressor and the precious?
	{
		var angle_to_aggressor = Angle(interceptor->GetX(), interceptor->GetY(), aggressor->GetX(), aggressor->GetY());
		var angle_to_precious = Angle(interceptor->GetX(), interceptor->GetY(), precious->GetX(), precious->GetY());
		
		var angle = Abs(Normalize(angle_to_aggressor - angle_to_precious, 0));
		
		return angle > 90;
	}
}

/*
public func Agent_FightAggroTarget(object agent, object target)
{
}
*/

/* -- Internals that are added to the agent logic -- */

private func SetAggroTarget(object target)
{
	this.agent_aggro_target = target;
}


private func GetAggroTarget()
{
	return this.agent_aggro_target;
}
