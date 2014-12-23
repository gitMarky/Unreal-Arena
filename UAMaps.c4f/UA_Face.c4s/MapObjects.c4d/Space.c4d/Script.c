/*-- Neues Script --*/

#strict 2
public func Initialize()
{
	MakeBackground();

	SetAction("Exist");

	//SetRDir(1, this, 100);

	SetPosition(LandscapeWidth()/2, LandscapeHeight()/2);

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

protected func FxIntRotateTimer(object target, int effect, int r)
{
	var xoff = 0, yoff = 0;
	var fsin=Sin(-r, 1000, 50), fcos=Cos(-r, 1000, 50);
	// set matrix values
	SetObjDrawTransform (
	    +fcos, +fsin, (1000-fcos)*xoff - fsin*yoff,
	    -fsin, +fcos, (1000-fcos)*yoff + fsin*xoff,
	    this
	);
}
