
public func Launch(int lifetime)
{
	local timer, end, change;

	timer = lifetime;
	end = timer / 2;

	change = BoundBy(timer / 18, 1, timer);

	local rgb1, rgb2;

	rgb1 = 255 - Random(30);
	rgb2 = 100 - Random(30);

	AddEffect("IntTravel", this, 1, 1, this);
}

private func FxIntTravelTimer()
{
	timer = BoundBy(timer - change, end, timer);

	SmokeUA(0, 0, timer, SMOKE_UA_Movement_Random, RandomX(25, 40), PV_Linear(RGBa(rgb1, rgb1, rgb1, 190), RGBa(rgb2, rgb2, rgb2, 0)));

	if (timer <= end) RemoveObject();
}

private func ContactBottom()
{
	return RemoveObject();
}
