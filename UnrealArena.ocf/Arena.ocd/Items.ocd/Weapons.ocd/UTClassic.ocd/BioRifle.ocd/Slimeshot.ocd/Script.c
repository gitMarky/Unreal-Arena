#include Library_Projectile

local glob_size;
local glob_color;
local glob_charge;

static const SLIMEGLOB_Wobble_Speed = 15;
static const SLIMEGLOB_Wobble_Strength = 100;
static const SLIMEGLOB_Wobble_Duration = 20;
static const SLIMEGLOB_Wobble_FallOff = 20;

static const SLIMEGLOB_Sticking_Time = 144;

static const SLIMEGLOB_Explosion_Size = 530;

public func Charge(int percent)
{
	if (percent > 0)
	{
		glob_charge = BoundBy(percent, 0, 100);
	}
}

private func OnLaunch()
{
	SetAction("TravelBallistic");
	
	if (damage == 0) damage = RandomX(5, 10);
	
	if (glob_charge) damage = glob_charge * damage / 10;
	
	//var size = 40 + 4 * damage / 5;
	//size *= 5;
	//var size = 200 + 4 * damage;
	//var size = 40 + 6 * damage / 25;
	//size *= 10;
	var size = 300 + 6 * damage / 5;

	glob_size = size;	
	StayOnHit();

	glob_color = RGBa(0, 250, 30, 160);
	//glob_color = RGBa(77, 50, 27, 220); // DrWodka-Mode color
	SetClrModulation(glob_color);

	Wobble(SLIMEGLOB_Wobble_Strength, SLIMEGLOB_Wobble_Duration, SLIMEGLOB_Wobble_FallOff, SLIMEGLOB_Wobble_Speed);
}

private func OnHitLandscape()
{
	if (!IsSticking())
	{
		Sound("Weapon::Classic::bio-hit");
		SetAction("Stick");
		Wobble(SLIMEGLOB_Wobble_Strength, SLIMEGLOB_Wobble_Duration, SLIMEGLOB_Wobble_FallOff, SLIMEGLOB_Wobble_Speed);
	}
}

private func OnHitObject(object target)
{
	if (IsSticking())
	{
		RemoveGlob();
	}
}


//////////////////////////////////////////////////////////////////////////////////////
//
// Stick to the ground

private func IsSticking()
{
	return GetAction() == "Sticking";
}

private func Sticking()
{
	var self = this;
	
	DoHitCheckCall();
	
	if (self && GetActTime() > SLIMEGLOB_Sticking_Time)
	{
		RemoveGlob();
	}
}

//////////////////////////////////////////////////////////////////////////////////////
//
// Wobble effect

private func Wobble(int strength, int duration, int decline, int degrees_per_frame)
{
	AddEffect("IntWobble", this, 1, 1, this, nil, strength, duration, strength / decline, degrees_per_frame);
}

private func DoWobble(int degrees, int strength)
{
	var skew_x = Cos(degrees, 2 * strength);
	var skew_y = Cos(degrees, -strength);
	SetObjDrawTransform(glob_size, skew_x, nil, skew_y, glob_size);
}

private func FxIntWobbleStart(object target, proplist effect, int temp, int strength, int duration, int decrease, int degrees_per_frame)
{
	effect.strength = strength;
	effect.duration = duration;
	effect.decrease = Max(1, decrease);
	effect.degrees = Max(1, degrees_per_frame);
	effect.offset = Random(360);
}

private func FxIntWobbleTimer(object target, proplist effect, int time)
{
	target->DoWobble(effect.offset + time * effect.degrees, effect.strength);

	if (time > effect.duration) effect.strength -= effect.decrease;
	if (effect.strength <= 0) return FX_Execute_Kill;

	return FX_OK;
}


//////////////////////////////////////////////////////////////////////////////////////
//
// Exploding 

private func RemoveGlob()
{
	if (glob_size > SLIMEGLOB_Explosion_Size)
	{
		SplashGlob();
	}
	
	CreateImpactEffect(glob_size / 50, 0, 0, "Magic", Particles_SlimeGlob());

	Remove();
}

private func SplashGlob()
{
	var constant = Weapon_UT99_BioRifle.firemode_primary.damage;

	damage -= constant;

	while (damage > 0)
	{
		CastBlob(Min(damage, constant));
		damage -= constant;
	}
}

private func CastBlob(int dmg)
{
	// determine an angle that does not point to solid material, but do only so many tries
	var angle = Random(360);
	var distance = 5;
	for (var i = 0; i < 10; i++)
	{
		if (!GBackSolid(Sin(angle, distance), -Cos(angle, distance))) break;
		
		angle = Random(360);
	}
	
	// launch it
	var projectile = CreateObject(GetID(), 0, 0, user->GetController());

	projectile->Shooter(user)
			  ->Weapon(weapon_ID)
			  ->DamageAmount(dmg)
			  ->DamageType(damage_type)
			  ->Velocity(RandomX(glob_size/60, glob_size/30))
			  ->Range(PROJECTILE_Range_Infinite)
			  ->Launch(angle, nil);
}

private func Particles_SlimeGlob()
{
	return
	{
		Prototype = Particles_Glimmer(), 
	    R = PV_Linear(GetRGBaValue(glob_color, RGBA_RED), GetRGBaValue(glob_color, RGBA_RED) / 2),
	    G = PV_Linear(GetRGBaValue(glob_color, RGBA_GREEN), GetRGBaValue(glob_color, RGBA_GREEN) / 2),
	    B = PV_Linear(GetRGBaValue(glob_color, RGBA_BLUE), GetRGBaValue(glob_color, RGBA_BLUE) / 2),
	    Alpha = GetRGBaValue(glob_color, RGBA_ALPHA),
	};
}

//////////////////////////////////////////////////////////////////////////////////////
//
// Actions

local ActMap = {
	TravelBallistic = {
		Prototype = Action,
		Name = "TravelBallistic",
		Procedure = DFA_NONE,
		Length = 1,
		Delay = 1,
		FacetBase = 0,
		X = 0,
		Y = 0,
		Wdt = 10,
		Hgt = 10,
		OffX = -2,
		OffY = -2,
		NextAction = "TravelBallistic",
		StartCall = "Travelling",
	},
	
	Stick = {
		Prototype = Action,
		Name = "Stick",
		Procedure = DFA_FLOAT,
		Length = 1,
		Delay = 1,
		X = 0,
		Y = 0,
		Wdt = 10,
		Hgt = 10,
		OffX = -2,
		OffY = -2,
		NextAction = "Stick",
		StartCall = "Sticking",
	},
};