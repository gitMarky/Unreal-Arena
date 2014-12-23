/*-- TIM --*/

#strict 2
#appendto TIM2

public func Spawn()
{
	if(!Contents()) return RemoveObject();
	// Klassenauswahl usw. nicht abbrechen
	if(GetMenu(Contents()))
		return ScheduleCall(this,"Spawn",1);
	if(g_iCountDown >= 0) // Runde hat noch nicht begonnen!!
		return ScheduleCall(this,"Spawn",1);

	AddSpawnEffect(Contents(), Contents()->GetColorDw());
	Sound("PlayerSpawn",true,0,0,GetOwner(Contents())+1);
	RemoveObject(0,1);
}


local spawn, auto;

func Initialize() { ScheduleCall(this, "WaitForSpawnOk", 1); }

func WaitForSpawnOk()
{
	if(!Contents()) {Log("RemovedPreemptive");return RemoveObject();}
	// Klassenauswahl usw. nicht abbrechen
	if(GetMenu(Contents()))
		return ScheduleCall(this,"WaitForSpawnOk",1);

	ScheduleCall(this,"SpawnOk",100); // nach 3 Sekunden kann man Spawnen

}

func SpawnOk()
{
  if(!Contents())
    return(RemoveObject());
  if(Contents()->GetOwner() == NO_OWNER)
    return(RemoveObject());
  spawn = true;
  // Last man standing: no spawn hiding / pressing button lets you spawn as fast as possible
  if(FindObject(GLMS) || auto)
  {
	Spawn();
  }
  else
  {
	ScheduleCall(this(), "Spawn", 350); // nach 10 weiteren Sekunden wird man gezwungenermaßen rausgeschmissen
  }
}

public func ContainedLeft( pCaller )	{ ManualSpawn( pCaller ); return 1; }
public func ContainedRight( pCaller )	{ ManualSpawn( pCaller ); return 1; }
public func ContainedDown( pCaller )	{ ManualSpawn( pCaller ); return 1; }
public func ContainedUp( pCaller )		{ ManualSpawn( pCaller, true ); return 1; }
public func ContainedDig( pCaller )		{ ManualSpawn( pCaller ); return 1; }
public func ContainedThrow( pCaller )	{ ManualSpawn( pCaller ); return 1; }

func ManualSpawn( pCaller, bSelect )
{
	if(!spawn)
	{
		if(MOD_ClassSelection() && bSelect)
		{
			FindObject(ID_Rule_ClassSelection)->~ReOpenClassMenu(pCaller);
		}
		else
			auto = 1;
	}
	else
		Spawn();
}
