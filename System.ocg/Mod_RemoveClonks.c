#appendto Clonk

public func OnRoundReset(int round)
{
	if (!GetAlive()) RemoveObject();
}
