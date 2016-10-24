/* Gestorben */

protected func Death()
{
	//Schedule("RemoveObject()",1);

	if(!bNoCorpse)
		OnDeathExtended(0, DMG_Melee, this, false );

	var plr = GetOwner();

	// Sound und Meldung
	//Sound("Die");
	DeathAnnounce(plr, this, GetKiller());
		NoDeathAnnounce(); // nicht nochmal aufrufen
	// Letztes Mannschaftsmitglied func tot() { neuer Einsatz

	if(GetPlayerType(GetOwner()) == C4PT_Script)
		GameCallEx("RelaunchPlayer", plr, this, GetKiller());
	else
	{
		var gotcrew;
		for(var i; i < GetCrewCount(plr); i++)
			if(GetOCF(GetCrew(plr, i)) & OCF_Alive)
				gotcrew = true;

		if(!gotcrew)
			GameCallEx("RelaunchPlayer", plr, this, GetKiller());
	}

	if(ammobag)
		RemoveObject(ammobag);

	if(GetEffect(0,0,wpneffect))
		RemoveEffect(0,0,wpneffect);

	//FadeOut(this());
	RemoveObject();

	_inherited();
}

public func DeathThrowWeapon( pProjectile )
{
	var wep = Contents();
	if(wep)
	{
		Exit(wep);
		var xdir, ydir;
		xdir = -Random(10)+GetXDir()+GetXDir(pProjectile)/3;
		ydir = -Random(10)+GetYDir()+GetYDir(pProjectile)/3;
		xdir /= 2; ydir /= 2;
		wep->SetSpeed(xdir, ydir);
		wep->SetRDir((GetXDir(pProjectile)+GetYDir(pProjectile))/10);
		wep->SetCategory(C4D_Vehicle); // Damit es nicht die Bots umwirft
		Schedule("Disintegrate(0,20,50,-10)",100,0,wep);
	}

	if(Contents())
		while(Contents())
			RemoveObject(Contents());
}

private func DeathAnnounce(int plr, object clonk, int killplr)
{
	if(GetEffect("NoAnnounce", this)) return;

	  if(killplr == -1) return;
	  if(!clonk) return;
	  if( bDeathHeadshot )
	  {
		  bDeathHeadshot = false;
		  UA_Announcer( "an_aw_headshot", GetKiller() );
	  }

	  //Selfkill?
	  if(plr == killplr)
		  HHKS->SKMsg(plr);
	  else
		  HHKS->KTMsg(killplr, plr, clonk->~LastDamageType(),clonk->~LastDamageWeapon());


	  //Killstatistik.
	  //Zwar ungenau wenn mehrere Clonks eines Spielers im Spiel sind,
	  //aber wann ist das schon der Fall?
	  HHKS->KillStat(GetCursor(killplr),plr);

	  //_inherited(plr, clonk, killplr);
}


public func NoDeathAnnounce() // Hält 1 Frame lang, also Clonk schnell töten, dann kommt keine Nachricht
{
	AddEffect("NoAnnounce", this, 1, 1, this);
}

public func FxNoAnnounceStop(dummy, dummy2, int iReason)
{
	if(iReason == 4) return -1;
}

