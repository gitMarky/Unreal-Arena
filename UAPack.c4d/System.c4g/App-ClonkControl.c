#strict 2
#appendto UBOT



/* Steuerung Richtung (an Inhalt weitergeben, sonst internen Befehl ausführen) */

protected func ControlUpdate(object clonk, int comdir, bool dig, bool throw)
{
	if (Control2Grab("ControlUpdate", comdir, dig, throw)) return false;
	if (Control2Contents("ControlUpdate", comdir, dig, throw)) return true;
	if (ControlAim("ControlUpdate", comdir, dig, throw)) return true;
	if (ControlLadder("ControlUpdate", comdir, dig, throw)) return true;

	return _inherited(clonk, comdir, dig, throw);
}

protected func ControlUp()
{
	var control = "ControlUp";
	ClearMacroCommands();
	SetAggroLevel(0, 0, 0, 0, control);

	if (ControlAim(control)) return true;
	if (Control2Grab(control)) return false;
	if (Control2Contents(control)) return true;
	if (ControlLadder(control)) return true;
	if (ControlJetpack(control)) return true;
	if (ControlAgility(control)) return true;

	return _inherited();
}

protected func ControlUpDouble()
{
	var control = "ControlLUpDouble";
	ClearMacroCommands();
	SetAggroLevel(0, 0, 0, 0, control);

	if (ControlAim("ControlUp")) return true;
	if (Control2Grab(control)) return false;
	if (Control2Contents(control)) return true;
	if (ControlLadder(control)) return true;
	if (ControlJetpack(control)) return true;
	if (ControlAgility(control)) return true;

	return _inherited();
}

protected func ControlDown()
{
	var control = "ControlDown";
	ClearMacroCommands();
	SetAggroLevel(0, 0, 0, 0, control);

	if (ControlAim(control)) return true;
	if (Control2Grab(control)) return false;
	if (Control2Contents(control)) return true;
	if (ControlLadder(control)) return true;

	return _inherited();
}


protected func ControlLeft()
{
	var control = "ControlLeft";
	ClearMacroCommands();
	SetAggroLevel(0, 0, 0, 0, control);

	if (ControlAim(control)) return true;
	if (Control2Grab(control)) return false;
	if (Control2Contents(control)) return true;
	if (ControlLadder(control)) return true;
	if (ControlJetpack(control)) return true;
	if (ControlAgility(control)) return true;

	return _inherited();
}

protected func ControlRight()
{
	var control = "ControlRight";
	ClearMacroCommands();
	SetAggroLevel(0, 0, 0, 0, control);

	if (ControlAim(control)) return true;
	if (Control2Grab(control)) return false;
	if (Control2Contents(control)) return true;
	if (ControlLadder(control)) return true;
	if (ControlJetpack(control)) return true;
	if (ControlAgility(control)) return true;

	return _inherited();
}

protected func ControlLeftDouble()
{
	var control = "ControlLeftDouble";
	ClearMacroCommands();
	SetAggroLevel(0, 0, 0, 0, control);

	if (ControlAim(control)) return true;
	if (Control2Grab(control)) return false;
	if (Control2Contents(control)) return true;
	if (ControlJetpack(control)) return true;
	if (ControlAgility(control)) return true;

	return _inherited();
}

protected func ControlRightDouble()
{
	var control = "ControlRightDouble";
	ClearMacroCommands();
	SetAggroLevel(0, 0, 0, 0, control);

	if (ControlAim(control)) return true;
	if (Control2Grab(control)) return false;
	if (Control2Contents(control)) return true;
	if (ControlJetpack(control)) return true;
	if (ControlAgility(control)) return true;

	return _inherited();
}

protected func ControlThrow()
{
	ClearMacroCommands();
	SetAggroLevel(0,0,0,0,"ControlThrow");

	// Bei vorherigem Doppel-Stop nur Ablegen
	if ( GetPlrDownDouble(GetOwner()) )
	{
		AddEffect("SquatAimTimeout", this(), 1, 15, this);
		return inherited();
	}
	// Steuerung an gerittenes Objekt weitergeben
	if(IsRiding())
		if(GetActionTarget()->~ControlThrow(this))
			return true;

	if (Control2Grab("ControlThrow")) return false;
	if (Control2Contents("ControlThrow") ) return true;
	if (ControlLadder("ControlThrow") ) return true;

	return inherited();
}

/* Steuerung Inhalt */

protected func ControlSpecial()
{
	[$CtrlInventoryDesc$|Image=INVT]
	if(!Contents()) return;
	// Hardcode: BR-Bombe darf man nicht abwählen
	if(Contents()->GetID() == GBRB) return;
	// wenn wir zielen, wollen wir nur Waffen haben
	if(IsAiming() && Contents()->~IsWeapon())
	{
		// nächste Waffe suchen
		for(var i = 1; i < ContentsCount(); i++)
			if(Contents(i)->~IsWeapon())
			{
				// zur Waffe wechseln
				ShiftContents(0,0,Contents(i)->GetID(),true);
				break;
			}
	}
	else
	{
		// Inventory verschieben
		ShiftContents(0,0,0,true);
	}

//	UpdateCharge();
  }

