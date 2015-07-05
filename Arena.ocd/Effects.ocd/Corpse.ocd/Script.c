/*-- Der Clonk --*/

#include Chain

static const CORPSE_Joint_Body = 0;
static const CORPSE_Joint_Neck = 1;
static const CORPSE_Joint_Head = 2;
static const CORPSE_Joint_ArmL = 3;
static const CORPSE_Joint_ArmR = 4;
static const CORPSE_Joint_Hips = 5;
static const CORPSE_Joint_LegL = 6;
static const CORPSE_Joint_LegR = 7;

static const CORPSE_Sibling = nil;

private func SetupSegments()
{
	var x = GetX(CHAIN_Precision);
	var y = GetY(CHAIN_Precision);

	SetParticle(CORPSE_Joint_Body, Verlet_Particle(x      , y      , CHAIN_Precision), nil);
	SetParticle(CORPSE_Joint_Neck, Verlet_Particle(x      , y - 485, CHAIN_Precision), CORPSE_Joint_Body);
	SetParticle(CORPSE_Joint_Head, Verlet_Particle(x      , y - 765, CHAIN_Precision), CORPSE_Joint_Neck);
	SetParticle(CORPSE_Joint_ArmL, Verlet_Particle(x - 130, y + 285, CHAIN_Precision), CORPSE_Joint_Neck);
	SetParticle(CORPSE_Joint_ArmR, Verlet_Particle(x + 130, y + 285, CHAIN_Precision), CORPSE_Joint_Neck);
	SetParticle(CORPSE_Joint_Hips, Verlet_Particle(x      , y + 285, CHAIN_Precision), CORPSE_Joint_Body);
	SetParticle(CORPSE_Joint_LegL, Verlet_Particle(x - 130, y + 885, CHAIN_Precision), CORPSE_Joint_Hips);
	SetParticle(CORPSE_Joint_LegR, Verlet_Particle(x + 130, y + 885, CHAIN_Precision), CORPSE_Joint_Hips);

	var low = 200;
	var high = 400;

	// bounciness
	GetParticle(CORPSE_Joint_Body).OnCollision = PC_Bounce(low);
	GetParticle(CORPSE_Joint_Neck).OnCollision = PC_Bounce(low);
	GetParticle(CORPSE_Joint_Head).OnCollision = PC_Bounce(high);
	GetParticle(CORPSE_Joint_ArmL).OnCollision = PC_Bounce(high);
	GetParticle(CORPSE_Joint_ArmR).OnCollision = PC_Bounce(high);
	GetParticle(CORPSE_Joint_Hips).OnCollision = PC_Bounce(low);
	GetParticle(CORPSE_Joint_LegL).OnCollision = PC_Bounce(high);
	GetParticle(CORPSE_Joint_LegR).OnCollision = PC_Bounce(high);

	// general settings
	for (var particle in GetParticles())
	{
		particle.ConstraintLength = CF_BoundBy(950, 1000); // give a little play for length
		particle.Fixed = true;
		UpdateLength(particle);
	}
}


public func SetLArmAngle(int angle)
{
	TransformBone("skeleton_arm_upper.L", Trans_Mul(Trans_Rotate(90, 0, 1, 0), Trans_Rotate(60, 1, 0, 0), Trans_Rotate(-60, 0, 0, 1)), 5, Anim_Const(1000), CORPSE_Sibling);
}


public func SetRArmAngle(int angle)
{
	TransformBone("skeleton_arm_upper.R", Trans_Mul(Trans_Rotate(90, 0, 1, 0), Trans_Rotate(60, 1, 0, 0), Trans_Rotate(-60, 0, 0, 1)), 5, Anim_Const(1000), CORPSE_Sibling);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// properties

local Name = "$Name$";
local Description = "$Description$";
local NoBurnDecay = 1;

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// actions

local ActMap = 
{
	Dead:
	{
		Prototype: Action,
		Name: "Dead",
		Procedure: DFA_NONE,
		Directions: 2,
		FlipDir: 1,
		Length: 3,
		Delay: 0,
		X: 0,
		Y: 0,
		Wdt: 16,
		Hgt: 20,
		NextAction: "Hold"
	}
};
