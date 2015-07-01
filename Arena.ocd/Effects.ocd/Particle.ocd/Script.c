/**
 A chain, sort of like Randrian's rope physics. The chain can break however, or tumble around freely.
 @author Marky
 @version 0.1.0
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

private func ParticleFriction(proplist particle)
{
		var newvel = Vec_Sub(particle.pos_cur, particle.pos_old);
		
		particle.pos_old = Vec_Sub(particle.pos_cur, Vec_Div(newvel, 2));
}

private func ParticleLandscape(proplist particle)
{
		var pos_cur = particle.pos_cur;
		var pos_old = particle.pos_old;
		
		var particle_x = pos_cur[0] / CHAIN_Precision;
		var particle_y = pos_cur[1] / CHAIN_Precision;

		// Don't touch ground
		if (GBackSolid(particle_x - GetX(), particle_y - GetY()))
		{
			// Moving left?
			var xdir = -1;
			if(pos_cur[0] < pos_old[0])
				xdir = 1;

			var ydir = -1;
			// Moving up?
			if(pos_cur[1] < pos_old[1])
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
					var new = [0, 0];
					if(pos[0])
						new[0] = search_x * CHAIN_Precision - xdir * CHAIN_Precision / 2 + xdir;
					else
						new[0] = pos_cur[0];

					if(pos[1])
						new[1] = search_y * CHAIN_Precision - ydir * CHAIN_Precision / 2 + ydir;
					else
						new[1] = pos_cur[1];
					// Notifier for applying friction after the constraints
					particle.collision = true;
					particle.pos_cur = new;
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
	var gravity = GetGravity() * CHAIN_Precision / 100;
	if (particle.acc[1] < gravity) particle.acc[1] += gravity;
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