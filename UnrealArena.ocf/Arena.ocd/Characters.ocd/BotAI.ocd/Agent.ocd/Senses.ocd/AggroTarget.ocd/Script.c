/**
	Aggro Target
	
	Bots can have a target that they want to fight.
 */
 
 
public func Agent_Initialize(object agent)
{
	_inherited(agent);

	agent->Agent_Properties()->AddFunctions([this.SetAggroTarget, this.GetAggroTarget]);
}


public func Agent_HasWeapon(object agent)
{
}

public func Agent_IsAggroTarget(object agent, object target)
{
}

public func Agent_FindAggroTarget(object agent)
{
}

public func Agent_FightAggroTarget(object agent, object target)
{
}

/* -- Internals that are added to the agent logic -- */

private func SetAggroTarget(object target)
{
	this.agent_aggro_target = target;
}


private func GetAggroTarget()
{
	return this.agent_aggro_target;
}
