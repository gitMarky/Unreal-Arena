/**
 A chain, sort of like Randrian's rope physics. The chain can break however, or tumble around freely.
 @author Marky
 @version 0.1.0
 */

local prev; // object - the top of the chain. This is the object that does all the calculations
local next;	// object - hangs below the head
local segments; // array list of objects
local length; // int - length of the segment

local particles; // proplist - see Verlet_Info below
local fixed;	 // bool - object has no external forces

static const CHAIN_Max_Segments = 15;
static const CHAIN_Precision = 100;
static const CHAIN_Constraint_Iterations = 5;

static const Verlet_Info = {
	x_cur = 0,
	y_cur = 0,
	x_old = 0,
	y_old = 0,
	x_acc = 0,
	y_acc = 0,
	mass = 1,
	friction = false,
};

global func VerletInfo(int x, int y, int x_acc, int y_acc, int mass, int precision)
{
	if (!precision) precision = 1;
	
	precision = CHAIN_Precision / precision;

	var verlet = { Prototype = Verlet_Info };
	if (x) verlet.x_cur = verlet.x_old = x * precision;
	if (y) verlet.y_cur = verlet.y_old = y * precision;
	if (x_acc) verlet.x_acc = x_acc * precision;
	if (y_acc) verlet.y_acc = y_acc * precision;
	if (mass) verlet.mass = mass;
	
	return verlet;
}

public func AttachTo(object previous)
{
	if (prev)
	{
		FatalError("Object is already attached to %v", prev);
	}
	if (previous == nil)
	{
		FatalError("The object has to attach to an object that is not nil");
	}
	if (!previous->IsChain())
	{
		FatalError("Can attach only to object that return 'true' in IsChain()");
	}
	if (previous->GetNext())
	{
		FatalError("Cannot attach to an object that has an object attached");
	}

	prev = previous;
	prev->UpdateSegments();
	
	if (fixed && !IsLast()) SetFixed(false);
}

public func IsChain()
{
	return true;
}

public func IsFirst()
{
	return !GetPrevious();
}

public func GetPrevious()
{
	return prev;
}

public func IsLast()
{
	return !GetNext();
}

public func GetNext()
{
	return next;
}

public func UpdateSegments()
{
	segments = [];
	
	PushBack(segments, this);

	var segment = GetNext();
	for (var i = 0; i < CHAIN_Max_Segments; i++)
	{
		if (segment == nil) break;
		
		PushBack(segments, segment);
		segment = segment->GetNext();
	}
}

public func SetParticle(int index, proplist verlet)
{
	particles[index] = verlet;
	
	var last = GetLength(particles) - 1;
	if (index == last)
	{
		length = Distance(GetParticle(0).x_cur, GetParticle(0).y_cur, GetParticle(last).x_cur, GetParticle(last).y_cur);		
	}
}

public func GetParticles()
{
	return particles;
}

public func GetParticle(int index)
{
	return particles[index];
}

protected func Initialize()
{
	particles = [];
	SetParticle(0, VerletInfo(0, -1));
	SetParticle(1, VerletInfo(0, +1));
	SetFixed(true);
	AddEffect("IntHang", this, 1, 1, this);
}

protected func SetFixed(bool state)
{
	fixed = state;
}

private func FxIntHangTimer(object target, proplist effect, int time)
{
	if (IsFirst()) TimeStep();
}

private func TimeStep()
{
	for (var segment in segments) segment->Verlet();
	for (var segment in segments) segment->SatisfyConstraints();
	for (var segment in segments) segment->ForcesOnObjects();
	for (var segment in segments) segment->UpdateLines();
}

/**
 Verlet integration step
 Moves the particles according to their old position and thus speed.
 @author Randrian
*/
private func Verlet()
{
	for(var i = 0; i < GetLength(GetParticles()); i++)
	{
		var particle = GetParticle(i);
		var x_temp = particle.x_cur;
		var y_temp = particle.y_cur;

		// Verlet step, get speed out of distance moved relativ to the last position
		particle.x_cur += particle.x_cur - particle.x_old + particle.x_acc;
		particle.y_cur += particle.y_cur - particle.y_old + particle.y_acc;
		particle.x_old = x_temp;
		particle.y_old = y_temp;
		particle.friction = false;
	}
}


/**
 Satisfying the constraints for the particles.
 The constraints are:@br
 - staying at the position of the objects,@br
 - respecting the length to the next particles,@br
 - and staying out of material
 @author Randrian
*/
private func SatisfyConstraints()
{
	for(var j = 0; j < CHAIN_Constraint_Iterations; j++)
	{
		ConstraintObjects();
		ConstraintLength();
		ConstraintLandscape();
	}
	
	ApplyFriction();
}

private func ApplyFriction()
{
	// Apply friction for those how have the notifier for it.
	// Friction just means that the velocity is divided by 2 to simulatie a friction force
	for(var i = 0; i < GetLength(GetParticles()); i++)
	{
		var particle = GetParticle(i);
		if(!particle.friction) continue;

		var newvel_x = particle.x_cur - particle.x_old;
		var newvel_y = particle.y_cur - particle.y_old;
		
		particle.x_old = particle.x_cur - newvel_x / 2;
		particle.y_old = particle.y_cur - newvel_y / 2;
	}
}

public func ConstraintObjects()
{
	// move all particles to where the first particle has moved
	if (prev)
	{
		var last = GetLength(prev->GetParticles()) - 1;
		
		var x_diff = prev->GetParticleX(last) - GetParticleX(0);
		var y_diff = prev->GetParticleY(last) - GetParticleY(0);
		
		for (var i = 0; i < GetLength(GetParticles()); i++)
		{
			SetParticleX(GetParticleX(i) + x_diff, i, CHAIN_Precision);
			SetParticleX(GetParticleY(i) + y_diff, i, CHAIN_Precision);
		}
	}
}

public func ConstraintLength()
{
	var last = GetLength(particles) - 1;

	var x_diff = GetParticle(last).x_cur - GetParticle(0).x_cur;
	var y_diff = GetParticle(last).y_cur - GetParticle(0).y_cur;
	
	var current_length = Distance(0, 0, x_diff, y_diff);
	
	GetParticle(last).x_cur = GetParticle(0).x_cur + x_diff * length / current_length;
	GetParticle(last).y_cur = GetParticle(0).y_cur + y_diff * length / current_length;
}

public func ConstraintLandscape()
{
}

public func GetParticleX(int index)
{
	return GetX(CHAIN_Precision) + GetParticle(index).x_cur;
}

public func GetParticleY(int index)
{
	return GetY(CHAIN_Precision) + GetParticle(index).y_cur;
}

public func SetParticleX(int value, int index, int precision)
{
	if (!precision) precision = 1;
	precision = CHAIN_Precision / precision;

	GetParticle(index).x_cur = value * precision - GetX(CHAIN_Precision);
}

public func SetParticleY(int value, int index, int precision)
{
	if (!precision) precision = 1;
	precision = CHAIN_Precision / precision;

	GetParticle(index).y_cur = value * precision - GetY(CHAIN_Precision);
}
