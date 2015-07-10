
/*
Creates a visual explosion effect at a position.
smoothness (in percent) determines how round the effect will look like
*/
global func ExplosionEffectSmoke(int level, int x, int y, int smoothness)
{
	var iRadius = level;
	var iDamage = level;

	CastObjectsCall([Effect_SmokeStack, 6, 100, x, y], "Launch", 25 * iRadius / 10);

	for (var i = 0; i < 3; i++)
	{
		// some smoke
		SmokeUA(x + RandomX(-iRadius, iRadius) / 2,
			    y + RandomX(-iRadius, iRadius) / 2,
			    iRadius * 5,
			    SMOKE_UA_Movement_Random,
			    15 * iRadius / 10,
			    RGBa(60, 60, 60, 195),
			    RGBa(20, 20, 20, 0),
			    SMOKE_UA_Color_Linear);

		// cast flames
		var flame = CreateObject(Effect_BlazingFlame, x, y, NO_OWNER);

		var rot = Random(360);
		var dist = iRadius + Random(30);

		flame->~SetUpSpark(iDamage + Random(20), +Sin(rot, dist), -Cos(rot, dist));
	}

	ExplosionEffectBlast(level, x, y, smoothness);
}


/*
Creates a visual explosion effect at a position.
smoothness (in percent) determines how round the effect will look like
*/
global func ExplosionEffectBlast(int level, int x, int y, int smoothness)
{
	var iRadius = level;
	
	if (!this) return;

	x = AbsX(x);
	y = AbsY(y);

	var con, rad, size, rot, rrd, shock, shockr;

	con = 100 * iRadius / 32;

	rad = 16 * con / 100;
	size = 64 * con / 20;

	shock = 48 * con / 100;
	//shockr = 72 * con / 100;
	shockr = 64 * con / 100;

	var explosion_color = this->~GetExplosionColor();
	var col_center, col_amb1, col_amb2;

	if (!explosion_color || !(col_center = explosion_color.Center))
		col_center = RGBa(255, 50 + Random(50), 0, 80 + Random(50));
	if (!explosion_color || !(col_amb1 = explosion_color.AmbientStart))
		col_amb1 = RGBa(255, 50, 0, 80);
	if (!explosion_color || !(col_amb2 = explosion_color.AmbientEnd))
		col_amb2 = RGBa(255, 100, 0, 130);

	for (var i = 0; i < 5; i++)
	{
		rot = Random(360);
		rrd = Random(rad);

		CreateParticle("Magic", x + Sin(rot, rrd), y - Cos(rot, rrd), Sin(rot, 5), -Cos(rot, 5), PV_Random(45, 55), Particles_FxExplosion_Blast(size, col_center));
	}

	// this was the same for all explosions, so let's leave it that way for now
	CreateParticle("Magic", x, y, 0, 0, PV_Random(18, 22), Particles_FxExplosion_Blast(size, RGBa(220 - Random(30), 255, 0, 80 + Random(50))));

	for (var i = 0; i < shock; i++)
	{
		var angle = Random(360);
		var speed = RandomX(shockr / 8, shockr / 2);

		var particle = Particles_FxExplosion_Blast(0, -1);
		
		particle.R = PV_Linear(GetRGBaValue(col_amb1, RGBA_RED), GetRGBaValue(col_amb2, RGBA_RED));
		particle.G = PV_Linear(GetRGBaValue(col_amb1, RGBA_GREEN), GetRGBaValue(col_amb2, RGBA_GREEN));
		particle.B = PV_Linear(GetRGBaValue(col_amb1, RGBA_BLUE), GetRGBaValue(col_amb2, RGBA_BLUE));
		particle.ForceY = GetGravity();
		particle.Size = PV_Linear(RandomX(8, 12), 2);
		particle.Stretch = PV_Speed(1000, 200);

		CreateParticle("Magic", x, y, Sin(angle, speed), -Cos(angle, speed), PV_Random(23, 28), particle);
	}

	// temporary light effect
	CreateLight(x, y, level, Fx_Light.LGT_Blast)->SetLightColor(col_center);
}


global func CastObjectsCall( aCastObjects, szCall, Par2, Par3, Par4, Par5, Par6, Par7, Par8, Par9 )
{
	// exclude existing objects
	var objects = FindObjects( Find_ID(aCastObjects[0]), Find_Distance( aCastObjects[2], aCastObjects[3], aCastObjects[4]));

	CastObjects( aCastObjects[0],
				 aCastObjects[1],
				 aCastObjects[2],
				 aCastObjects[3],
				 aCastObjects[4]
	);

	var newobj = FindObjects( Find_ID(aCastObjects[0]), Find_Distance( aCastObjects[2], aCastObjects[3], aCastObjects[4]));

	var pcall;
	for( pcall in newobj )
	{
		if(GetLength(objects))
			if(IsValueInArray(objects, pcall)) continue; // skip existing objects
		pcall->Call(szCall, Par2, Par3, Par4, Par5, Par6, Par7, Par8, Par9 );
	}
}

global func Particles_FxExplosion_Blast(int size, int color)
{
	return
	{
		Size = PV_KeyFrames(0, 0, size / 8, 200, size / 5, 1000, size / 10), 
		Rotation = PV_Direction(),
		OnCollision = PC_Stop(),
		CollisionVertex = 500,
	    R = GetRGBaValue(color, RGBA_RED),
	    G = GetRGBaValue(color, RGBA_GREEN),
	    B = GetRGBaValue(color, RGBA_BLUE),
	    Alpha = PV_Linear(GetRGBaValue(color, RGBA_ALPHA), 0),
		BlitMode = GFX_BLIT_Additive,
	};
}
