#appendto Library_HUDAdapter


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


private func OnSelectedWeaponAmmoChange(id ammo, int new_value)
{
	if (HUDcontroller)
		HUDcontroller->~OnSelectedWeaponAmmoChange(ammo, new_value);
	return _inherited(ammo, new_value, ...);
}
