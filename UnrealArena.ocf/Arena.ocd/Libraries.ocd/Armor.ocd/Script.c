local lib_armor;

static const UA_ARMOR_MAX = 150;
static const UA_SHIELD_MAX = 100;

func Initialize()
{
	_inherited(...);
	lib_armor = {
		Armor = 0,
		Shield = 0,
	};
}


func GetUTArmor()
{
	return BoundBy(lib_armor.Armor, 0, UA_ARMOR_MAX);
}


func DoUTArmor(int change)
{
	lib_armor.Armor = BoundBy(lib_armor.Armor + change, 0, UA_ARMOR_MAX);
}


func GetUTShield()
{
	return BoundBy(lib_armor.Shield, 0, UA_SHIELD_MAX);
}


func DoUTShield(int change)
{
	lib_armor.Shield = BoundBy(lib_armor.Shield + change, 0, UA_SHIELD_MAX);
}

