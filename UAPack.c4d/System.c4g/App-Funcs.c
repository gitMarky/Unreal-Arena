#strict 2

global func GetShooter(object weapon, bool bID)
{

	var shooter;

	if(!weapon) return;

	// Waffe?
	if(weapon->~IsWeapon())
	{
		if(bID) return GetID(weapon);
		shooter = weapon->GetUser();
		// Schütze ist irgendwo drin
		if(Contained(shooter))
			shooter = Contained(shooter);
		//oder fässt was an
		if(shooter)
			if(shooter->GetActionTarget())
				shooter = GetActionTarget();
	}
	else if(weapon->~IsBullet())
	{
		shooter = LocalN("shooter",weapon);
		if(bID && LocalN("shooter_weapon",weapon)) return LocalN("shooter_weapon",weapon);
	}

	// noch immer nichst gefunden?
	if(!shooter)
		// dann einfach weapon nehmen.
		shooter = weapon;

	if(bID) return GetID(weapon);
	return shooter;
}

global func PlaceDeco(id type, int x, int y, int con, int rot, int color)
{
	var deco = CreateObject(type, 0, 0, -1);

	if (con) deco->SetCon(con);
	if (rot) deco->SetR(rot);
	if (color) deco->SetClrModulation(color);

	deco->SetPosition(x,y);

	return deco;
}

global func PlaceFloodLight(int x, int y, int r, int color)
{
	var obj = PlaceDeco(FLHH, x, y, 0, r);

	obj->SetCategory(C4D_StaticBack|C4D_Background);
	obj->RotateLight();
	obj->GetLight()->ChangeColor(color);

	return obj;
}
