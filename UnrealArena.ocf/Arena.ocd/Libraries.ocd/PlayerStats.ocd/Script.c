local lib_stats;

static const UA_ARMOR_MAX = 150;
static const UA_SHIELD_MAX = 100;
static const UA_HEALTH_MAX = 100;
static const HEALTH_PRECISION = 1000;

func Initialize()
{
	_inherited(...);
	lib_stats = {
		Armor = 0,
		Shield = 0,
		ExtraHealth = 0,
	};
}


func GetUTArmor()
{
	return BoundBy(lib_stats.Armor, 0, UA_ARMOR_MAX);
}


func DoUTArmor(int change)
{
	lib_stats.Armor = BoundBy(lib_stats.Armor + change, 0, UA_ARMOR_MAX);
}


func GetUTShield()
{
	return BoundBy(lib_stats.Shield, 0, UA_SHIELD_MAX);
}


func DoUTShield(int change)
{
	lib_stats.Shield = BoundBy(lib_stats.Shield + change, 0, UA_SHIELD_MAX);
}


func GetUTExtraHealth()
{
	return BoundBy(lib_stats.ExtraHealth, 0, UA_HEALTH_MAX);
}


func DoUTExtraHealth(int change, int cause, int caused_by)
{
	lib_stats.ExtraHealth = BoundBy(lib_stats.ExtraHealth + change, 0, UA_HEALTH_MAX);

	// Do not use "SetMaxEnergy", because the additional energy should be added absolute, not relative!
	var max_energy = this->GetID().MaxEnergy;
	this.MaxEnergy = max_energy + HEALTH_PRECISION * lib_stats.ExtraHealth;
	if (change > 0)
	{
		this->DoEnergy(change);	
	}
}


func DoEnergy(int change, bool exact, int damage_type, int caused_by)
{
	if (change < 0)
	{
		var amount = Abs(change);
		var extra = GetUTExtraHealth();
		if (exact)
		{
			extra *= HEALTH_PRECISION;
			
			// round up
			var remainder = amount % HEALTH_PRECISION;
			if (remainder > 0)
			{
				amount += HEALTH_PRECISION - remainder;
			}
		}

		amount = Min(amount, extra); // this much can be subtracted from extra health;
		change += Min(change + amount, 0); // the remaining change that has to be handled normally
		DoUTExtraHealth(-amount);
	}

	_inherited(change, exact, damage_type, caused_by);
}
