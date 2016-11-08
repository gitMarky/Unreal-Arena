/**
 Changed explosion effects.
 @version 0.2
 */

//global func BlastObject(int level, int caused_by)
//{
//	var self = this;
//	if (caused_by == nil)
//		caused_by = GetController();
//
//	DoDamage(level, FX_Call_DmgBlast, caused_by);
//	if (!self) return;
//
//	if (GetAlive())
//		DoEnergy(-level, false, FX_Call_EngBlast, caused_by);
//	if (!self) return;
//
//	if (this.BlastIncinerate && GetDamage() >= this.BlastIncinerate)
//		Incinerate(level, caused_by);
//	return;
//
//	AssertObjectContext("BlastObject");
//
//	var self = this;
//
//	var explosion = CreateObject(Projectile_Explosion, RandomX(-5, 5), RandomX(-10, 10), caused_by);
//
//}

global func BlastObjects(int x, int y, int level, object container, int cause_plr, int damage_level, object layer, object prev_container)
{
	// caused by: if not specified, controller of calling object
	if (cause_plr == nil)
		if (this)
			cause_plr = GetController();
	
	// damage: if not specified this is the same as the explosion radius
	if (damage_level == nil)
		damage_level = level;
	
	// In a container?
	if (container)
	{
		BlastObjectInContainer(container, cause_plr, damage_level, layer, prev_container);
	}
	else
	{
		BlastObjectNoContainer(x, y, level, container, cause_plr, damage_level, layer, prev_container);
	}
	// Done.
	return true;
}

global func BlastObjectInContainer(object container, int cause_plr, int damage_level, object layer, object prev_container)
{
	if (container->GetObjectLayer() == layer)
	{
		container->BlastObject(damage_level, cause_plr);
		if (!container)
			return true; // Container could be removed in the meanwhile.
		for (target in FindObjects(Find_Container(container), Find_Layer(layer), Find_Exclude(prev_container)))
		{
			if (!target) continue;

			target->BlastObject(damage_level, cause_plr);
		}
	}
}


global func BlastObjectNoContainer(int x, int y, int level, object container, int cause_plr, int damage_level, object layer, object prev_container)
{
	// Coordinates are always supplied globally, convert to local coordinates.
	var l_x = x - GetX(), l_y = y - GetY();

	// Damage objects in radius.
	for (var target in FindObjects(Find_Distance(level, l_x, l_y), Find_NoContainer(), Find_Layer(layer), Find_Exclude(prev_container))) 
	{
		if (!target) continue;

		var explosion = CreateObject(Projectile_Explosion, RandomX(-5, 5), RandomX(-10, 10));

		var precision = 1000;
		var angle = Angle(GetX(precision), GetY(precision), target->GetX(precision), target->GetY(precision), precision);
		var speed = level * 2 - ObjectDistance(target, this);
		
		target->SetSpeed(Sin(angle, speed, precision), -Cos(angle, speed, precision));

		var percent = Sin(BoundBy(90 * (level - ObjectDistance(explosion, target)) / level, 0, 90), 100);
		
		explosion->Weapon(this->~GetWeaponID() ?? this);
		explosion->Shooter(this->~GetShooter() ?? this);
		explosion->DamageAmount(percent * damage_level / 100);
		explosion->Velocity(speed);
		explosion->DamageType(DMG_Explosion);
		explosion->HitScan();
		explosion->Launch(angle); // does not actually launch it, just does the configuration
		explosion->HitObject(target, true);
		
		// somehow not removed
		if (explosion) explosion->RemoveObject();
	}
}
