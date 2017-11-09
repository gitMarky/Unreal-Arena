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


/*
 Bots that were added after goal creation need to get the team strategy, too.
 */
private func Recruitment()
{
	var previous = _inherited(...);

	for (var goal in FindObjects(Find_Func("IsGoal")))
	{
		OnGoalCreation(goal);
	}

	return previous;
}
