
/*
movement

0 = keine Bewegung
1 = Standard-Clonk Rauch
2 = Zufallsbewegung auf der Stelle

*/

/*
effect

0 = keine Farbübergänge
1 = Farbübergang von rgba1 nach rgba2
2 = Zufallsfarbe zwischen rgba1 und rgba2
3 = wie 2,nur wechselnd

*/

global func SmokeX(int x, int y, int con, int move_style, int lifetime, int dwColor1, int dwColor2,int effect_style)
{
	con /= 2;
	var size = con * 64 / 100;
	var r1, g1, b1, a1;
	var r2, g2, b2, a2;
	
	r1 = GetRGBaValue(dwColor1, 1);
	g1 = GetRGBaValue(dwColor1, 2);
	b1 = GetRGBaValue(dwColor1, 3);
	a1 = GetRGBaValue(dwColor1, 0);
	
	r2 = GetRGBaValue(dwColor2, 1);
	g2 = GetRGBaValue(dwColor2, 2);
	b2 = GetRGBaValue(dwColor2, 3);
	a2 = GetRGBaValue(dwColor2, 0);
	
	if (!a1 && !a2)
	{
		a1 = 200;
		a2 = 0;
	}

	var force_x, force_y;

	var random = PV_Random(-10, 10);

	if (move_style == 1)
	{
		force_y = -20;
		force_x = PV_KeyFrames(0, 0, random, 300, random, 600, random, 1000, random);
	}
	else if (move_style == 2)
	{
		force_x = force_y = PV_KeyFrames(0, 0, random, 300, random, 600, random, 1000, random);
	}

	if (!lifetime) lifetime = PV_Random(30, 35);
	var particle = {Prototype = Particles_Thrust(),
					Size = PV_Linear(75 * size / 100, 120 * size / 100),
					ForceX = force_x,
					ForceY = force_y};

	if (effect_style == 0)
	{
		particle.R = 255;
		particle.G = 255;
		particle.B = 255;
		particle.Alpha = 128;
	}
	else if (effect_style == 1)
	{
		particle.R = PV_Linear(r1, r2);
		particle.G = PV_Linear(g1, g2);
		particle.B = PV_Linear(b1, b2);
		particle.Alpha = PV_Linear(a1, a2);
	}
	else if (effect_style >= 2)
	{
		particle.R = PV_Random(r1, r2, 100);
		particle.G = PV_Random(g1, g2, 100);
		particle.B = PV_Random(b1, b2, 100);
		particle.Alpha = PV_Random(a1, a2, 100);
		
		if (effect_style == 3)
		{
			particle.R = PV_KeyFrames(0, 0, particle.R, 300, particle.R, 600, particle.R, 1000, particle.R);
			particle.G = PV_KeyFrames(0, 0, particle.G, 300, particle.G, 600, particle.G, 1000, particle.R);
			particle.B = PV_KeyFrames(0, 0, particle.B, 300, particle.B, 600, particle.B, 1000, particle.R);
			particle.Alpha = PV_KeyFrames(0, 0, particle.Alpha, 300, particle.Alpha, 600, particle.Alpha, 1000, particle.Alpha);
		}		
	}

	CreateParticle("Smoke2", x, y, 0, 0, lifetime, particle);
}
