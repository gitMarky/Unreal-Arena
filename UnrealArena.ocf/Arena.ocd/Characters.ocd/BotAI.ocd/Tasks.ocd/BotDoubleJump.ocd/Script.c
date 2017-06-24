#include Task_Template


public func Execute(proplist controller, object bot)
{
	// Execute a double jump if it makes sence
	if (WildcardMatch(bot->GetAction(), "Jump*")
	&& bot->GBackSolid(20 * bot->GetCalcDir()))
	{
		bot->~ControlUp();
	}

	return TASK_EXECUTION_IN_PROGRESS;
}
