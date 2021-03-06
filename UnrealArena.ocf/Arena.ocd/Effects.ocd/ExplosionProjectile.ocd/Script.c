#include Library_Projectile

local lift = 0;

public func Launch(int angle, proplist deviation)
{
	this.is_launched = true;
	RemoveOnHit();

	if (user) SetController(user->GetController());

	var precision = 100;

	// get correct precision
	if (deviation == nil)
	{
		// everything ok
	}
	else if (deviation.precision > precision)
	{
		precision = deviation.precision;
	}
	else
	{
		deviation = ScaleDeviation(deviation, precision);
	}

	// get angle and velocity
	angle = GetLaunchAngle(angle, precision, deviation);
	velocity_x = +Sin(angle, velocity, precision);
	velocity_y = -Cos(angle, velocity, precision);

	var self = this;

	this->OnLaunch();

	if (self)
	{
		this->OnLaunched();
	}
}

func Lift(int value)
{
	lift = value;
	return this;
}

func CorpsePhysicsElasticityConstant(){ return 1000;}

func GetYDir(int precision)
{
	return _inherited(precision) + precision * lift / 10;
}
