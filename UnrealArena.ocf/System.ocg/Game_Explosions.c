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
//	var help = CreateObject(Projectile_Explosion, RandomX(-5, 5), RandomX(-10, 10), caused_by);
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
		for (obj in FindObjects(Find_Container(container), Find_Layer(layer), Find_Exclude(prev_container)))
			if (obj)
				obj->BlastObject(damage_level, cause_plr);
	}

}


global func BlastObjectNoContainer(int x, int y, int level, object container, int cause_plr, int damage_level, object layer, object prev_container)
{
	var obj;
	
	// Coordinates are always supplied globally, convert to local coordinates.
	var l_x = x - GetX(), l_y = y - GetY();
	
	// Object is outside.
	var at_rect = Find_AtRect(l_x - 5, l_y - 5, 10, 10);
	// Damage objects at point of explosion.
	for (var obj in FindObjects(at_rect, Find_NoContainer(), Find_Layer(layer), Find_Exclude(prev_container)))
		if (obj) obj->BlastObject(damage_level, cause_plr);
		
	// Damage objects in radius.
	for (var obj in FindObjects(Find_Distance(level, l_x, l_y), Find_Not(at_rect), Find_NoContainer(), Find_Layer(layer), Find_Exclude(prev_container)))
		if (obj) obj->BlastObject(damage_level / 2, cause_plr);

	
	// Perform the shockwave at the location where the top level container previously was.
	// This ensures reliable flint jumps for explosives that explode inside a crew member.
	var off_x = 0, off_y = 0;
	if (prev_container)
	{	
		var max_offset = 300;
		off_x = BoundBy(-prev_container->GetXDir(100), -max_offset, max_offset);
		off_y = BoundBy(-prev_container->GetYDir(100), -max_offset, max_offset);			
	}
	DoShockwave(x, y, level, cause_plr, layer, off_x, off_y);
}
