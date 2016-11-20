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
