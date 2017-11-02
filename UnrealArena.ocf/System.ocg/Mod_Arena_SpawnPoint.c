#appendto SpawnPoint

private func SpawnEffect(int index)
{
	var player = GetPlayerByIndex(index);
	Sound("SpawnItem", false, nil, player);
}
