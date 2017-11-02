#appendto RelaunchContainerEx


public func OnInitializeCrew(object crew)
{
	//if (RoundManager()->IsRoundActive())
	//{
		crew->~OnStartRelaunch();
	//}
}


public func OnRelaunchCrew(object crew)
{
	if (RoundManager()->IsRoundActive())
	{
		crew->Sound("SpawnPlayer");
		crew->~OnFinishRelaunch();
	}
}
