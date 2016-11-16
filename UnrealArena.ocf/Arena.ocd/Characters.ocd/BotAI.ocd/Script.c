#include AI
#include Library_MacroCommand


func AddAI(object clonk)
{
	var fx = AI->AddAI(clonk);
	if (fx)
	{
		clonk.ExecuteAI = BotAI.Execute;
		fx.ai = BotAI;
		fx.ignore_allies = true;
	}
	return fx;
}


func Execute(proplist fx, int time)
{
	// Do nothing!
	return;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
//
// legacy code below: App-ClonkHazAggro.c


/*-- Importiert ben�tigtes Zeug aus dem Hazardclonk --*/

/* KI-Kampf */

// Setzt einen bestimmten Aggro-Wert f�r den Clonk
// 0 - keinerlei eigenm�chtiges Handeln
// 1 - schie�en auf Feinde, ohne dabei vom aktuellen Kommando abzuweichen
// 2 - auf Feinde wird geschossen und sie werden �ber kurze Strecken verfolgt
// 3 - Position (iX, iY) bewachen und dabei nur iDist weit weglaufen (wenn Feind verfolgt werden)
// Konstanten:
static const Aggro_Nothing = 0;
static const Aggro_Shoot   = 1;
static const Aggro_Follow  = 2;
static const Aggro_Guard   = 3;

public func GetAggroLevel()
{
  var effect = GetEffect("Aggro", this);
  if(!effect) return;
  return(effect.var0);
}

// Setzt sofort das Angriffsziel
public func SetAggroTarget(object pTarget)
{
  if(GetAggroLevel() == Aggro_Nothing) return;
  GetEffect("Aggro", this).var1 = pTarget;
  return(1);
}

public func GetAggroTarget()
{
  return(GetEffect("Aggro", this).var1);
}


// Sucht nach Waffen und l�uft dorthin
public func SearchWeapon(int iAggro)
{
  // N�chstbeste Spawnpunkte abklappern
  for(var pSpawn in FindObjects(Find_ID(SPNP), Sort_Random()))
    // Da ist eine Waffe drin?
    if(pSpawn -> Contents() ->~ IsWeapon())
      // Die haben wir auch noch nicht?
      if(!FindContents(pSpawn->Contents()->GetID()))
      	// Einsammelbar?
    		if(pSpawn->CheckCollect(GetOwner(),this))
        	// Hinlaufen
        	return(SetMacroCommand(0, "MoveTo", pSpawn, 0,0,0, iAggro));
}

// Sucht nach Munition und l�uft dorthin
public func SearchAmmo(int iAggro)
{
  // N�chstbeste Spawnpunkte abklappern
  for(var pSpawn in FindObjects(Find_ID(SPNP), Sort_Random()))
    // Da ist Munition drin?
    if(pSpawn -> Contents() ->~ IsAmmo())
    	// Einsammelbar?
    	if(pSpawn->CheckCollect(GetOwner(),this))
      	// Hinlaufen (wir sind gutgl�ubig und denken, dass wir die auch brauchen)
      	return(SetMacroCommand(0, "MoveTo", pSpawn, 0,0,0, iAggro));
}
























// L�sst den Clonk von einem Wegpunkt zum anderen laufen, wobei er die jeweilige Pfadeigenschaft beachten muss
private func MoveAlongPath(object pCurrentWp, object pNextWp, int iNextStep)
{
  // Rausfinden, welchen Weg wir gehen m�ssen
  for(var i = 0, path = -1 ; i < pCurrentWp->GetPathCount() ; i++)
    if(pCurrentWp->GetPathTarget(i) == pNextWp)
    {
      path = i;
      break;
    }
  // Kein Weg? -> Fehler
  if(path < 0) return(MacroComSuccessFailed(0, Macro_PathBroken));
  // Sonderbehandlung f�r Zielwegpunkte, die nur einen Pfad von sich wegf�hren haben (da k�nnten Jumppads o.�. sein)
  ClearScheduleCall(this, "JumppadCheck"); // Zur Sicherheit
  if(GetLength(aPath) > iNextStep && pNextWp->GetPathCount() == 1)
    ScheduleCall(this, "JumppadCheck", 5, 0, pNextWp, iNextStep);
  // Jetpackfliegen
  if(pCurrentWp->GetPathJetpack(path))
    ScheduleCall(this, "StartJetpack", pCurrentWp->GetPathJetpack(path), 0, pCurrentWp->GetPathJetpackFlag(path));
  // Flag rausfinden und entsprechend agieren
  var flag = pCurrentWp->GetPathFlag(path);
  if(flag == Path_MoveTo)
  {
    // Hinlaufen
    SetCommand(this, "MoveTo", pNextWp);
    return(AddSpecialCommands(pCurrentWp, path));
  }
  if(flag == Path_Jump)
  {
    // Springen und dann laufen
    var dir = pCurrentWp->GetPathDir(path);
    if(dir < 0) dir = 0;
    SetDir(dir);
    Jump();
    SetCommand(this, "MoveTo", pNextWp);
    return(AddSpecialCommands(pCurrentWp, path));
  }
  if(flag == Path_Backflip)
  {
    // Backflippen und dann laufen
    var dir = pCurrentWp->GetPathDir(path);
    if(dir < 0) dir = 0;
    SetDir(!dir);
    var iEff = AddEffect("ControlStack", this, 110, 5, this);
		if(GetDir() == DIR_Left)
		  EffectVar(0, this, iEff) = COMD_Right;
		else
		  EffectVar(0, this, iEff) = COMD_Left;
    Jump();
    ScheduleCall(0, "JumpStart", 1, 1, true);

    SetCommand(this, "MoveTo", pNextWp);
    return(AddSpecialCommands(pCurrentWp, path));
  }
  if(flag == Path_Lift)
  {
    // Wir suchen den Lift und warten oder betreten ihn
    LiftControl(0, ObjectNumber(pCurrentWp), ObjectNumber(pNextWp));
    return(AddSpecialCommands(pCurrentWp, path));
  }
  // Unbekanntes flag, MoveTo versuchen
  SetCommand(this, "MoveTo", pNextWp);
  return(AddSpecialCommands(pCurrentWp, path));
}

/* Sonderfunktionen */

protected func JumppadCheck(object pTargetWp, int iNextStep)
{
  // Pfad hat sich irgendwie ge�ndert
  if(GetLength(aPath) <= iNextStep) return;
  var pNextWp = aPath[iNextStep];
  // Feststellen, ob pTargetWp immernoch unser Bewegungsziel ist
  if(aPath[iNextStep-1] != pTargetWp)
    return;
  // Schauen, ob wir n�her an pNextWp dran sind
  if(ObjectDistance(pTargetWp) > ObjectDistance(pNextWp))
    // Wegpunkt wird �bersprungen
    return(MacroComMoveToStep(0, iNextStep));
  // Weiterlaufen
  ScheduleCall(this, "JumppadCheck", 5, 0, pTargetWp, iNextStep);
}

protected func ClimbLadder()
{
  if(!GetMacroCommand() && !GetCommand()) return;
  // Testen, ob wir noch klettern
  if(GetAction() != "ScaleLadder") return;
  // Feststellen, ob nach oben oder unten
  var targetx = GetCommand(this, 2);
  var targety = GetCommand(this, 3);

  /*
  if(GetActTime() > 5) {
    if(targetx < GetX() && GetDir() != DIR_Right)
      ControlLadder("ControlLeft");
    if(targetx > GetX() && GetDir() != DIR_Left)
      ControlLadder("ControlRight");
  }
  */

  if(targety < GetY()) SetComDir(COMD_Up);
  if(targety > GetY()) SetComDir(COMD_Down);

  // 2do: Wir m�ssen irgendwie feststellen, ob die Leiter aus ist. :(


  // Wegpunkt oben? fr�her abspringen.
  if(Inside(GetY()-targety,5,15) && Abs(GetX()-targetx) < 30)
  {
    var comd = GetComDir();
    if(targetx < GetX())
    {
    	ControlLadder("ControlLeft");
    	if(GetDir() == DIR_Left)
    	  SetComDir(comd);
   	}
    if(targetx > GetX())
    {
    	ControlLadder("ControlRight");
    	if(GetDir() == DIR_Right)
    	  SetComDir(comd);
   	}
  }

  // Sind wir ungef�hr da?
  if(Inside(GetY(), targety-5, targety+5) && PathFree(GetX(), GetY(), targetx, targety))
  {
    // Wir versuchen mal, abzuspringen (das klappt sp�testens beim 2. Mal(theoretisch))


    //wenn Ziel weiter oben: immer nach oben abspringen
    if(GetY() > targety)
      SetComDir(COMD_Up);
    //Ziel weiter unten? Nach unten abspringen.
    else if(Abs(GetY()-targety) > 50)
      SetComDir(COMD_Down);
    //Ziel weiter weg? Backflip! :D
    else if(Abs(GetX()-targetx) > 80)
      SetComDir(COMD_Stop);

    var comd = GetComDir();
    if(targetx < GetX())
    {
    	ControlLadder("ControlLeft");
    	if(GetDir() == DIR_Left)
    	  SetComDir(comd);
   	}
    if(targetx > GetX())
    {
    	ControlLadder("ControlRight");
    	if(GetDir() == DIR_Right)
    	  SetComDir(comd);
   	}
  }
  // Weiter checken
  ScheduleCall(this, "ClimbLadder", 10);
}

protected func StartJetpack(int iDir)
{
  // Kein Jetpack mehr? oO
  var jetpack = HasJetpack();
  if(!jetpack) return;
  // Richtung z�nden
  if(iDir == Jetpack_Left) // Links
  {
    SetDir(DIR_Left);
    jetpack->ControlLeftDouble(this);
  }
  if(iDir == Jetpack_UpLeft) // Oben links
  {
    SetDir(DIR_Left);
    jetpack->ControlUpDouble(this);
  }
  if(iDir == Jetpack_UpRight) // Oben rechts
  {
    SetDir(DIR_Right);
    jetpack->ControlUpDouble(this);
  }
  if(iDir == Jetpack_Right) // Rechts
  {
    SetDir(DIR_Right);
    jetpack->ControlRightDouble(this);
  }
}

protected func LiftControl(object dummy, int pCurrentWp, int pNextWp)
{
  // F�sst den Lift schon an?
  if(GetAction() == "Push")
  {
    var lift = GetActionTarget();
    if(!lift) return;
    // Nah genug am Ziel? Absteigen
    if(ObjectDistance(Object(pNextWp)) <= 50)
      return(AddCommand(this, "MoveTo", Object(pNextWp)));
    // Liftplatten befehligen
    lift->~ControlCommand("MoveTo",0, Object(pNextWp)->GetX());
    // Warten
    AddCommand(this, "Call", this, pCurrentWp, pNextWp, 0,0, "LiftControl");
    AddCommand(this, "Wait", 0,0,0,0,0, 15);
    return(1);
  }
  // Liftplatte suchen
  var x1 = Object(pCurrentWp)->GetX();
  var x2 = Object(pNextWp)->GetX();
  if(x1 > x2) { x1 = x2; x2 = Object(pCurrentWp)->GetX(); }
  var lift = FindObject2(Find_Func("IsLift"), Find_InRect(AbsX(x1), -25, x2-x1, 50));
  // Kein Lift? -> Fehlschlag
  if(!lift) return(MacroComSuccessFailed(0, Macro_PathBroken));
  // Lift nah genug? -> Einsteigen
  if(ObjectDistance(lift) <= 50)
  {
    AddCommand(this, "Call", this, pCurrentWp, pNextWp, 0,0, "LiftControl");
    AddCommand(this, "Grab", lift);
    return(1);
  }
  // Warten
  AddCommand(this, "Call", this, pCurrentWp, pNextWp, 0,0, "LiftControl");

  AddCommand(this, "Wait", 0,0,0,0,0, 15);
  return(1);
}

// Alles was der Wegpunkt als ArriveCommand hat
protected func AddSpecialCommands(object pCurrentWp, int path)
{
  if(!(pCurrentWp->GetArriveCommandCount(path))) return;
  for(var i= pCurrentWp->GetArriveCommandCount(path) - 1,command ; i > -1 ; i--)
    AddCommand(this, pCurrentWp->GetArriveCommand(path, i, 0, 0, this),
                       pCurrentWp->GetArriveCommand(path, i, 1, 0, this),
                       pCurrentWp->GetArriveCommand(path, i, 2, 0, this),
                       pCurrentWp->GetArriveCommand(path, i, 3, 0, this),
                       pCurrentWp->GetArriveCommand(path, i, 4, 0, this), 0,
                       pCurrentWp->GetArriveCommand(path, i, 5, 0, this));
}


// n�chsten Wegpunkt von einer Position aus suchen (geht nur, wenn PathFree!)
private func FindWaypoint(int iX, int iY)
{
  var wp;
  while(wp = FindObject(WAYP, AbsX(iX), AbsY(iY), -1, -1, 0,0,0,0, wp)) {
    if(PathFree(iX, iY, wp->GetX(), wp->GetY()) || GBackSolid(wp->GetX(), wp->GetY()))
      return(wp);
  }
  // Nix gefunden
  return;
}



///////////////////////////////////////////////////////////////////////////////////////////////////
//
// legacy code below: App-ClonkAI.C

/*-- Neues Script --*/


//global func BotSkill( i ){ if( i==-1) return(ObjectCount(_MGD));	return(Min(i,ObjectCount(_MGD)));}
//global func BotSkill( i ){ if( i==-1) return(15);	return(Min(i,15));}
global func BotSkill( i ){ if( i==-1) return(20);	return(Min(i,20));}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// �berladenes Hazard-Zeug
// die Wegfindung kann man z.B. auch f�r das normale Hazard verwenden


// Alles was der Wegpunkt als ArriveCommand hat
/*protected func AddSpecialCommands(object pCurrentWp, int path)
{
  if(!(pCurrentWp->GetArriveCommandCount(path))) return;
  for(var i= pCurrentWp->GetArriveCommandCount(path) - 1,command ; i > -1 ; i--)
    AddCommand(this(), pCurrentWp->GetArriveCommand(path, i),
                       pCurrentWp->GetArriveCommand(path, i, 1),
                       pCurrentWp->GetArriveCommand(path, i, 2),
                       pCurrentWp->GetArriveCommand(path, i, 3),
                       pCurrentWp->GetArriveCommand(path, i, 4), 0,
                       pCurrentWp->GetArriveCommand(path, i, 5));
}*/


// Hier wurde nur der Timer ge�ndert
public func SetAggroLevel(int iLevel, int iDist, int iX, int iY, string text)
{
	//	if(GetOwner() < -1) Message("@SetAggroLevel: %d: %s", this, iLevel, text);
	// > 3
	if(iLevel > 3) return;
	// Wir kommen von > 2
	var target;
	if(GetAggroLevel() >= 2 && iLevel < 2)
		if(target = GetEffect("Aggro", this).var1)
			if(GetMacroCommand(1, 1) == target)
			{
				FinishMacroCommand(1,0,1);
				FinishMacroCommand(1);
			}
	// 0
	if(!iLevel) return(RemoveEffect("Aggro", this));
	// 1, 2 und 3
	var effect = GetEffect("Aggro", this);
	if(!effect) effect = AddEffect("Aggro", this, 50, 20, this); // Timer war 10;
	effect.var0 = iLevel;
	// Parameter
	// iDist, Default = 500
	if(!iDist)
		iDist = 500;
	effect.var2 = iDist;
	// iX und iY, Default = GetX() & GetY()
	if(!iX && !iY && iLevel == 3)
	{
		iX = GetX();
		iY = GetY();
	}
	effect.var3 = iX;
	effect.var4 = iY;
	return 1;
}



public func FxAggroTimer(object pTarget, int no )
{
	// Doppelsprung ausf�hren, wenn sinnvoll
	if( WildcardMatch( GetAction(),"Jump*")) if(GBackSolid(-20+GetDir()*40))
		ControlUp();

	// Je schlechter der Botskill, desto �fter machen wir hier Pause
	if(Random(8-BotSkill(7))) return;

	// Kugeln ausweichen
	UTBotAIDodgeBullets();

	var target = EffectVar(1,this,no);

	if(target && !PathFree(GetX(), GetY(), target->GetX(), target->GetY()))
		target = 0;

		// Wir haben ein Ziel?
	if(target)
	{
		EffectCall(this, no, "Fire");
		// ein Ausweichsprung
		if(ObjectDistance( EffectVar(1,this,no) ) < 25 && CheckEnemy(EffectVar(1,this,no), this, false)) UTBotAIDodgeJump();
		return;
	}
	else
	{
		// Waffe soll zu schie�en aufh�ren
		if(Contents()) Contents()->~StopAutoFire();
	}

	var weaprange = 400;
	if(Contents())
		weaprange = Contents()->~GetBotData(BOT_Range);

	target = UTBotAIFindEnemy(this,Max(400,weaprange));//GetTarget(90*dir, 160);

	if(!target)
	{
		// Nichts gefunden :(
		// -> Bescheid geben!
		if(EffectVar(99, this, no))
		{
			if(Contained())
				Contained()->~HandleAggroFinished(this);
			else if(IsRiding())
				GetActionTarget()->~HandleAggroFinished(this);

			EffectVar(99, this, no);
		}
		// -> Waffen durchchecken
		//CheckIdleWeapon();
		return;
	}
	// Super
	EffectVar(1, this, no) = target;
	EffectVar(99,this, no) = true; // wir haben ein Ziel \o/}
}

public func FxAggroFire(object pTarget, int no)
{
	// Zusatzhack: BR-Bombe!
	if(GetID(Contents()) == GBRB)
		// Nichts tun :C
		return;
	// Nichts tun, wenn gerade verhindert
	if(!ReadyToFire()) return;
	var y = EffectVar(4, this(), no);
	var x = EffectVar(3, this(), no);
	var dist = EffectVar(2, this(), no);
	var target = EffectVar(1, this(), no);
	var level = EffectVar(0, this(), no);
	var pathfree = true;
	var weaprange = 0;

	if(Contents())
		weaprange = Contents()->~GetBotData(BOT_Range);


	// Fahrzeugsteuerung
	if(Contained())
	{
		if(Contained()->~HandleAggro(this(), level, target, dist, x, y))
			return(1);
		else
			return(AddCommand(this(), "Exit", 0,0,0,0,0,0,0, C4CMD_SilentSub));
	}

	// Zu weit von der Wachposition entfernt?
	/*if(level == 3)
	{
		if(Distance(GetX(), GetY(), x, y) > dist)
		{
			if(GetMacroCommand(1, 1) == target)
			{
				FinishMacroCommand(1,0,1);
				FinishMacroCommand(1);
			}
			AddMacroCommand(0, "MoveTo", 0, x,y, 0, level);
			EffectVar(1, this(), no) = 0;
		//			Message("@Returning to guarded position", this());
			return;
		}
	}

	var maxdist = dist;
	if(!PathFree(GetX(), GetY(), target->GetX(), target->GetY()))
	{
		if(level == 1) maxdist = 0;
		if(level >= 2) maxdist = dist/2;
		pathfree = false;
	}
	*/

	// Ziel irgendwie weg?
	// (Pathfree wurde schon gecheckt)
	if(!CheckTarget(target,this,Max(dist,weaprange),0,0,true))
	{
			EffectVar(1, this(), no) = 0;
			if(EffectVar(0, this(), no) == 2)
				ClearMacroCommands();
			// Hier zielt man eh immer!!
			//if(IsAiming())
			//	StopAiming();
			if(Contents()) Contents()->~StopAutoFire();
			return;
	}
	// Ich hab nix? �-�
	if(!Contents()) return; // Lauf, Forest, lauf!
	// Waffe in die Hand nehmen
	if(!SelectWeapon(level, target, false))
	{
			// Bei Aggro_Follow k�nnen wir von unserem Pfade weg. D.h. eine Waffe und/oder Munition muss her
			if(GetAggroLevel() == Aggro_Follow)
			{
//			Message("@Searching for weapons / ammo", this());
				// Waffen auffrischen?
				if(CustomContentsCount("IsWeapon") <= 1)
					return(SearchWeapon(Aggro_Shoot));
				// Munition auffrischen
				return(SearchAmmo(Aggro_Shoot));
			}
			// ein Balrog, ein Feind gegen den ihr nichts ausrichten k�nnt...lauft!
			return;
	}
	// Stufe 1 - nur in die grobe Richtung ballern, lieber nicht anhalten oder sowas

	// Zielen, muss auch mal sein
	// IsAiming ist er fast immer, wenn er schie�en kann, also passt das schon ;)
	// INFO: die Berechnung ist gut, hier aber noch nicht angebracht!!
	/*if((!GetCommand() && !GetMacroCommand()) || level != 1 || IsAiming())
	{
		if(pathfree && Contents()->GetBotData(BOT_Range) > 20) // Weg frei und keine Nahkampfwaffe?
		{
			var angle = UTBotAICalcAimAngle( target, Contents());

				if(IsAiming())
				{

					var tx = GetX()+Sin(angle,1000);//target->GetX();
					var ty = GetY()-Cos(angle,1000);//target->GetY();

					//if(Contents()->GetBotData(BOT_Ballistic))
					//	ty -= 15;

					DoMouseAiming(tx, ty);
				}
		}
	}*/

	var pWeapon = Contents();

	 // Gut. Feuern wir bereits?
	if(pWeapon->IsRecharging() /* || pWeapon->IsShooting()*/) return;

	// Er schie�t nicht immer, wenn er kann
	var efficiency = pWeapon->~GetBotData( BOT_Rating );
	if( Random( efficiency - BotSkill(efficiency-1) )) return;

	UTBotAIAdjustDir( target );

	// Feuer!
	//if(maxdist != 300 && pathfree)
	//if(pathfree)
	//if(Contents()->~GetBotData(BOT_Range) <= ObjectDistance(target))
	//	Control2Contents("ControlThrow");
	//	Message("@My target: %s @%d/%d with level %d", this(), target->GetName(), target->GetX(), target->GetY(), level);
	// Stufe 2 - verfolgen!
	if(EffectVar(0, this(), no) >= 2)
		if(GetMacroCommand(1) != "Follow" || GetMacroCommand(1, 1) != target)
			if(GetMacroCommand(0) != "Follow" || GetMacroCommand(0,1) != target)
			{
				DebugLog("FxAggroFire - Adding Follow command","aggro");
				AddMacroCommand(0, "MoveTo", 0, GetX(),GetY(), 0, level);
				AddMacroCommand(0, "Follow", target, 0, 0, 0, level);
			}


	//if((!GetCommand() && !GetMacroCommand()) || level != 1 || IsAiming())
	//{
		//if( pWeapon->GetBotData(BOT_Range) > 20) // Weg frei und keine Nahkampfwaffe?
		//{

	UTBotAIAimAt(target);
		//}
	//}
	// Waffe benutzen
	var override = pWeapon->~GetBotData(BOT_Override);
	if( override )
	{
		//EffectCall(this, no, override, pTarget,pWeapon);
		pWeapon->~UTBotAIUse( this, target );
		return;
	}


	if( Inside( ObjectDistance( pTarget ), pWeapon->~GetBotData(BOT_RangeMin,2) , pWeapon->~GetBotData(BOT_Range,2) ) )
	{
		UTBotAIFire( pWeapon,2);
	}
	else
	{
		UTBotAIFire( pWeapon,1);
	}

	return;
}

// Wenn iLevel = 1 (Aggro_Shoot) werden keine Waffen mit FM_Aim ausgew�hlt
public func SelectWeapon(int iLevel, object pTarget, bool fFireModes)
//public func FindBestWeapon( object pTarget )
{
	var arsenal, distance, selection, i, prio;

	distance = ObjectDistance( pTarget );

	arsenal = FindObjects( Find_Container(this), Find_Func("IsWeapon") );

	for( i = 0; i < GetLength( arsenal ); i++ )
	{
		if( distance <= ( arsenal[i]->~GetBotData(BOT_Range) ))
		if(( prio < ( arsenal[i]->~GetBotData(BOT_Priority, 1) )) && ( ObjectDistance( pTarget ) >= ( arsenal[i]->~GetBotData(BOT_Priority, 1) )) )
		{
			prio = arsenal[i]->~GetBotData(BOT_Priority, 1);//BotInfoPriority();

			selection = arsenal[i];
		}

	}

	if(selection) ShiftContents( this(), 0, GetID(selection) );

	//ActionCheck();

	return( Contents() );
}
/*
// Wenn iLevel = 1 (Aggro_Shoot) werden keine Waffen mit FM_Aim ausgew�hlt
public func SelectWeapon(int iLevel, object pTarget, bool fFireModes)
{
	// Entfernung zum Ziel
	var dist = ObjectDistance(pTarget);
	// Keine Waffen in Inventar?
	if(!CustomContentsCount("IsWeapon")) return;
	// Bevorzugten Schadenstyp bestimmen
	var preftype = GetPrefDmgType(pTarget), type;
	// Alle durchgehen und passende pr�fen
	for(var i=0,obj,fav,mode,favmode ; obj = Contents(i) ; mode++)
	{
		// Nix Waffe
		if(!(obj->~IsWeapon())) { i++; mode = -1; continue; }
		// Feuermodus
		if(mode && !fFireModes) { i++; mode = -1; continue; }
		if(!(obj->GetFMData(FM_Name, mode))) { i++; mode = -1; continue; }
		if(mode == obj->GetFireMode() && mode) continue;
		// Nix gut
		// man kann immer und �berall schie�en!!
		//if(obj->GetFMData(FM_Aim, mode)>0)
		//	if(iLevel == 1 || !WildcardMatch(GetAction(), "*Walk*"))
		//		continue;
		// Keine Munition daf�r?
		if(!(obj->GetCharge()) && !GetAmmo(obj->GetFMData(FM_AmmoID, mode)))
			continue;
		// EMP nur gegen Maschinen
		if(obj->GetBotData(BOT_EMP, mode))
			if(!(pTarget->~IsMachine()))
				continue;
		// Kein Favorit bisher?
		if(!fav)
		{
			fav = obj;
			type = fav->GetBotData(BOT_DmgType, mode);
			favmode = mode;
		}
		else
		{
			// Favorit hat nicht genug Reichweite
			if(fav->GetBotData(BOT_Range, favmode) < dist)
			{
				// Neue Waffe hat mehr
				if(obj->GetBotData(BOT_Range, mode) > dist)
				{
					fav = obj;
					type = obj->GetBotData(BOT_DmgType, mode);
					favmode = mode;
				}
			}
			else if(fav->GetBotData(BOT_RangeMin, favmode) > dist)
			{
					// Neue Waffe hat mehr
					if(obj->GetBotData(BOT_RangeMin, mode) < dist)
					{
						fav = obj;
						type = obj->GetBotData(BOT_DmgType, mode);
						favmode = mode;
					}
			}
			else
			{
				// Favorit hat genug Reichweite -> nur wechseln, wenn Schadenstyp besser
				if(pTarget->~OnDmg(obj->GetBotData(BOT_DmgType, mode)) < pTarget->~OnDmg(type) &&
					// Allerdings darf die Waffe nicht zu schwach sein
					fav->GetBotData(BOT_Power, favmode)-1 <= obj->GetBotData(BOT_Power, mode) &&
					// Und das gleiche nochmal mit Priorit�t!
					fav->GetBotData(BOT_Priority, favmode)-1 <= obj->GetBotData(BOT_Priority, mode))
				{
					// Neuer Favorit
					fav = obj;
					type = fav->GetBotData(BOT_DmgType);
					favmode = mode;
				}
				else
				{
					// St�rke der neuen Waffe ist gr��er oder Favorit ist ein Langlader
					if(fav->GetBotData(BOT_Power, favmode) < obj->GetBotData(BOT_Power, mode) ||
						 (fav->GetBotData(BOT_Priority, favmode) < obj->GetBotData(BOT_Priority, mode)) ||
						 (fav->GetBotData(BOT_Power, favmode) == BOT_Power_LongLoad && (fav->IsReloading() || !(fav->GetCharge()))))
					{
						// Waffe hat keine extralange Nachladezeit
						if(obj->GetBotData(BOT_Power, mode) != BOT_Power_LongLoad)
						{
							// Neuer Favorit
							fav = obj;
							type = fav->GetBotData(BOT_DmgType);
							favmode = mode;
						}
						// Waffe sollte nicht nachladen und nicht leer sein
						else if(obj->GetCharge() != 0 && !(obj->IsReloading()))
							{
								// Neuer Favorit
								fav = obj;
								type = fav->GetBotData(BOT_DmgType);
								favmode = mode;
							}
					}
				}
			}
			// Reichweite passt
			if(fav->GetBotData(BOT_Range, favmode) >= dist)
			if(fav->GetBotData(BOT_RangeMin, favmode) <= dist)
				// Schadenstyp auch, hier geht es jetzt logisch zu ^^
				if(preftype & type)
					// St�rke auch
						if(fav->GetBotData(BOT_Power, favmode) >= BOT_Power_3)
							break;
		}
	}
	// Ausw�hlen
	if(!fav) return;
	// Feuermodus wechseln?
	if(fFireModes)
		if(favmode && favmode != fav->GetFireMode())
			fav->SetFireMode(favmode);
	if(ContentsCount() == 1) return 1;
	return(ShiftContents(0,0,fav->GetID()));
}
*/


public func GetPrefDmgType(object pTarget)
{
	var min = pTarget->~OnDmg(DMG_Projectile);
	var type = DMG_Projectile;
	if(pTarget->~OnDmg(DMG_Melee) < min)
	{
		type = DMG_Melee;
		min = pTarget->~OnDmg(type);
	}
	if(pTarget->~OnDmg(DMG_Fire) < min)
	{
		type = DMG_Fire;
		min = pTarget->~OnDmg(type);
	}
	if(pTarget->~OnDmg(DMG_Explosion) < min)
	{
		type = DMG_Explosion;
		min = pTarget->~OnDmg(type);
	}
	if(pTarget->~OnDmg(DMG_Energy) < min)
	{
		type = DMG_Energy;
		min = pTarget->~OnDmg(type);
	}
	if(pTarget->~OnDmg(DMG_Bio) < min)
		type = DMG_Bio;

	return(type);
}





// Das ganze Hazard-Ding ist ja mal ziemlich Banane gewesen, mal sehen, ob ich hier einen guten
// Algorithmus einbauen kann. Wir machen das jetzt mit Dijkstra - Floyd-Warshall kann zwar negative
// Kantengewichte und so, aber das kommt hier eh nicht vor, au�erdem m�sste man da erstmal eine
// Matrix aufstellen.

// gibt Pfad von Wegpunkt pStart bis pEnd als Array aus. An Stelle 0 ist pStart.
// wenn FindPath = 0, hat er keinen Pfad gefunden.
public func FindPath(object pStart, object pEnd, bool fJetpack)
{
	var pCurrent = pStart;

	var aNodes = []; // das Array, in welchem die Wegpunkte gespeichert werden
	var aDistance = []; // dieses Array gibt an, wie weit ich zum Wegpunkt laufen muss
						 // k�nnte ausschlie�lich mit ObjectDistance() gemacht werden
						 // stattdessen erlauben wir auch, dass Teleporter oder Jumppads
						 // eine geringere "Distanz" haben, weil sie den Clonk schneller
						 // ans Ziel bringen
	var aPrev = [];		 // die Vorg�nger, daraus bauen wir einen Pfad zusammen
	var aPath = [];		 // aNodes hilft nur f�r die Nachbarschaft, aPath ist der gew�hlte Pfad
	var aSet = [];		 // Suchmenge

	var jetp = 0;
	var ammoload = DefinitionCall(JTPK,"GetFMData",FM_AmmoLoad);

	// Initialisierung: Startpunkt in die Knotenmenge, Label ist 0
	aNodes = FindObjects(Find_ID(WAYP));
	aSet = aNodes;
	for(var i=0; i<GetLength(aNodes); i++)
		aDistance[i]=-1;
	aDistance[GetArrayItemPosition(pCurrent,aNodes)] = 0;

	// jetzt der eigentliche Algorithmus
	var bEnd = false;
	while(GetLength(aSet)&&(!bEnd))
	{

		var aBest = [], pBest, pNode, iBest = -1;

		// besten Knoten ausw�hlen
		for( pNode in aSet)
		{
			var dist = aDistance[GetArrayItemPosition(pNode,aNodes)];
			//var dist = aDistance[i];
			if(dist < 0) continue;
			if(iBest == -1 || dist < iBest )
			{
				iBest = dist;
				DebugLog("Least Distance = %d (Obj %d)","dijkstra",iBest,ObjectNumber(aSet[i]));
			}
		}
		for( pNode in aSet )
		{
			if( aDistance[GetArrayItemPosition(pNode,aNodes)] == iBest )
					PushBack(pNode,aBest);
		}
		DebugLog("Best Nodes: %v","dijkstra",aBest);
		pCurrent = aBest[Random(GetLength(aBest))];

		if(!pCurrent) break;

		DropArrayItem(pCurrent, aSet);

		var pNext = 0;
		var pathcount = (pCurrent->WAYP->GetPathCount());
		var iCurrent = GetArrayItemPosition(pCurrent,aNodes);

		// alle Nachbarknoten des besten Knotens nach k�rzeren Wegen absuchen
		for(var i=0; i<pathcount; ++i)
		{
			pNext = (pCurrent->WAYP->GetPathTarget(i));
			var iNext = GetArrayItemPosition(pNext,aNodes);

			if(!Check4Jetpack(pCurrent,i,fJetpack,jetp,ammoload))
			{
				DebugLog("   Checking Path %d - Waypoint %d: Needs Jetpack","dijkstra",i,ObjectNumber(pNext));
				continue;
			}


			// jetzt die Distanz updaten
			var distnew = aDistance[iCurrent] + pCurrent->~GetPathLengthWP(i);
			DebugLog("   Checking Path %d - Waypoint %d: new %d, old %d","dijkstra",i,ObjectNumber(pNext), distnew, aDistance[iNext]);

			if( aDistance[iNext] < 0 ||
			    aDistance[iNext] > distnew)
			{
				aDistance[iNext] = distnew;
				aPrev[iNext] = pCurrent;
			}

			if(pNext == pEnd)
			{
				DebugLog("Found Endpoint","dijkstra");
				bEnd = true;
				break;
			}
		}
	}

	 // er findet keinen Weg dorthin, leider
	if(!bEnd) return false;

	// Jetzt den Weg aufbauen
	aPath=[pEnd];
	pCurrent = pEnd;
	while( pCurrent = aPrev[GetArrayItemPosition(pCurrent,aNodes)])
	{
		PushFront(pCurrent,aPath);
	}

	return aPath;
}


// Bots haben nie geschossen. Komischerweise bringt das includieren dieser Funktion,
// dass sie schie�en. Komisch deshalb, weil sie ja schon im Script enthalten ist.
/*public func GetTarget(int iAngle, int iMaxAngle, int iRange)
{
	// Parameter zurechtstutzen
	while(iAngle<0) iAngle+=360;
	while(iAngle>360) iAngle-=360;
	if(!iMaxAngle) iMaxAngle = 0;
	if(!iRange) iRange = 350;

	// Landschaft durchsuchen
	// es wird nur auf Lebewesen gezielt
	var targets = FindTargets(this, iRange, iMaxAngle, iAngle, false);
	var target;

	// n�chstes Ziel finden...
	for(var t in targets)
	{
		if(!target)
		{
			target = t;
			continue;
		}
		if(ObjectDistance(t) < ObjectDistance(target))
		{
			target = t;
			break;
		}
	}

	if(target) DebugLog("Target is %s","hazard",GetName(target));
	return(target);
}*/

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Eigenes Zeug, braucht man nicht f�r Hazard


public func UTBotAIDodgeBullets()
{
	if(Random(20-BotSkill(0))) return;
	if(!UTBotAIFindDodgeBullet()) return;
	DebugLog("FoundDodgeBullet","hazard");
	UTBotAIDodgeJump();
}


public func UTBotAIDodgeJump()
{
	if(GetProcedure() == "FLIGHT")
		ControlUp();
	else
		Jump();
}


public func UTBotAICalcAimAngle( object obj, object gun, int xmod, int ymod)
{
	var angle, skill = 22-BotSkill(-1);

	// fliegt gerade oder ist hitscan?
	if(!(gun->~GetFMData(FM_Hitscan)))
	//if(gun->~BotInfoInstantHit())
	{
		 angle=Angle(GetX(),GetY(),GetX(obj)+xmod,GetY(obj)+ymod);
		 angle=angle-Random(skill+1)+skill/2;
		 return angle;
	 }
	 /*
	 angle=Angle(GetX(),GetY(),GetX(obj)+GetXDir(obj)/2,GetY(obj)+GetYDir(obj)/2);
	 skill=0;//;
	 angle=angle-Random(skill+1)+skill/2;
	 */

		var prec = 1000;
		var t, x1,y1,x2,y2,v1x,v1y,v2x,v2y,v1,v2,f1,f2;

		// Position des Ziels
		x1 = prec*GetX(obj);
		y1 = prec*GetY(obj);
		v1x = GetXDir(obj,prec);
		v1y = GetYDir(obj,prec);
		v1 = Distance(v1x,v1y); // Betrag der Geschwindigket

		// Objekt f�llt ballistisch?
		//if(GetProcedure(obj) == "FLOAT" || GetProcedure(obj) == "SWIM")
		//	f1 = 0;
		//else
		//	f1 = 1;
		if(GetProcedure(obj) == "FLIGHT" || GetProcedure(obj) == "NONE" || GetProcedure(obj) == "TUMBLE")
			f1 = 1;
		else
			f1 = 0;

		// Position des "Projektils"
		x2 = prec*GetX();
		y2 = prec*GetY();
		v2 = gun->~GetFMData(FM_ProjSpeed);
		v2 *= prec/10;
		// Projektil f�llt ballistisch?
		if( gun->~GetFMData(FM_Ballistic) )
			f2 = 1;
		else
			f2 = 0;


		// Flugzeit-Sch�tzung:
		// Mittelwert zwischen Direktverbindung und gesch�tzter ballistischer Strecke
		// gesch�tzte Ballistische Strecke = 	* Direktverbindung, wenn f1 = f2 = 0
		//										* Summe der Dreieckskanten im K-Sys, wenn f1 = f2 = 1
		//										* der Mittelwert davon, sonst

		var dist1 = Distance(x1,y1,x2,y2);
		var dist2 = ( (2-f1-f2)*dist1 + (f1+f2)*(Abs(x2-x1)+Abs(y2-y1)) )/2;

		t= (dist1 + dist2)/(2*(v1+v2));
		t= dist2 /(v1+v2);
		var t2= BoundBy(t - gun->~GetFMData(FM_Ballistic),0,t);

		//local shitter;
		//if(!shitter) Log("Flugzeit t = %d, Distanz %d, %d, %d, V %d",t,dist1,dist2,(dist1+dist2)/2,v1+v2);
		//if(!shitter) Log("Flugzeit t = %d, f1 %d f2 %d, f2-f1 %d add = %d",t,f1,f2,f2-f1,(t*t*(f2-f1)*GetGravity())/100);

		v2x = -(-t*v1x +x2-x1)/t;
		v2y = -(-t*v1y +y2-y1 + (t*t*(f2-f1)*prec*GetGravity())/1000)/t;


		//if(!shitter) Log("Flugzeit t = %d, vx %d vy %d",t,v2x,v2y);

		angle = Angle(0,0,v2x,v2y);
		//shitter = 1;
		angle=angle-Random(skill+1)+skill/2;
		return angle;
}


public func UTBotAIFindDodgeBullet()
{
	var obj;

	var bullets = FindObjects( Find_InRect(-180,-60,360,120), Find_Func( "IsBullet" ) );

	for( obj in bullets )
	{
		if(CheckEnemy(obj,this)) return obj;
		//if(GameCall("TeamPlay")) if(ObjectCall((obj->~BulletCounter()),"GetTeam")!=GetTeam()) return(obj);
		//if(GameCall("TeamPlay")) if(( GetPlayerTeam(GetOwner(obj->~BulletCounter())) )!=GetTeam()) return(obj);
		//if((obj->~BulletOwner())!=this()) return(obj);
	}

	return(0);
}

public func UTBotAIFindEnemy( object pExclude, int iRadius)
{
	var obj;

	var enemy = UTBotAIFindEnemies(pExclude,iRadius,false);

	for( obj in enemy )
	{
		return obj;
	}

	return 0;
}

public func UTBotAIFire( object pWeapon, int iMode)
{
	if(!pWeapon) return;
	pWeapon->SetFireMode(iMode);
	pWeapon->~ControlThrow(this); //Control2Contents("ControlThrow")
}

public func UTBotAIAdjustDir( object target )
{
	if(GetX() < target->GetX())
	{
			SetDir(DIR_Right);
	}
	else
	{
			SetDir(DIR_Left);
	}
}

public func UTBotAIAimAt( object target, int xmod, int ymod )
{
	if(IsAiming())
	{
		var angle = UTBotAICalcAimAngle( target, Contents(), xmod, ymod);

		var tx = GetX()+Sin(angle,1000);//target->GetX();
		var ty = GetY()-Cos(angle,1000);//target->GetY();

		if (!crosshair) InitCrosshair();

		DoMouseAiming(tx, ty);
		return angle;
	}
}
