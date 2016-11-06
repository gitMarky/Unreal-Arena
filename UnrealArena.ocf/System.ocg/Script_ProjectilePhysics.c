/**
 Handles interaction between projectiles and physical objects.
 
 This is based on a physics calculcation, but it makes a lot of assumptions and probably is not entirely correct.
 
 @title Projectile physics
 @author Marky
 */
 
global func Calc_ProjectileCollision(object body1, object body2, int elasticity)
{
	if (elasticity < 0)
	{
		FatalError(Format("Parameter elasticity must be 0 <= elasticity <= 1000, you passed: %d", elasticity));
	}
	if (body1 == nil || body2 == nil)
	{
		FatalError("The function needs two objects as arguments");
	}
	
	var precision = 1000;
	
	// initial data
	
	var m_b1 = body1->GetMass();
	var m_b2 = body2->GetMass();

	var v_x_b1 = body1->GetXDir(precision);
	var v_y_b1 = body1->GetYDir(precision);

	var v_x_b2 = body2->GetXDir(precision);
	var v_y_b2 = body2->GetYDir(precision);

	// resulting velocity

	var v_x = m_b1 * v_x_b1 + m_b2 * v_x_b2 - elasticity * m_b2 * (v_x_b1 - v_x_b2) / precision;
	var v_y = m_b1 * v_y_b1 + m_b2 * v_y_b2 - elasticity * m_b2 * (v_y_b1 - v_y_b2) / precision;
	
	var mass = m_b1 + m_b2;

	v_x /= mass; v_y /= mass;

	// resulting angular velocity
	
	var angle = Angle(0, 0, v_x_b2, v_y_b2, precision) - body1->GetR() * precision; // angle rotated as if it were a global angle
	var tangential_component = Sin(angle, Distance(0, 0, v_x, v_y), precision); 
	
	// radius of rotation: distance from bottom of body1 to center of body2
	var bottom = body1->GetDefHeight() + body1->GetDefOffset(1);
	bottom *= precision;
	
	var bottom_x = body1->GetX(precision) - Sin(body1->GetR(), bottom);
	var bottom_y = body1->GetY(precision) + Cos(body1->GetR(), bottom);
	
	var length = Distance(bottom_x, bottom_y, body2->GetX(precision), body2->GetY(precision));

	// Log("Angular velocity: bottom = %d, length = %d, angle = %d, tangent = %d", bottom, length / precision, angle, tangential_component);

	var angular_velocity = tangential_component * precision / Max(1, length);

	//Log("Body1 = %d/%d, Body2 = %d/%d", v_x_b1, v_y_b1, v_x_b2, v_y_b2);
	//Log("Resulting velocity = %d/%d %d", v_x, v_y, angular_velocity);

	return { XDir = v_x, YDir = v_y, RDir = angular_velocity, Precision = precision};
}

global func Test_Calc_ProjectileCollision()
{
	
	// test data: proplist with inputs body1, proplist with inputs body2, elasticity, expected results
	var data = [[{Mass = 1, XDir = 0, YDir = 0}, {Mass = 1, XDir = 10, YDir = 0}, 0, {XDir = 500, YDir = 0}],
	            [{Mass = 1, XDir = 0, YDir = 0}, {Mass = 1, XDir = 10, YDir = 0}, 1000, {XDir = 1000, YDir = 0}],
	            [{Mass = 1, XDir = 0, YDir = 0}, {Mass = 1, XDir = 10, YDir = 0}, 500, {XDir = 750, YDir = 0}],
	            [{Mass = 1, XDir = 0, YDir = 0}, {Mass = 1, XDir = 0, YDir = 10}, 0, {XDir = 0, YDir = 500}],
	            [{Mass = 1, XDir = 10, YDir = 0}, {Mass = 1, XDir = 0, YDir = 0}, 0, {XDir = 0, YDir = 0}],       // test elasticity for first body
	            [{Mass = 1, XDir = 0, YDir = 10}, {Mass = 1, XDir = 0, YDir = 0}, 0, {XDir = 0, YDir = 0}],       // test elasticity for first body
	            [{Mass = 2, XDir = 20, YDir = 0}, {Mass = 1, XDir = 50, YDir = 0}, 0, {XDir = 3000, YDir = 0}],
	            [{Mass = 2, XDir = 20, YDir = 0}, {Mass = 1, XDir =-100, YDir = 0}, 0, {XDir = -2000, YDir = 0, RDir = -500}],
	            [{Mass = 1, XDir = 0, YDir = 0, Y = 6}, {Mass = 1, XDir =10, YDir = 0}, 0, {XDir = 500, YDir = 0, RDir = 50}],	// test angular velocity
	            [{Mass = 1, XDir = 0, YDir = 0, R = 90}, {Mass = 1, XDir =10, YDir = 0}, 0, {XDir = 500, YDir = 0, RDir = 0}],	// test angular velocity
	            [{Mass = 1, XDir = 0, YDir = 0, R = 90, X = -6}, {Mass = 1, XDir =0, YDir = -20}, 0, {XDir = 0, YDir = -1000, RDir = -100}]	// test angular velocity
	           ];

	for (var test in data)
	{
		var dummy1 = CreateObject(Rock);
		dummy1->SetXDir(test[0].XDir);
		dummy1->SetYDir(test[0].YDir);
		dummy1->SetMass(test[0].Mass);
		dummy1->SetPosition(test[0].X, test[0].Y);
		dummy1->SetR(test[0].R);

		var dummy2 = CreateObject(Rock);
		dummy2->SetXDir(test[1].XDir);
		dummy2->SetYDir(test[1].YDir);
		dummy2->SetMass(test[1].Mass);
		dummy2->SetPosition(test[1].X, test[1].Y);
		dummy2->SetR(test[0].R);

		var result = Calc_ProjectileCollision(dummy1, dummy2, test[2]);
		
		if (test[3].RDir != nil)
		{
			Log("Result: XDir %d/%d, YDir %d/%d, RDir %d/%d", result.XDir, test[3].XDir, result.YDir, test[3].YDir, result.RDir, test[3].RDir);
		}
		else
		{
			Log("Result: XDir %d/%d, YDir %d/%d", result.XDir, test[3].XDir, result.YDir, test[3].YDir);
		}		

		if (dummy1) dummy1->RemoveObject();
		if (dummy2) dummy2->RemoveObject();
	}
}

