#appendto Environment_ConfigurationUA


protected func GetDefaultItemConfigurations()
{
	return [GetWeaponSet(weapon_set_ut_classic)];
}

local weapon_set_ut_classic = {
	key = "default",
	name = "Unreal Classic",
	icon = Environment_Configuration,
	weapon_chainsaw = Rock,
	weapon_enforcer = Weapon_UT99_Enforcer,
	weapon_biorifle = Weapon_UT99_BioRifle,
	weapon_asmd = Weapon_UT99_EnergyRifle,
	weapon_pulserifle = Weapon_UT99_PulseRifle,
	weapon_ripper = Weapon_UT99_Ripper,
	weapon_minigun = Weapon_UT99_Minigun,
	weapon_flakcannon = Weapon_UT99_Flak,
	weapon_rocketlauncher = Weapon_UT99_RocketLauncher,
	weapon_sniperrifle = Weapon_UT99_Sniper,
	ammo_chainsaw = nil,
	ammo_enforcer = Ammo_Pack_UT99_Enforcer,
	ammo_biorifle = Ammo_Pack_UT99_BioRifle,
	ammo_asmd = Ammo_Pack_UT99_EnergyRifle,
	ammo_pulserifle = Ammo_Pack_PulseRifle,
	ammo_ripper = Ammo_Pack_UT99_Ripper,
	ammo_minigun = Ammo_Pack_UT99_Minigun,
	ammo_flakcannon = Ammo_Pack_Flak,
	ammo_rocketlauncher = Ammo_Pack_UT99_RocketLauncher,
	ammo_sniperrifle = Ammo_Pack_UT99_Sniper,
	weapon_superweapon = Weapon_UT99_Redeemer,
	item_healthvial = Pickup_UT99_HealthVial,
	item_healthpack = Pickup_UT99_HealthPack,
	item_healthkeg = Pickup_UT99_HealthKeg,
	item_pads = Pickup_UT99_ThighPads,
	item_armor = Pickup_UT99_Armor,
	item_shield = Pickup_UT99_Shield,
	item_cloak = Bread,
	item_damage = Bread,
	item_boots = Bread,
};

private func GetWeaponSet(proplist info)
{
	// basic information
	var set = {};
	SetProperty(GAMECONFIG_Property_Keys, info.key, set);
	SetProperty(GAMECONFIG_Property_Name, info.name, set);
	SetProperty(GAMECONFIG_Property_Icon, info.icon, set);

	// empty item proplists
	var items = {};
	var weapon_chainsaw = {};
	var weapon_enforcer = {};
	var weapon_biorifle = {};
	var weapon_asmd = {};
	var weapon_pulserifle = {};
	var weapon_ripper = {};
	var weapon_minigun = {};
	var weapon_flakcannon = {};
	var weapon_rocketlauncher = {};
	var weapon_sniperrifle = {};
	var weapon_superweapon = {};
	var ammo_chainsaw = {};
	var ammo_enforcer = {};
	var ammo_biorifle = {};
	var ammo_asmd = {};
	var ammo_pulserifle = {};
	var ammo_ripper = {};
	var ammo_minigun = {};
	var ammo_flakcannon = {};
	var ammo_rocketlauncher = {};
	var ammo_sniperrifle = {};
	var item_healthvial = {};
	var item_healthpack = {};
	var item_healthkeg = {};
	var item_pads = {};
	var item_armor = {};
	var item_shield = {};
	var item_cloak = {};
	var item_damage = {};
	var item_boots = {};

	// fill slot proplists
	SetProperty(GAMECONFIG_Proplist_Def, info.weapon_chainsaw, weapon_chainsaw);
	SetProperty(GAMECONFIG_Proplist_Def, info.weapon_enforcer, weapon_enforcer);
	SetProperty(GAMECONFIG_Proplist_Def, info.weapon_biorifle, weapon_biorifle);
	SetProperty(GAMECONFIG_Proplist_Def, info.weapon_asmd, weapon_asmd);
	SetProperty(GAMECONFIG_Proplist_Def, info.weapon_pulserifle, weapon_pulserifle);
	SetProperty(GAMECONFIG_Proplist_Def, info.weapon_ripper, weapon_ripper);
	SetProperty(GAMECONFIG_Proplist_Def, info.weapon_minigun, weapon_minigun);
	SetProperty(GAMECONFIG_Proplist_Def, info.weapon_flakcannon, weapon_flakcannon);
	SetProperty(GAMECONFIG_Proplist_Def, info.weapon_rocketlauncher, weapon_rocketlauncher);
	SetProperty(GAMECONFIG_Proplist_Def, info.weapon_sniperrifle, weapon_sniperrifle);
	SetProperty(GAMECONFIG_Proplist_Def, info.weapon_superweapon, weapon_superweapon);

	SetProperty(GAMECONFIG_Proplist_Def, info.ammo_chainsaw, ammo_chainsaw);
	SetProperty(GAMECONFIG_Proplist_Def, info.ammo_enforcer, ammo_enforcer);
	SetProperty(GAMECONFIG_Proplist_Def, info.ammo_biorifle, ammo_biorifle);
	SetProperty(GAMECONFIG_Proplist_Def, info.ammo_asmd, ammo_asmd);
	SetProperty(GAMECONFIG_Proplist_Def, info.ammo_pulserifle, ammo_pulserifle);
	SetProperty(GAMECONFIG_Proplist_Def, info.ammo_ripper, ammo_ripper);
	SetProperty(GAMECONFIG_Proplist_Def, info.ammo_minigun, ammo_minigun);
	SetProperty(GAMECONFIG_Proplist_Def, info.ammo_flakcannon, ammo_flakcannon);
	SetProperty(GAMECONFIG_Proplist_Def, info.ammo_rocketlauncher, ammo_rocketlauncher);
	SetProperty(GAMECONFIG_Proplist_Def, info.ammo_sniperrifle, ammo_sniperrifle);

	SetProperty(GAMECONFIG_Proplist_Def, info.item_healthvial, item_healthvial);
    SetProperty(GAMECONFIG_Proplist_Def, info.item_healthpack, item_healthpack);
    SetProperty(GAMECONFIG_Proplist_Def, info.item_healthkeg, item_healthkeg);
    SetProperty(GAMECONFIG_Proplist_Def, info.item_pads, item_pads);
    SetProperty(GAMECONFIG_Proplist_Def, info.item_armor, item_armor);
    SetProperty(GAMECONFIG_Proplist_Def, info.item_shield, item_shield);
    SetProperty(GAMECONFIG_Proplist_Def, info.item_cloak, item_cloak);
    SetProperty(GAMECONFIG_Proplist_Def, info.item_damage, item_damage);
    SetProperty(GAMECONFIG_Proplist_Def, info.item_boots, item_boots);

	// fill item proplist
    SetProperty(SPAWN_Weapon_Chainsaw, weapon_chainsaw, items);
    SetProperty(SPAWN_Weapon_Enforcer, weapon_enforcer, items);
    SetProperty(SPAWN_Weapon_BioRifle, weapon_biorifle, items);
    SetProperty(SPAWN_Weapon_ASMD, weapon_asmd, items);
    SetProperty(SPAWN_Weapon_PulseRifle, weapon_pulserifle, items);
    SetProperty(SPAWN_Weapon_Ripper, weapon_ripper, items);
    SetProperty(SPAWN_Weapon_Minigun, weapon_minigun, items);
    SetProperty(SPAWN_Weapon_FlakCannon, weapon_flakcannon, items);
    SetProperty(SPAWN_Weapon_RocketLauncher, weapon_rocketlauncher, items);
    SetProperty(SPAWN_Weapon_SniperRifle, weapon_sniperrifle, items);

    SetProperty(SPAWN_Superweapon, weapon_superweapon, items);

    SetProperty(SPAWN_Ammo_Chainsaw, ammo_chainsaw, items);
    SetProperty(SPAWN_Ammo_Enforcer, ammo_enforcer, items);
    SetProperty(SPAWN_Ammo_BioRifle, ammo_biorifle, items);
    SetProperty(SPAWN_Ammo_ASMD, ammo_asmd, items);
    SetProperty(SPAWN_Ammo_PulseRifle, ammo_pulserifle, items);
    SetProperty(SPAWN_Ammo_Ripper, ammo_ripper, items);
    SetProperty(SPAWN_Ammo_Minigun, ammo_minigun, items);
    SetProperty(SPAWN_Ammo_FlakCannon, ammo_flakcannon, items);
    SetProperty(SPAWN_Ammo_RocketLauncher, ammo_rocketlauncher, items);
    SetProperty(SPAWN_Ammo_SniperRifle, ammo_sniperrifle, items);

    SetProperty(SPAWN_Item_HealthVial, item_healthvial, items);
    SetProperty(SPAWN_Item_HealthPack, item_healthpack, items);
    SetProperty(SPAWN_Item_HealthKeg, item_healthkeg, items);
    SetProperty(SPAWN_Item_Pads, item_pads, items);
    SetProperty(SPAWN_Item_Armor, item_armor, items);
    SetProperty(SPAWN_Item_Shield, item_shield, items);
    SetProperty(SPAWN_Item_Cloak, item_cloak, items);
    SetProperty(SPAWN_Item_UDamage, item_damage, items);
    SetProperty(SPAWN_Item_JumpBoots, item_boots, items);

	SetProperty(GAMECONFIG_Property_Items, items, set);

	return set;
}

protected func GetConfigurableItems()
{
	return [Firestone, IronBomb, Dynamite, Bread];
}
