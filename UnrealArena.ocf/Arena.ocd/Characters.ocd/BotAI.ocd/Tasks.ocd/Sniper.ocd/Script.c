#include Task_Template

private func CreateTask()
{
	var task = inherited();
	task->AddFunctions([this.SetTarget, this.GetTarget]);
	return task;
}


public func Execute(proplist controller, object agent)
{
	var logic = controller->GetAgent();
	var owner = agent->GetOwner();

	if (!logic->Agent_HasWeapon(agent)) 
	{
		if (!this.TaskGetWeapon)
		{
			this.TaskGetWeapon = Task_GetWeapon->AddTo(agent, TASK_PRIORITY_HIGH, this);
		}

		return TASK_EXECUTION_IN_PROGRESS;
	}

	var vantage_points = FindObjects(Find_Func("IsWaypoint"), Find_Func("IsSniperPoint", GetPlayerTeam(owner)));
 	AI_Debugging->LogAI_Info(controller, Format("Found vantage points %v", vantage_points));

 	if (GetLength(vantage_points) == 0)
 	{
 		return TASK_EXECUTION_FAILURE;
 	}

	var is_at_vantage_point;
 	for (var vantage_point in vantage_points)
 	{
		if (logic->Agent_IsNear(agent, vantage_point))
		{
			is_at_vantage_point = true;
			break;
		}
	}
	if (is_at_vantage_point)
	{
		var target = agent->Agent_Properties()->GetAggroTarget()
		          ?? logic->Agent_FindAggroTarget(agent, 1500);

		if (target)
		{
			agent->Agent_Properties()->SetAggroTarget(target);
			agent->Message("@Sniping,...");
			target->Message("@Sn: %d", agent->ObjectNumber());
		}
		else
		{
			agent->Message("@Waiting,...");
		}
	}
	else
	{
		var destination = vantage_points[Random(GetLength(vantage_points))];

		if (destination)
		{	
			Task_MoveAlongPath->AddTo(agent, TASK_PRIORITY_NORMAL, this)->SetStart(agent)->SetDestination(destination)->SetDescription("Moving to vantage point");
		}
		else
		{
			return TASK_EXECUTION_FAILURE;
		}
	}

	return TASK_EXECUTION_IN_PROGRESS;
}

