
static const SMOKE_UA_Movement_None = 0;
static const SMOKE_UA_Movement_Standard = 1;
static const SMOKE_UA_Movement_Random = 2;

global func SmokeUA(int x, int y, int con, int move_style, int lifetime, color, string name)
{
	con /= 2;
	var size = con * 64 / 100;
	var r1, g1, b1, a1;
	var r2, g2, b2, a2;

	var force_x, force_y;

	var random = PV_Random(-10, 10);

	if (move_style == SMOKE_UA_Movement_Standard)
	{
		force_y = -20;
		force_x = PV_KeyFrames(0, 0, random, 300, random, 600, random, 1000, random);
	}
	else if (move_style == SMOKE_UA_Movement_Random)
	{
		force_x = force_y = PV_KeyFrames(0, 0, random, 300, random, 600, random, 1000, random);
	}

	if (!lifetime) lifetime = PV_Random(30, 35);
	var particle = {Prototype = Particles_Thrust(),
					Size = PV_Linear(75 * size / 100, 120 * size / 100),
					ForceX = force_x,
					ForceY = force_y};

	if (color == nil)
	{
		r1 = 255;
		g1 = 255;
		b1 = 255;
		a1 = 128;
	}
	else if (GetType(color) == C4V_Int)
	{
		r1 = GetRGBaValue(color, RGBA_RED);
		g1 = GetRGBaValue(color, RGBA_GREEN);
		b1 = GetRGBaValue(color, RGBA_BLUE);
		a1 = GetRGBaValue(color, RGBA_ALPHA);
	}
	else if (GetType(color) == C4V_Array)
	{
		r1 = GetRGBaValue(color[1], RGBA_RED);
		g1 = GetRGBaValue(color[1], RGBA_GREEN);
		b1 = GetRGBaValue(color[1], RGBA_BLUE);
		a1 = GetRGBaValue(color[1], RGBA_ALPHA);

		r2 = GetRGBaValue(color[2], RGBA_RED);
		g2 = GetRGBaValue(color[2], RGBA_GREEN);
		b2 = GetRGBaValue(color[2], RGBA_BLUE);
		a2 = GetRGBaValue(color[2], RGBA_ALPHA);
	}

	if (!a1 && !a2)
	{
		a1 = 200;
		a2 = 0;
	}

	if (color == nil || GetType(color) == C4V_Int)
	{
		particle.R = r1;
		particle.G = g1;
		particle.B = b1;
		particle.Alpha = a1;
	}
	else if (GetType(color) == C4V_Array)
	{
		if (color[0] == PV_Linear()[0])
		{
			particle.R = PV_Linear(r1, r2);
			particle.G = PV_Linear(g1, g2);
			particle.B = PV_Linear(b1, b2);
			particle.Alpha = PV_Linear(a1, a2);
		}
		else if (color[0] == PV_Random()[0])
		{
			particle.R = PV_Random(r1, r2, 100);
			particle.G = PV_Random(g1, g2, 100);
			particle.B = PV_Random(b1, b2, 100);
			particle.Alpha = PV_Random(a1, a2, 100);

			if (color[3])
			{
				particle.R = PV_KeyFrames(0, 0, particle.R, 300, particle.R, 600, particle.R, 1000, particle.R);
				particle.G = PV_KeyFrames(0, 0, particle.G, 300, particle.G, 600, particle.G, 1000, particle.R);
				particle.B = PV_KeyFrames(0, 0, particle.B, 300, particle.B, 600, particle.B, 1000, particle.R);
				particle.Alpha = PV_KeyFrames(0, 0, particle.Alpha, 300, particle.Alpha, 600, particle.Alpha, 1000, particle.Alpha);
			}		
		}
	}

	if (name == nil) name = "Smoke2";

	CreateParticle(name, x, y, 0, 0, lifetime, particle);
}
