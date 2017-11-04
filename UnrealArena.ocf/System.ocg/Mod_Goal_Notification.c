#appendto Library_ConfigurableGoal


private func Construction(object creator)
{
	_inherited(creator, ...);
	
	// Notify crew members that a goal was created
	for (var crew in FindObjects(Find_OCF(OCF_CrewMember)))
	{
		crew->~OnGoalCreation(this);
	}
}
