/**
 A chain, sort of like Randrian's rope physics. The chain can break however, or tumble around freely.
 @author Marky
 @version 0.1.0
 */
 
 #include Particle

local prev; // object - the top of the chain. This is the object that does all the calculations
local next;	// object - hangs below the head
local segments; // array list of objects
local length; // int - length of the segment

local particles; // proplist - see Verlet_Info below
local fixed;	 // bool - object has no external forces

local particle_objects;

static const CHAIN_Max_Segments = 15;
static const CHAIN_Precision = 100;
static const CHAIN_Constraint_Iterations = 5;

public func AttachTo(object previous)
{
	if (prev)
	{
		FatalError(Format("Object is already attached to %v", prev));
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
	prev->SetNext(this);
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

public func SetNext(object obj)
{
	next = obj;
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
	
	var last = GetLength(GetParticles()) - 1;
	if (index == last)
	{
		length = Vec_Length(Vec_Sub(GetParticle(last).pos_cur, GetParticle(0).pos_cur));
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
	particle_objects = [];
	segments = [];
	SetParticle(0, VerletInfo(GetX()-3, GetY()-3));
	SetParticle(1, VerletInfo(GetX(), GetY() + 5));
	SetFixed(true);
	UpdateSegments();
	AddEffect("IntHang", this, 1, 1, this);
}

protected func SetFixed(bool state)
{
	fixed = state;
	
	UpdateGravity();
}

private func FxIntHangTimer(object target, proplist effect, int time)
{
	if (IsFirst()) TimeStep();
}

private func TimeStep()
{
	for (var segment in segments) segment->~VerletIntegration();
	for (var segment in segments) segment->~SatisfyConstraints();
	for (var segment in segments) segment->~ForcesOnObjects();
	for (var segment in segments) segment->~UpdateLines();
}

/**
 Verlet integration step
 Moves the particles according to their old position and thus speed.
 @author Randrian
*/
private func VerletIntegration()
{
	for(var i = 0; i < GetLength(GetParticles()); i++)
	{
		VerletStep(GetParticle(i), !fixed || i > 0);
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

	HandleCollision();
}

private func HandleCollision()
{
	// Apply friction for those how have the notifier for it.
	// Friction just means that the velocity is divided by 2 to simulatie a friction force
	for(var particle in GetParticles())
	{
		ParticleFriction(particle);
	}
}

public func ConstraintObjects()
{
	// move all particles to where the first particle has moved
	if (prev)
	{
		var last = GetLength(prev->GetParticles()) - 1;
		
		var diff = Vec_Sub(prev->GetParticlePos(last), GetParticlePos(0));
		
		for (var i = 0; i < GetLength(GetParticles()); i++)
		{
			SetParticlePos(Vec_Add(GetParticlePos(i), diff), i, CHAIN_Precision);
		}
	}
}

public func ConstraintLength()
{
	var last = GetLength(GetParticles()) - 1;

	var diff = Vec_Sub(GetParticlePos(last), GetParticlePos(0));
	
	var current_length = Vec_Length(diff);
	
	SetParticlePos(Vec_Add(GetParticlePos(0), Vec_Div(Vec_Mul(diff, length), current_length)), last, CHAIN_Precision);
}

public func ConstraintLandscape()
{
	for(var particle in GetParticles())
	{
		ParticleLandscape(particle);
	}
}


public func GetParticlePos(int index)
{
	return GetParticle(index).pos_cur;
}

public func SetParticlePos(array value, int index, int precision)
{
	if (!precision) precision = 1;
	precision = CHAIN_Precision / precision;

	GetParticle(index).pos_cur = Vec_Mul(value, precision);
}

private func DrawParticleObject(int index)
{
	var obj = particle_objects[index];
	
	if (!obj)
	{
		obj = CreateObject(Rock);
		obj->SetCategory(C4D_StaticBack);
		obj->SetObjDrawTransform(300, 0, 0, 0, 300);
		obj->SetClrModulation(RGB(255, 0, 0));
		
		particle_objects[index] = obj;
	}
	
	obj->SetPosition(GetParticlePos(index)[0] / CHAIN_Precision, GetParticlePos(index)[1] / CHAIN_Precision);
	obj->SetYDir();
	obj->SetXDir();
}

private func UpdateGravity()
{
	for (var particle in GetParticles())
	{
		ParticleGravity(particle);
	}
}

private func UpdateLines()
{
	for (var i = 0; i < GetLength(GetParticles()); i++)
	{
		DrawParticleObject(i);
	}
}

private func LandscapeTestArray()
{
	return [[0, 1], [1, 0], [1, 1],
	        [0, 2], [1, 2], [2, 0],
	        [2, 1], [2, 2], [0, 3],
	        [1, 3], [2, 3], [3, 0],
	        [3, 1], [3, 2], [0, 4],
	        [1, 4], [2, 4], [3, 3],
	        [4, 0], [4, 1], [4, 2],
	        [0, 5], [1, 5], [2, 5],
	        [3, 4], [3, 5], [4, 3],
	        [4, 4], [5, 0], [5, 1],
	        [5, 2], [5, 3], [0, 6],
	        [1, 6], [2, 6], [3, 6],
	        [4, 5], [5, 4], [6, 0],
	        [6, 1], [6, 2], [6, 3]/*,
	        [0, 7], [1, 7], [2, 7],
	        [3, 7], [4, 6], [5, 5],
	        [5, 6], [6, 4], [6, 5],
	        [7, 0], [7, 1], [7, 2],
	        [7, 3], [0, 8], [1, 8],
	        [2, 8], [3, 8], [4, 7],
	        [4, 8], [5, 7], [6, 6],
	        [7, 4], [7, 5], [8, 0],
	        [8, 1], [8, 2], [8, 3],
	        [8, 4], [0, 9], [1, 9],
	        [2, 9], [3, 9], [4, 9],
	        [5, 8], [6, 7], [7, 6],
	        [7, 7], [8, 5], [9, 0],
	        [9, 1], [9, 2], [9, 3],
	        [9, 4]*/];

}