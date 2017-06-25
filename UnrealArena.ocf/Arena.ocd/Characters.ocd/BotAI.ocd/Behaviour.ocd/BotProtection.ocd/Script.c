#include Task_Template


public func Execute(proplist controller, object bot)
{
	if (DelayBotBySkill(7))
		return TASK_EXECUTION_IN_PROGRESS;

	if (DelayBotBySkill(20, 0))
		return TASK_EXECUTION_IN_PROGRESS;

	if (bot->UTBotAIFindDodgeBullet())
	{
		bot->UTBotAIDodgeJump(bot);
	}

	return TASK_EXECUTION_IN_PROGRESS;
}

