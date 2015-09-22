#appendto RelaunchContainerEx

public func OnRelaunchCrew(object crew)
{
	if (RoundManager()->IsRoundActive()) crew->Sound("SpawnPlayer");
}
