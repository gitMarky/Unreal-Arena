#strict 2

protected func Activate(iPlr)
{
  MessageWindow(GetDesc(),iPlr);
}

public func ChooserFinished()
{
	gWeaponPos01 = ID_UT99_Chainsaw;
	gWeaponPos02 = ID_UT99_Enforcer;
	gWeaponPos03 = ID_UT99_Bio_Rifle;
	gWeaponPos04 = ID_UT99_ASMD_Rifle;
	gWeaponPos05 = ID_UT99_Pulse_Rifle;
	gWeaponPos06 = ID_UT99_Ripper_Weapon;
	gWeaponPos07 = ID_UT99_Minigun;
	gWeaponPos08 = ID_UT99_Flak_Cannon;
	gWeaponPos09 = ID_UT99_Rocket_Launcher;
	gWeaponPos10 = ID_UT99_Sniper_Rifle;
	gWeaponPos11 = 0;
	gWeaponPos12 = 0;
	gWeaponPos13 = 0;
	gWeaponPos14 = 0;
	gWeaponPos15 = 0;

	gItemPos01 = ID_Item_HealthVial;
	gItemPos02 = ID_Item_HealthPack;
	gItemPos03 = ID_Item_HealthKeg;
	gItemPos04 = ID_Item_Pads;
	gItemPos05 = ID_Item_Armor;
	gItemPos06 = ID_Item_Shield;
	gItemPos07 = ID_Item_Cloak;
	gItemPos08 = ID_Item_UDamage;
	gItemPos09 = ID_Item_JumpBoots;
	gItemPos10 = 0;

	SetAmmo();
	//static gAmmoPos01, gAmmoPos02, gAmmoPos03, gAmmoPos04, gAmmoPos05, gAmmoPos06, gAmmoPos07, gAmmoPos08, gAmmoPos09, gAmmoPos10;

}

public func SetAmmo()
{
	gAmmoPos01 = gWeaponPos01->~AmmoPackID();
	gAmmoPos02 = gWeaponPos02->~AmmoPackID();
	gAmmoPos03 = gWeaponPos03->~AmmoPackID();
	gAmmoPos04 = gWeaponPos04->~AmmoPackID();
	gAmmoPos05 = gWeaponPos05->~AmmoPackID();
	gAmmoPos06 = gWeaponPos06->~AmmoPackID();
	gAmmoPos07 = gWeaponPos07->~AmmoPackID();
	gAmmoPos08 = gWeaponPos08->~AmmoPackID();
	gAmmoPos09 = gWeaponPos09->~AmmoPackID();
	gAmmoPos10 = gWeaponPos10->~AmmoPackID();
	/*gAmmoPos11 = gWeaponPos11->~AmmoPackID();
	gAmmoPos12 = gWeaponPos12->~AmmoPackID();
	gAmmoPos13 = gWeaponPos13->~AmmoPackID();
	gAmmoPos14 = gWeaponPos14->~AmmoPackID();
	gAmmoPos15 = gWeaponPos15->~AmmoPackID();*/
}

// Kann mittels des Spielzielauswählers ausgewählt werden
public func IsWeaponSet() { return true; }
