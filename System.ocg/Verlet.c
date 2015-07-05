
global func Verlet_Particle(int x, int y, int precision)
{
	if (!precision) precision = 1;
	precision = CHAIN_Precision / precision;

	var verlet = {
		Position = Vector2D(0, 0),
		Origin = Vector2D(0, 0),
		Velocity = Vector2D(0, 0),
		Acceleration = Vector2D(0, 0),
		Mass = 1,
		Collision = false,
		Friction = 1000,
		Fixed = true,
		ConstraintLength = CF_BoundBy(1000, 1000), // stay the same length
		ConstraintAngle = nil, // take any angle
		// same as normal particles
		ForceX = 0,
		ForceY = 0,
		DampingX = 1000,
		DampingY = 1000,
		OnCollision = PC_Stop(),
	};

	if (x) verlet.Position.x = verlet.Origin.x = x * precision;
	if (y) verlet.Position.y = verlet.Origin.y = y * precision;

	return verlet;
}