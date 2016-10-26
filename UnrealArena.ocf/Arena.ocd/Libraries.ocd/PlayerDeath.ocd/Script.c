
/**
 Ejects the currently selected weapon, removes the rest of the inventory.
 
 @par projectile The projectile that hit the user.
 */
func DeathThrowWeapon(object projectile)
{
	var weapon = Contents();
	if (weapon)
	{
		weapon->Exit();
		var xdir = -Random(10) + GetXDir();
		var ydir = -Random(10) + GetYDir();
		var rdir = RandomX(-5, 5);
		if (projectile)
		{
		 	xdir += projectile->GetXDir() / 3;
		 	ydir += projectile->GetYDir() / 3;
		 	rdir += (projectile->GetXDir() + projectile->GetYDir()) / 10;
		}
		xdir /= 2;
		ydir /= 2;

		weapon->SetSpeed(xdir, ydir);
		weapon->SetRDir(rdir);
		weapon->SetCategory(C4D_Vehicle); // the weapon should not hurt objects
		//TODO: this should not be necessary Schedule("Disintegrate(0,20,50,-10)", 100, 0, weapon);
	}

	var contents = FindObjects(Find_Container(this));
	for (var item in contents) item->RemoveObject();
}
