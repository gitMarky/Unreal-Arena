#appendto Weapon_UT_Base


// Add callback to the adapter
public func SetAmmo(id ammo, int new_value)
{
	var current_value = _inherited(ammo, new_value, ...);
	UpdateHUDAmmo(Contained(), ammo, current_value);

	return current_value;
}

// Add callback to adapter
public func OnSelection(object user, proplist firemode)
{
	UpdateHUDAmmo(user, firemode.ammo_id, GetAmmo(firemode));
	_inherited(user, firemode, ...);
}


private func UpdateHUDAmmo(object user, id ammo, int current_value)
{
	user = user ?? Contained();
	var has_hud = user && user->~IsHUDAdapter();
	if (has_hud && is_selected)
	{
		user->OnSelectedWeaponAmmoChange(ammo, current_value);
	}
}
