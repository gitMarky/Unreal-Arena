/**
	Spawn point knowledge for bots.
	
	Bots know all spawn points on the map without seeing them,
	just like a player who is familiar with the map.
 */


public func Agent_FindItemType(object agent, def type) // for item collection
{
	var found = _inherited(agent, type);
	var candidates = [];
	var spawnpoints = agent->FindObjects(Find_Func("IsSpawnPoint"), Find_Func("HasCollectibleItem", agent));

	for (var spawnpoint in spawnpoints) 
	{
		if (spawnpoint->GetIDSpawned() == type)
		{
			PushBack(candidates, spawnpoint); // The agent will find the spawn point and try to collect that.
		}
	}

	if (found) PushBack(candidates, found);

	return agent->GetNearestObject(candidates);
}
