/**
 A chain, sort of like Randrian's rope physics. The chain can break however, or tumble around freely.
 @author Marky
 @version 0.1.0
 */
 
/*
 From the particle documentation:
 	Name 	Values 	Description
	R 					0 to 255 	Red part of the color modulation.
	G 					0 to 255 	Green part of the color modulation.
	B 					0 to 255 	Blue part of the color modulation.
	Alpha 				0 to 255 	Alpha part of the color modulation.
	Size 				pixels 		Size of the particle in pixels.
	Stretch 			factor 		The vertical stretch of the particle. 1000 equals no stretch.
	Phase 				phase 		The displayed phase of the particle from the Graphics.png. The index starts at 0 and will be wrapped.
	Rotation 			0 to 360 	Rotation of the particle.
x	ForceX 				Integer 	Force in x-direction that is constantly applied to the particle's speed. Can f.e. simulate wind.
x	ForceY 				Integer 	Force in y-direction that is constantly applied to the particle's speed. Ca f.e. simulate gravity.
x	DampingX 			0 to 1000 	Damping of the particle's speed in x-direction. 1000 means no damping, 0 means instant stop.
x	DampingY 			0 to 1000 	Damping of the particle's speed in y-direction. 1000 means no damping, 0 means instant stop.
	BlitMode 			0 or 
				GFX_BLIT_Additive 	The particle's blit mode. Currently only additive blitting is supported.
	CollisionVertex 	0 to 1000 	The offset of the particle's hit point relative to its width. When set, the particle will collide with the landscape. 0 means the particle will collide with its center.
x	OnCollision 		PC_Die,
						PC_Bounce,
						PC_Stop 	Defines what happens when the particle collides with the landscape.
	Attach 				bit mask 	Defines the attachment of the particles to the calling object. Can be a combination of ATTACH_Front, ATTACH_Back, and ATTACH_MoveRelative. For example ATTACH_Front | ATTACH_MoveRelative
*/

/**
 Interface for getting a particle. Override this in your implementation!
 @return proplist nil. This should return the particle in question.
 */
public func GetParticle(int index)
{
	return nil;
}

/**
 Verlet integration step
 Moves the particles according to their old position and thus speed.
 @author Randrian
*/
private func VerletStep(proplist particle)
{
	particle.Collision = false;

	if (particle.Fixed) return;
	
	// Verlet step, get speed out of distance moved relative to the last position
	var damping = Vector2D(particle.DampingX,
						   particle.DampingY);

	// scale velocity up to the original value and calculate difference						   
	var velocity;
	
	if (particle.VelocityOverride)
	{
		particle.VelocityOverride = false;
		velocity = particle.Velocity;
	}
	else
	{
		velocity = Vec_Sub(particle.Position, particle.Origin);
	    velocity = Vector2D(1000 * velocity.x / Max(1, damping.x),
	                        1000 * velocity.y / Max(1, damping.y));
	}
 
	// apply gravity
	velocity = Vec_Add(velocity, particle.Acceleration);
	
	// update velocity
	particle.Velocity = velocity;

	// damping of current velocity
	var vel_damped = Vector2D(damping.x * velocity.x / 1000,
							  damping.y * velocity.y / 1000);

	// update position
	particle.Position = Vec_Add(particle.Position, vel_damped);
}

private func ParticleCollision(proplist particle)
{
	if (!particle.Collision) return;

	var event = particle.OnCollision[0];
	var data = particle.OnCollision[1];
	if (event == PC_Bounce()[0])
	{
		var normal = GetSurfaceVector(particle.Position.x / CHAIN_Precision - GetX(),
									  particle.Position.y / CHAIN_Precision - GetY());

		var angle_in = Vec_Angle(particle.Velocity, normal, CHAIN_Precision);

		var velocity = Vec_Rotate(particle.Velocity, 180 * CHAIN_Precision - 2 * angle_in, CHAIN_Precision); 
		    velocity = Vec_Div(Vec_Mul(velocity, data), 1000);

		particle.Velocity = velocity;
		particle.VelocityOverride = true;
	}
	else
	{
		// friction
		var velocity = Vec_Div(Vec_Mul(particle.Velocity, 1000 - particle.Friction), 1000);
		particle.Velocity = velocity;
		particle.VelocityOverride = true;
	}
}

private func ParticleLength(proplist particle)
{
	if (particle.Parent == nil) return;

	var parent = GetParticle(particle.Parent);
	if (parent == nil) return;

	// length constraints make no sense when both particles are fixed
	if (parent.Fixed && particle.Fixed) return;

	// determine constraint and cancel if it does not apply
	var constraint_function = particle.ConstraintLength;
	if (constraint_function == nil) return;
	if (constraint_function[0] != CONSTRAINT_BoundBy) return;

	// determine length
	var min_ratio = constraint_function[1];
	var max_ratio = constraint_function[2];

	var vector = Vec_Sub(particle.Position, parent.Position);

	var current_length = Vec_Length(vector);
	var desired_length = current_length;

	// determine whether a constraint has to be applied
	if (min_ratio != nil)
	{
		var min_length = min_ratio * particle.Length / 1000;
		if (current_length < min_length) desired_length = min_length;
	}
	
	if (max_ratio != nil)
	{
		var max_length = max_ratio * particle.Length / 1000;
		if (current_length > max_length) desired_length = max_length;
	}
	
	// if constraint is necessary, then apply
	if (desired_length != current_length)
	{
		var desired_vector = Vec_Div(Vec_Mul(vector, desired_length), current_length);
	
		if (parent.Collision && particle.Collision && !parent.Fixed)
		{
			// both collided... expand in both directions
			var center = Vec_Div(Vec_Add(parent.Position, particle.Position), 2);
			parent.Position = Vec_Sub(center, Vec_Div(desired_vector, 2));
			particle.Position = Vec_Add(parent.Position, desired_vector);
		}
		else if (parent.Collision && particle.Collision && !particle.Fixed)
		{
			// both collided... expand in both directions
			var center = Vec_Div(Vec_Add(parent.Position, particle.Position), 2);
			particle.Position = Vec_Add(center, Vec_Div(desired_vector, 2));
			parent.Position = Vec_Sub(particle.Position, desired_vector);
		}
		else if ((particle.Collision || particle.Fixed) && !parent.Fixed)
		{
			// last collided... expand in direction of first
			parent.Position = Vec_Sub(particle.Position, desired_vector);
		}
		else // if (parent.Collision)
		{
			// first collided.. (or none collided) expand in direction of last
			particle.Position = Vec_Add(parent.Position, desired_vector);
		}
	}
}


private func ParticleLandscape(proplist particle)
{
	var particle_x = particle.Position.x;
	var particle_y = particle.Position.y;

	// Don't touch ground
	if (GBackSolid(particle_x / CHAIN_Precision - GetX(), particle_y / CHAIN_Precision - GetY()))
	{
		particle.Collision = true;

		// Moving left?
		var xdir = -1;
		if(particle_x < particle.Origin.x)
			xdir = 1;

		var ydir = -1;
		// Moving up?
		if(particle_y < particle.Origin.y)
			ydir = 1;

		var found = 0;
		// Look for all possible places where the particle could move (from nearest to farest)
		for(var pos in LandscapeTestArray())
		{
			if (pos == nil) continue;
			
			for (var factor = CHAIN_Precision / 10; factor < CHAIN_Precision; factor += CHAIN_Precision / 10)
			{
				var search_x = particle_x + factor * xdir * pos[0];
				var search_y = particle_y + factor * ydir * pos[1];
	
				if(!GBackSolid(search_x / CHAIN_Precision - GetX(), search_y / CHAIN_Precision - GetY()))
				{
					found = true;

					// Notifier for applying friction after the constraints
					particle.Position = Vector2D(search_x, search_y);
					break;
				}
			}
			
			if (found) break;
		}

		// No possibility to move the particle out? Then reset it. The old position should be valid
		if(!found) particle.Position = particle.Origin;
	}
}

private func ParticleGravity(proplist particle)
{
	var gravity = 2 * GetGravity() * CHAIN_Precision / 100;
	if (particle.Acceleration.y < gravity) particle.Acceleration.y += gravity;
}

private func ParticleOriginBackup(proplist particle)
{
	particle.Temp = particle.Position;
}

private func ParticleOriginUpdate(proplist particle)
{
	if (particle.Temp != nil) particle.Origin = particle.Temp;
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