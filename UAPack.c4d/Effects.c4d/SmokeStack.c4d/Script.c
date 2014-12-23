#strict 2

public func Launch( int i)
{
	local timer,end,change;

	timer=i;
	end=timer/2;

	change=BoundBy(timer/18,1,timer);

	SetAction("Travel");

	local rgb1,rgb2;

	rgb1=150-Random(30);
	rgb2=50-Random(30);
}

public func Travel()
{
	timer=BoundBy(timer-change,end,timer);

	SmokeX(0,0,timer,1,0,RGBa(rgb1,rgb1,rgb1,60),RGBa(rgb2,rgb2,rgb2,160),1);

	if(timer<=end) RemoveObject();
}

public func ContactBottom(){ return RemoveObject();}
