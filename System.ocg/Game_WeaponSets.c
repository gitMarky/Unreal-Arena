#appendto Environment_ConfigurationUA


protected func GetDefaultItemConfigurations()
{
	return [GetWeaponSet(weapon_set_ut_classic)];
}

local weapon_set_ut_classic = {
	key = "default",
	name = "Unreal Classic",
	icon = Environment_Configuration,
	weapon_chainsaw = Firestone,
	weapon_enforcer = Firestone,
	weapon_biorifle = Firestone,
	weapon_asmd = Firestone,
	weapon_pulserifle = Firestone,
	weapon_ripper = Firestone,
	weapon_minigun = Firestone,
	weapon_flakcannon = Firestone,
	weapon_rocketlauncher = Firestone,
	weapon_sniperrifle = Firestone,
	item_healthvial = Bread,
	item_healthpack = Bread,
	item_healthkeg = Bread,
	item_pads = Bread,
	item_armor = Bread,
	item_shield = Bread,
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