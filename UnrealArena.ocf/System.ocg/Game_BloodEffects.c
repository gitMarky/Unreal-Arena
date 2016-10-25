
global func EffectBloodSpray(int amount, int radius, int x, int y)
{
	AssertObjectContext("EffectBloodSpray");

	var type = BloodFXColor(this->~CrewGetBlood());
	//CastParticles("Blood", amount, radius, x, y, 10,	40, type[0], type[1]);
}

global func EffectBloodStream(int x, int y, int xdir, int ydir)
{
	AssertObjectContext("EffectBloodStream");

	var stream = CreateObject(Dummy, x, y, NO_OWNER);
	stream->SetSpeed(xdir, ydir);
	stream->~Launch(this->~CrewGetBlood());
}

global func EffectGoreChunk(int x, int y, int xdir, int ydir)
{
	AssertObjectContext("EffectGoreChunk");

	var chunk = CreateObject(Dummy, x, y, NO_OWNER);
	chunk->SetSpeed(xdir, ydir);
	chunk->~Launch(this->~CrewGetBlood());
}
