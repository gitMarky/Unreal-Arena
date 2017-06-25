/**
	Jump
 */

public func Agent_Jump(object agent)
{
	// Jump if standing
	if (_inherited(agent)) return true;

	// Jump if flying
	return agent->GetProcedure() == "Flight" && agent->~ControlJump();
}