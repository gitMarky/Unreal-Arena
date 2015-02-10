#appendto Environment_ConfigurationUA


protected func GetDefaultItemConfigurations()
{
	return [GetWeaponSetUTClassic()];
}

private func GetWeaponSetUTClassic()
{
	// default
	var set = {};
	SetProperty(GAMECONFIG_Property_Keys, "default", set);
	SetProperty(GAMECONFIG_Property_Name, "Unreal Classic", set);
	SetProperty(GAMECONFIG_Property_Icon, Environment_Configuration, set);
	
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

	SetProperty(GAMECONFIG_Proplist_Def, Firestone, weapon_chainsaw);
	SetProperty(GAMECONFIG_Proplist_Def, Firestone, weapon_enforcer);
	SetProperty(GAMECONFIG_Proplist_Def, Firestone, weapon_biorifle);
	SetProperty(GAMECONFIG_Proplist_Def, Firestone, weapon_asmd);
	SetProperty(GAMECONFIG_Proplist_Def, Firestone, weapon_pulserifle);
	SetProperty(GAMECONFIG_Proplist_Def, Firestone, weapon_ripper);
	SetProperty(GAMECONFIG_Proplist_Def, Firestone, weapon_minigun);
	SetProperty(GAMECONFIG_Proplist_Def, Firestone, weapon_flakcannon);
	SetProperty(GAMECONFIG_Proplist_Def, Firestone, weapon_rocketlauncher);
	SetProperty(GAMECONFIG_Proplist_Def, Firestone, weapon_sniperrifle);
	
	SetProperty(GAMECONFIG_Proplist_Def, Bread, item_healthvial);
    SetProperty(GAMECONFIG_Proplist_Def, Bread, item_healthpack);
    SetProperty(GAMECONFIG_Proplist_Def, Bread, item_healthkeg);
    SetProperty(GAMECONFIG_Proplist_Def, Dynamite, item_pads);
    SetProperty(GAMECONFIG_Proplist_Def, Dynamite, item_armor);
    SetProperty(GAMECONFIG_Proplist_Def, Dynamite, item_shield);
    SetProperty(GAMECONFIG_Proplist_Def, Dynamite, item_cloak);
    SetProperty(GAMECONFIG_Proplist_Def, Dynamite, item_damage);
    SetProperty(GAMECONFIG_Proplist_Def, Dynamite, item_boots);
	
	
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