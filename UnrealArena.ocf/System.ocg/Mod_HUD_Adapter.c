#appendto Library_HUDAdapter

// Either returns the current HUD controller or creates one.
// But only if owner is a human otherwise returns nil.
func GetHUDController()
{
	var plr = GetOwner();
	if (GetPlayerType(plr) != C4PT_User) return nil;
	if (HUDcontroller) return HUDcontroller;
	HUDcontroller = FindObject(Find_ID(GUI_Controller_UA), Find_Owner(plr));
	if (!HUDcontroller)
		HUDcontroller = CreateObject(GUI_Controller_UA, AbsX(0), AbsY(0), plr);
	return HUDcontroller;
}

protected func OnEnergyChange(int change, int cause, int caused_by)
{
	DebugLog("*** OnEnergyChange: %d %d %d", change, cause, caused_by);
	return _inherited(change, cause, caused_by, ...);
}



private func OnShieldChange(int change, int cause, int caused_by)
{
	if (HUDcontroller)
		HUDcontroller->~OnCrewShieldChange(this, change, cause, caused_by);
	return _inherited(change, cause, caused_by, ...);

}

private func OnArmorChange(int change, int cause, int caused_by)
{
	if (HUDcontroller)
		HUDcontroller->~OnCrewArmorChange(this, change, cause, caused_by);
	return _inherited(change, cause, caused_by, ...);

}
