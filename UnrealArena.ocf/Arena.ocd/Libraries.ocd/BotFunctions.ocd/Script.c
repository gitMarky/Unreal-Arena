
func UTBotAIDoubleJump()
{
	if (GetProcedure() == "FLIGHT")
	{
		this->~ControlUp();
		return true;
	}
	return false;
}

func UTBotAIDodgeJump()
{
	if (!UTBotAIDoubleJump())
		Jump();
}


func UTBotAIFindDodgeBullet()
{
	var bullets = FindObjects(Find_InRect(-180, -60, 360, 120), Find_Func("IsProjectile"));

	for (var bullet in bullets) 
	{
		// TODO if (this->~CheckEnemy(bullet, this))
			return bullet;
	}

	return nil;
}



func UTBotAIFindEnemy(int radius)
{
	var enemies = UTBotAIFindEnemies(radius, false);

	for (var enemy in enemies) 
	{
		return enemy;
	}

	return nil;
}


public func UTBotAIFindEnemies(int radius, bool disregardLineOfSight)
{
	var targets = [];

	var x = GetX();
	var y = GetY();

	var candidates = FindObjects(	Find_Distance(radius),
									Find_Exclude(this),
									Find_NoContainer(),
									Find_Or( Find_Func("IsProjectileTarget", nil, this),
											 Find_OCF(OCF_Alive)),
									Sort_Distance());

	for (var target in candidates)
	{
		var target_x = target->GetX();
		var target_y = target->GetY();

		// TODO
//		if(!this->CheckEnemy(target, this, true))
//		{
//			continue;
//		}

		// Path has to be free
		if ((disregardLineOfSight || PathFree(x, y, target_x, target_y)) && target->CheckVisibility(GetOwner()))
		{
			PushBack(targets, target);
		}
	}

	return targets;
}


func UTBotAIAdjustDir(object target)
{
	if (GetX() < target->GetX())
	{
		SetDir(DIR_Right);
	}
	else
	{
		SetDir(DIR_Left);
	}
}


func UTBotAICalcAimAngle(object obj, object gun, int xmod, int ymod)
{
	var angle, skill = 22 - BotSkill(-1);

	// fliegt gerade oder ist hitscan?
	if (!(gun->~GetFMData(FM_Hitscan)))
	{
		angle = Angle(GetX(), GetY(), obj->GetX() + xmod, obj->GetY() + ymod);
		angle = angle - Random(skill + 1) + skill / 2;
		return angle;
	}

	var prec = 1000;
	var t, x1, y1, x2, y2, v1x, v1y, v2x, v2y, v1, v2, f1, f2;

	// Position des Ziels
	x1 = prec * obj->GetX();
	y1 = prec * obj->GetY();
	v1x = obj->GetXDir(prec);
	v1y = obj->GetYDir(prec);
	v1 = Distance(v1x, v1y); // Betrag der Geschwindigket

	// Objekt fällt ballistisch?
	//if(obj->GetProcedure() == "FLOAT" || obj->GetProcedure() == "SWIM")
	//	f1 = 0;
	//else
	//	f1 = 1;
	if (obj->GetProcedure() == "FLIGHT" || obj->GetProcedure() == "NONE" || obj->GetProcedure() == "TUMBLE")
		f1 = 1;
	else
		f1 = 0;

	// Position des "Projektils"
	x2 = prec * GetX();
	y2 = prec * GetY();
	v2 = gun->~GetFMData(FM_ProjSpeed);
	v2 *= prec / 10;
	// Projektil fällt ballistisch?
	if (gun->~GetFMData(FM_Ballistic))
		f2 = 1;
	else
		f2 = 0;


	// Flugzeit-Schätzung:
	// Mittelwert zwischen Direktverbindung und geschätzter ballistischer Strecke
	// geschätzte Ballistische Strecke = 	* Direktverbindung, wenn f1 = f2 = 0
	//										* Summe der Dreieckskanten im K-Sys, wenn f1 = f2 = 1
	//										* der Mittelwert davon, sonst

	var dist1 = Distance(x1, y1, x2, y2);
	var dist2 = ((2 - f1 - f2) * dist1 + (f1 + f2) * (Abs(x2 - x1) + Abs(y2 - y1))) / 2;

	t = (dist1 + dist2) / (2 * (v1 + v2));
	t = dist2 / (v1 + v2);

	v2x = -(-t * v1x + x2 - x1) / t;
	v2y = -(-t * v1y + y2 - y1 + (t * t * (f2 - f1) * prec * GetGravity()) / 1000) / t;

	angle = Angle(0, 0, v2x, v2y);
	angle = angle - Random(skill + 1) + skill / 2;
	return angle;
}


func UTBotAIAimAt(object target, int xmod, int ymod)
{
	// TODO
/*
	if (this->~IsAiming())
	{
		var angle = UTBotAICalcAimAngle(target, Contents(), xmod, ymod);

		var tx = GetX() + Sin(angle, 1000);
		var ty = GetY() - Cos(angle, 1000);

		if (!this.crosshair)
			this->~InitCrosshair();

		this->~DoMouseAiming(tx, ty);
		return angle;
	}
*/
}


func UTBotAIFire(object weapon, int mode)
{
	DebugLogBot("Firing weapon %v in mode %d", weapon, mode);
// TODO
//	if (weapon)
//	{
//		weapon->SetFireMode(mode);
//		weapon->~ControlThrow(this); //Control2Contents("ControlThrow")
//	}
}

private func DebugLogBot(string message, a, b, c, d, e)
{
	DebugLog(Format("Bot %v %s", this, Format(message, a, b, c, d, e)));
}
