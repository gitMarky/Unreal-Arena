#include Task_Template


public func Execute(proplist controller, object bot)
{
/*
	if (DelayBotBySkill(7))
		return TASK_EXECUTION_IN_PROGRESS;

	if (DelayBotBySkill(20, 0))
		return TASK_EXECUTION_IN_PROGRESS;
*/
	var logic = controller->GetAgent();

	var projectiles = logic->Agent_FindProjectiles(bot);
	for (var projectile in projectiles)
	{
		var dx = projectile->GetX() - bot->GetX();
		var dy = projectile->GetY() - bot->GetY();
		var vx = projectile->GetXDir();
		var vy = projectile->GetYDir();
		if (Abs(dx) > 40 && vx)
		{
			dy += (Abs(10 * dx / vx)**2) * GetGravity() / 200;
		}
		var v2 = Max(vx * vx + vy * vy, 1);
		var d2 = dx * dx + dy * dy;
		var time_to_impact = 10 * Sqrt(d2) / Sqrt(v2);
		if (time_to_impact > 20)
		{
			// Won't hit within the next 20 frames.
			continue;
		}
		
		var l = dx * vx + dy * vy;
		var relative_distance = 100;
		if (l < 0)
		{
			relative_distance = Sqrt(d2 - l * l / v2);
		}
		
		if (logic->Agent_ProjectileCanHit(bot, projectile, relative_distance)
		 && logic->Agent_AvoidProjectile(bot, projectile))
		{
			break;
		}
	}

	// Nothing to do.
	return TASK_EXECUTION_IN_PROGRESS;
}

