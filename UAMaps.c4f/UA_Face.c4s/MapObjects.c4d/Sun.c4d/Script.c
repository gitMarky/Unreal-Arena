/*-- Neues Script --*/

#strict

static moon,earth,sunrot;
local centered;

public func Initialize()
{
	MakeBackground();

	moon=CreateObject(_OAG);
	earth=CreateObject(_OAH);

	AddEffect("IntRotate", this, 50, 1, this);
}

private func MakeBackground()
{
	SetCategory(GetCategory() | C4D_Parallax | C4D_Background);

	Local() = Local(1) = 30;

}

protected func UpdateTransferZone()
  {
  if (IsNewgfx()) if (~GetCategory() & C4D_Parallax) MakeBackground();
  }

public func FxIntRotateTimer()
{
	if(!centered) {	GameCall("CenterAroundSun"); centered=1;}

	//sunrot += 1;

	sunrot +=1;

	var prec ;

	prec  = 4;

	if( sunrot >= 360 * prec ) sunrot = 0;

	var ang_earth, ang_moon;

	ang_earth = sunrot + 110  * prec;
	ang_moon = sunrot +250  * prec;

	while( ang_earth > 180  * prec ) ang_earth -= 360  * prec;
	while( ang_moon > 180  * prec ) ang_moon -= 360  * prec;

	//SetPosition( +Sin( ang_earth, 550), -Cos( ang_earth, 550), earth);
	//SetPosition( +Sin( ang_moon, 370), -Cos( ang_moon, 370), moon);

	SetPosition( GetX() +Sin( ang_earth, 424, prec), GetY() -Cos( ang_earth, 424, prec), earth);
	SetPosition( GetX() +Sin( ang_moon, 370, prec), GetY() -Cos( ang_moon, 370, prec), moon);

	SetR(Angle(GetX(),GetY(),GetX(earth),GetY(earth))-110,earth);
	SetR(Angle(GetX(),GetY(),GetX( moon),GetY( moon))-250, moon);

	earth->SetSpeed();
	moon->SetSpeed();

	Log("Earth: %d %d %d", GetX(earth), GetY(earth), sunrot);
}
