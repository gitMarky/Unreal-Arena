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
 Verlet integration step
 Moves the particles according to their old position and thus speed.
 @author Randrian
*/
private func VerletStep(proplist particle, bool gravity)
{
		var temp = particle.pos_cur;

		// Verlet step, get speed out of distance moved relative to the last position
		particle.pos_cur = Vec_Add(particle.pos_cur, Vec_Sub(particle.pos_cur, particle.pos_old));

		if (gravity)
		{
			particle.pos_cur = Vec_Add(particle.pos_cur, particle.acc);
		}

		particle.pos_old = temp;
		particle.collision = false;
}

private func ParticleCollision(proplist particle)
{
	// friction
	var newvel = Vec_Sub(particle.pos_cur, particle.pos_old);	
	particle.pos_old = Vec_Sub(particle.pos_cur, Vec_Div(newvel, 2));
}

private func ParticleLandscape(proplist particle)
{
		var pos_cur = particle.pos_cur;
		var pos_old = particle.pos_old;
		
		var particle_x = pos_cur.x / CHAIN_Precision;
		var particle_y = pos_cur.y / CHAIN_Precision;

		// Don't touch ground
		if (GBackSolid(particle_x - GetX(), particle_y - GetY()))
		{
			// Moving left?
			var xdir = -1;
			if(pos_cur.x < pos_old.x)
				xdir = 1;

			var ydir = -1;
			// Moving up?
			if(pos_cur.y < pos_old.y)
				ydir = 1;

			var found = 0;
			// Look for all possible places where the particle could move (from nearest to farest)
			for(var pos in LandscapeTestArray())
			{
				if (pos == nil) continue;

				var search_x = particle_x + xdir * pos[0];
				var search_y = particle_y + ydir * pos[1];

				if(!GBackSolid(search_x - GetX(), search_y - GetY()))
				{
					// Calculate the new position (if we don't move in a direction don't overwrite the old value)
					var pos_new = Vector2D(0, 0);
					if(pos[0])
						pos_new.x = search_x * CHAIN_Precision - xdir * CHAIN_Precision / 2 + xdir;
					else
						pos_new.x = pos_cur.x;

					if(pos[1])
						pos_new.y = search_y * CHAIN_Precision - ydir * CHAIN_Precision / 2 + ydir;
					else
						pos_new.y = pos_cur.y;
					// Notifier for applying friction after the constraints
					particle.collision = true;
					particle.pos_cur = pos_new;
					found = true;
					break;
				}
			}

			// No possibility to move the particle out? Then reset it. The old position should be valid
			if(!found) particle.pos_cur = particle.pos_old;
		}
}

private func ParticleGravity(proplist particle)
{
	var gravity = 2 * GetGravity() * CHAIN_Precision / 100;
	if (particle.acc.y < gravity) particle.acc.y += gravity;
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