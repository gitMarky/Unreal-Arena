/**
	Spawn point knowledge for bots.
	
	Bots know all spawn points on the map without seeing them,
	just like a player who is familiar with the map.
 */


public func Agent_FindItem(object agent, array find_criteria) // for item collection
{
	var found = _inherited(agent, find_criteria);
	var candidates = [];
	var spawnpoints = agent->FindObjects(Find_Func("IsSpawnPoint"), Find_Func("HasCollectibleItem", agent));

	for (var spawnpoint in spawnpoints) 
	{
		if (GetLength(FindObjects(Find_Container(spawnpoint), find_criteria)))
		{
			PushBack(candidates, spawnpoint); // The agent will find the spawn point and try to collect that.
		}
	}

	if (found) PushBack(candidates, found);

	return agent->GetNearestObject(candidates);
}
