global func CreateCartridgeEffect(string particle, int size, int x, int y, int xdir, int ydir)
{
	CreateParticle(particle, x, y, xdir, ydir, PV_Random(60, 120), Particles_Cartridge(size), 1);
}

global func Particles_Cartridge(int size)
{
	return
	{
		Alpha = PV_KeyFrames(0, 0, 255, 950, 255, 1000, 0),
		R = 255, G = 255, B = 255,
		Rotation = PV_Step(PV_Random(1, 4), PV_Random(60)),
		CollisionVertex = 0,
		OnCollision = PC_Bounce(500),
		ForceY = PV_Gravity(500),
		Size = size
	};
}


