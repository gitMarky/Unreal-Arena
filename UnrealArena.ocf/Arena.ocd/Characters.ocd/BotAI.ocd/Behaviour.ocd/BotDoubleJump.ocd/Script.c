#include Task_Template


public func Execute(proplist controller, object bot)
{
	// Execute a double jump if it makes sence
	if (bot->GBackSolid(20 * bot->GetCalcDir()))
	{
		bot->UTBotAIDoubleJump();
	}

	return TASK_EXECUTION_IN_PROGRESS;
}
