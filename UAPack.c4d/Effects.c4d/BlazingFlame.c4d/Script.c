#strict 2

local master,x,y,rot,dist,ang;

public func Hover()
{
	if(!master) return RemoveObject();

	var _x,_y,r;

	r = ang + rot + GetR(master);

	_x=+Sin(r,dist);
	_y=-Cos(r,dist);

	SetXDir();
	SetYDir();

	if(GetActTime() > 30 ) DoCon(-1);

	SetPosition(GetX(master)+x,GetY(master)+y);

}

public func Fall()
{
	if(GetActTime() > 30 ) DoCon(-2);

}
public func SetUpFlame( object obj, int dam, int _x, int _y)
{
	master = obj;
	x = _x;
	y = _y;

	dist = Distance(0,0,x,y);
	ang = Angle(0,0,x,y);

	rot = -GetR( master );

	Incinerate();

	SetCon(2*dam);

	SetAction("Hover",master);
	SetObjectLayer(this);
}

public func SetMaster( object obj )
{
	master = obj;
	SetAction("Hover",master);
}

public func SetUpSpark( int dam, int _x, int _y)
{
	Incinerate();

	SetCon(2*dam);

	SetXDir(_x);
	SetYDir(_y);

	SetAction("Fall");
	SetObjectLayer(this);
}

public func Hit()
{
	if(GetAction() == "Fall") RemoveObject();
}
