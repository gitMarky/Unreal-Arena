#appendto RelaunchContainerEx

public func OnRelaunchCrew(object crew)
{
	if (IsRoundActive()) crew->Sound("SpawnPlayer");
}
