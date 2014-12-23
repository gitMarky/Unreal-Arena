/*-- wir wollen kein Hazard-HUD --*/
#strict 2

global func GetHUD(object obj, id theID)
{
	if (!obj)
		if (!(obj = this))
			return;
	if (!theID)
	{
		theID = 1HUD;
	}
	var owner = GetOwner(obj);
	if (owner <= -1)
		return;
	var bar = FindObjectOwner(theID, owner);
	if (!bar)
	{
		//bar = CreateObject(theID, 0,0, owner);
	}
	return bar;
}
