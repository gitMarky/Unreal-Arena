
// You can add to the two components of the velocity vector individually with this function.
global func AddSpeed(int x_dir, int y_dir, int prec)
{
	SetXDir(GetXDir(prec) + x_dir, prec);
	SetYDir(GetYDir(prec) + y_dir, prec);
}


// Adds to an objects's speed and its direction:
// Can set either speed or angle of velocity, or both
global func AddVelocity(int angle, int speed, int precision_angle, int precision_speed)
{
	precision_speed = precision_speed ?? 10;
	precision_angle = precision_angle ?? 1;
	speed = speed ?? 0;
	angle = angle ?? 0;

	var current_x_dir = GetXDir(precision_speed);
	var current_y_dir = GetYDir(precision_speed);
		
	var x_dir = +Sin(angle, speed, precision_angle);
	var y_dir = -Cos(angle, speed, precision_angle);

	SetXDir(current_x_dir + x_dir, precision_speed);
	SetYDir(current_y_dir + y_dir, precision_speed);
	return;
}
