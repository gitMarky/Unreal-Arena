#include Task_GetItem


private func CreateTask()
{
	var task = inherited();
	task->SetItem(Find_Later(Global.Find_Func, "IsShooterWeapon"));
	return task;
}
