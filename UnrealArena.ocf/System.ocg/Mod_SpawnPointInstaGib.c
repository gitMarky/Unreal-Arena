#appendto SpawnPoint

protected func OnRoundStart()
{
	// the spawn point is inactive when insta gib is configured!
	RemoveSpawnedObjects();

	if (!IsInstaGibConfigured())
	{
		StartSpawning();
	}
}
