#appendto Clonk

/*
 Callback from the goal, when it is created.
 
 @par goal The goal object.
 */
private func OnGoalCreation(object goal)
{
	var ai = this->~GetAI();
	if (ai)
	{
		if (goal->GetID() == Goal_CaptureTheFlagEx)
		{
			Behaviour_Goal_CTF->AddTo(this);
		}
	}
}
