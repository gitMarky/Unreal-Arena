
global func VerletInfo(int x, int y, int x_acc, int y_acc, int mass, int precision)
{
	if (!precision) precision = 1;
	
	precision = CHAIN_Precision / precision;

	var verlet = {
		pos_cur = [0, 0],
		pos_old = [0, 0],
		acc = [0, 0],
		mass = 1,
		friction = false,
	};

	if (x) verlet.pos_cur[0] = verlet.pos_old[0] = x * precision;
	if (y) verlet.pos_cur[1] = verlet.pos_old[1] = y * precision;
	if (x_acc) verlet.acc[0] = x_acc * precision;
	if (y_acc) verlet.acc[1] = y_acc * precision;
	if (mass) verlet.mass = mass;
	
	return verlet;
}