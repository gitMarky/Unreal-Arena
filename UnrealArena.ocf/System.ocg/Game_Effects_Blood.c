
global func Particles_UA_Blood(colors)
{
	var r0 = GetRGBaValue(colors[0], RGBA_RED);
	var g0 = GetRGBaValue(colors[0], RGBA_GREEN);
	var b0 = GetRGBaValue(colors[0], RGBA_BLUE);
	var a0 = GetRGBaValue(colors[0], RGBA_ALPHA);
	var r1 = GetRGBaValue(colors[1], RGBA_RED);
	var g1 = GetRGBaValue(colors[1], RGBA_GREEN);
	var b1 = GetRGBaValue(colors[1], RGBA_BLUE);
	var a1 = GetRGBaValue(colors[1], RGBA_ALPHA);
	return
	{
		Size = PV_Linear(PV_Random(1, 4), 0),
	    ForceY = GetGravity(),
		Stretch = PV_Speed(1000, 750),
		Rotation = PV_Direction(),
		OnCollision = PC_Die(),
		CollisionVertex = 500,
	    R = PV_Linear(r0, r1),
	    G = PV_Linear(g0, g1),
	    B = PV_Random(b0, b1),
	    Alpha = PV_Linear(a0, a1),
	};
}

global func EffectBloodColor(string type)
{
	if (type == "Machine")
	{
		return [RGBa(90, 0, 200, 210), RGBa(90, 0, 200, 95)];
	}
	else if (type == "Alien")
	{
		return [RGBa(0, 220, 0, 210), RGBa(0, 220, 0, 95)];
	}
	else
	{
		return [RGBa(255, 0, 0, 210), RGBa(200, 0, 0, 95)];
	}
}


global func EffectBloodSpray(int amount, int radius, int x, int y)
{
	if (MOD_NoBlood()) return;

	AssertObjectContext("EffectBloodSpray");

	var ydir = -3;

	var particles = Particles_UA_Blood(EffectBloodColor(this->~CrewGetBlood()));
	CreateParticle("Blood", PV_Random(x - 1, x + 1), PV_Random(y - 1, y+ 1), 
	                        PV_Random(-radius, radius), PV_Random(-radius + ydir, radius + ydir),
	                        PV_Random(60, 90), particles, amount);
}

global func EffectBloodStream(int x, int y, int xdir, int ydir)
{
	if (MOD_NoBlood()) return;

	AssertObjectContext("EffectBloodStream");

	var stream = CreateObject(Effect_BloodStream, x, y, NO_OWNER);
	stream->SetSpeed(xdir, ydir);
	stream->Launch(this->~CrewGetBlood());
}

global func EffectGoreChunk(int x, int y, int xdir, int ydir)
{
	if (MOD_NoBlood()) return;

	AssertObjectContext("EffectGoreChunk");

	var chunk = CreateObject(Effect_GoreChunk, x, y, NO_OWNER);
	chunk->SetSpeed(xdir, ydir);
	chunk->~Launch(this->~CrewGetBlood());
	return chunk;
}

global func EffectCastGore(int amount, int radius, int x, int y)
{
	if (MOD_NoBlood()) return;

	AssertObjectContext("EffectCastGore");

	var chunks = CastObjects(Effect_GoreChunk, amount, radius, x, y);
	for (var chunk in chunks)
	{
		chunk->~Launch(this->~CrewGetBlood());
	}
	return chunks;
}

global func EffectCastBloodStream(int amount, int radius, int x, int y)
{
	if (MOD_NoBlood()) return;

	AssertObjectContext("EffectCastBloodStream");

	var streams = CastObjects(Effect_BloodStream, amount, radius, x, y);
	for (var stream in streams)
	{
		stream->Launch(this->~CrewGetBlood());
	}
}

global func MOD_MoreGore()
{
	return 0; // TODO
}

global func MOD_NoBlood()
{
	return false; // TODO
}
