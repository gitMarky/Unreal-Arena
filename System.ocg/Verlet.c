
global func Verlet_Particle(int x, int y, int precision)
{
	if (!precision) precision = 1;
	precision = CHAIN_Precision / precision;

	var verlet = {
		pos_cur = Vector2D(0, 0),
		pos_old = Vector2D(0, 0),
		acc = Vector2D(0, 0),
		mass = 1,
		collision = false,
		// same as normal particles
		ForceX = 0,
		ForceY = 0,
		DampingX = 1000,
		DampingY = 1000,
		OnCollision = PC_Stop(),
	};

	if (x) verlet.pos_cur.x = verlet.pos_old.x = x * precision;
	if (y) verlet.pos_cur.y = verlet.pos_old.y = y * precision;

	return verlet;
}