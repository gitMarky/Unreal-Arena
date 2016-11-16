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


/*-- Importiert benötigtes Zeug aus dem Hazardclonk --*/

/* KI-Kampf */

// Setzt einen bestimmten Aggro-Wert für den Clonk
// 0 - keinerlei eigenmächtiges Handeln
// 1 - schießen auf Feinde, ohne dabei vom aktuellen Kommando abzuweichen
// 2 - auf Feinde wird geschossen und sie werden über kurze Strecken verfolgt
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
  return 1;
}

public func GetAggroTarget()
{
  return(GetEffect("Aggro", this).var1);
}


// Sucht nach Waffen und läuft dorthin
public func SearchWeapon(int iAggro)
{
  // Nächstbeste Spawnpunkte abklappern
  for(var pSpawn in FindObjects(Find_ID(SPNP), Sort_Random()))
    // Da ist eine Waffe drin?
    if(pSpawn -> Contents() ->~ IsWeapon())
      // Die haben wir auch noch nicht?
      if(!FindContents(pSpawn->Contents()->GetID()))
      	// Einsammelbar?
    		if(pSpawn->CheckCollect(GetOwner(), this))
        	// Hinlaufen
        	return(SetMacroCommand(nil, "MoveTo", pSpawn, 0, 0, 0, iAggro));
}

// Sucht nach Munition und läuft dorthin
public func SearchAmmo(int iAggro)
{
  // Nächstbeste Spawnpunkte abklappern
  for(var pSpawn in FindObjects(Find_ID(SPNP), Sort_Random()))
    // Da ist Munition drin?
    if(pSpawn -> Contents() ->~ IsAmmo())
    	// Einsammelbar?
    	if(pSpawn->CheckCollect(GetOwner(), this))
      	// Hinlaufen (wir sind gutgläubig und denken, dass wir die auch brauchen)
      	return(SetMacroCommand(nil, "MoveTo", pSpawn, 0, 0, 0, iAggro));
}
























// Lässt den Clonk von einem Wegpunkt zum anderen laufen, wobei er die jeweilige Pfadeigenschaft beachten muss
private func MoveAlongPath(object pCurrentWp, object pNextWp, int iNextStep)
{
  // Rausfinden, welchen Weg wir gehen müssen
  for(var i = 0, path = -1 ; i < pCurrentWp->GetPathCount() ; i++)
    if(pCurrentWp->GetPathTarget(i) == pNextWp)
    {
      path = i;
      break;
    }
  // Kein Weg? -> Fehler
  if(path < 0) return(MacroComSuccessFailed(0, Macro_PathBroken));
  // Sonderbehandlung für Zielwegpunkte, die nur einen Pfad von sich wegführen haben (da könnten Jumppads o.ä. sein)
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
    SetCommand("MoveTo", pNextWp);
    return(AddSpecialCommands(pCurrentWp, path));
  }
  if(flag == Path_Jump)
  {
    // Springen und dann laufen
    var dir = pCurrentWp->GetPathDir(path);
    if(dir < 0) dir = 0;
    SetDir(dir);
    Jump();
    SetCommand("MoveTo", pNextWp);
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
		  iEff.var0 = COMD_Right;
		else
		  iEff.var0 = COMD_Left;
    Jump();
    ScheduleCall(nil, "JumpStart", 1, 1, true);

    SetCommand("MoveTo", pNextWp);
    return(AddSpecialCommands(pCurrentWp, path));
  }
  if(flag == Path_Lift)
  {
    // Wir suchen den Lift und warten oder betreten ihn
    LiftControl(nil, ObjectNumber(pCurrentWp), ObjectNumber(pNextWp));
    return(AddSpecialCommands(pCurrentWp, path));
  }
  // Unbekanntes flag, MoveTo versuchen
  SetCommand("MoveTo", pNextWp);
  return(AddSpecialCommands(pCurrentWp, path));
}

/* Sonderfunktionen */

protected func JumppadCheck(object pTargetWp, int iNextStep)
{
  // Pfad hat sich irgendwie geändert
  if(GetLength(aPath) <= iNextStep) return;
  var pNextWp = aPath[iNextStep];
  // Feststellen, ob pTargetWp immernoch unser Bewegungsziel ist
  if(aPath[iNextStep-1] != pTargetWp)
    return;
  // Schauen, ob wir näher an pNextWp dran sind
  if(ObjectDistance(pTargetWp) > ObjectDistance(pNextWp))
    // Wegpunkt wird übersprungen
    return(MacroComMoveToStep(nil, iNextStep));
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

  if(targety < GetY()) SetComDir(COMD_Up);
  if(targety > GetY()) SetComDir(COMD_Down);

  // 2do: Wir müssen irgendwie feststellen, ob die Leiter aus ist. :(


  // Wegpunkt oben? früher abspringen.
  if(Inside(GetY()-targety, 5, 15) && Abs(GetX()-targetx) < 30)
  {
    var comd = GetComDir();
    if(targetx < GetX())
    {
    	this->~ControlLadder("ControlLeft");
    	if(GetDir() == DIR_Left)
    	  SetComDir(comd);
   	}
    if(targetx > GetX())
    {
    	this->~ControlLadder("ControlRight");
    	if(GetDir() == DIR_Right)
    	  SetComDir(comd);
   	}
  }

  // Sind wir ungefähr da?
  if(Inside(GetY(), targety-5, targety+5) && PathFree(GetX(), GetY(), targetx, targety))
  {
    // Wir versuchen mal, abzuspringen (das klappt spätestens beim 2. Mal(theoretisch))


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
    	this->~ControlLadder("ControlLeft");
    	if(GetDir() == DIR_Left)
    	  SetComDir(comd);
   	}
    if(targetx > GetX())
    {
    	this->~ControlLadder("ControlRight");
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
  var jetpack = this->~HasJetpack();
  if(!jetpack) return;
  // Richtung zünden
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
  // Fässt den Lift schon an?
  if(GetAction() == "Push")
  {
    var lift = GetActionTarget();
    if(!lift) return;
    // Nah genug am Ziel? Absteigen
    if(ObjectDistance(Object(pNextWp)) <= 50)
      return(AddCommand("MoveTo", Object(pNextWp)));
    // Liftplatten befehligen
    lift->~ControlCommand("MoveTo", 0, Object(pNextWp)->GetX());
    // Warten
    AddCommand("Call", this, pCurrentWp, pNextWp, nil, 0, "LiftControl");
    AddCommand("Wait", nil, 0, 0, nil, 0, 15);
    return 1;
  }
  // Liftplatte suchen
  var x1 = Object(pCurrentWp)->GetX();
  var x2 = Object(pNextWp)->GetX();
  if(x1 > x2) { x1 = x2; x2 = Object(pCurrentWp)->GetX(); }
  var lift = FindObject(Find_Func("IsLift"), Find_InRect(AbsX(x1), -25, x2-x1, 50));
  // Kein Lift? -> Fehlschlag
  if(!lift) return(MacroComSuccessFailed(0, Macro_PathBroken));
  // Lift nah genug? -> Einsteigen
  if(ObjectDistance(lift) <= 50)
  {
    AddCommand("Call", this, pCurrentWp, pNextWp, nil, 0, "LiftControl");
    AddCommand("Grab", lift);
    return 1;
  }
  // Warten
  AddCommand("Call", this, pCurrentWp, pNextWp, nil, 0, "LiftControl");

  AddCommand("Wait", nil, 0, 0, nil, 0, 15);
  return 1;
}

// Alles was der Wegpunkt als ArriveCommand hat
protected func AddSpecialCommands(object pCurrentWp, int path)
{
  if(!(pCurrentWp->GetArriveCommandCount(path))) return;
  for(var i= pCurrentWp->GetArriveCommandCount(path) - 1; i > -1 ; i--)
    AddCommand(pCurrentWp->GetArriveCommand(path, i, 0, 0, this),
                       pCurrentWp->GetArriveCommand(path, i, 1, 0, this),
                       pCurrentWp->GetArriveCommand(path, i, 2, 0, this),
                       pCurrentWp->GetArriveCommand(path, i, 3, 0, this),
                       pCurrentWp->GetArriveCommand(path, i, 4, 0, this), 0,
                       pCurrentWp->GetArriveCommand(path, i, 5, 0, this));
}


// nächsten Wegpunkt von einer Position aus suchen (geht nur, wenn PathFree!)
private func FindWaypoint(int iX, int iY)
{
  var wp;
  for (wp in FindObjects(Find_ID(WAYP), Find_AtPoint(iX, iY))) {
    if(PathFree(iX, iY, wp->GetX(), wp->GetY()) || GBackSolid(wp->GetX(), wp->GetY()))
      return wp;
  }
  // Nix gefunden
  return;
}



///////////////////////////////////////////////////////////////////////////////////////////////////
//
// legacy code below: App-ClonkAI.C

/*-- Neues Script --*/


global func BotSkill( i ){ if( i==-1) return 20;	return(Min(i, 20));}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Überladenes Hazard-Zeug
// die Wegfindung kann man z.B. auch für das normale Hazard verwenden



// Hier wurde nur der Timer geändert
public func SetAggroLevel(int iLevel, int iDist, int iX, int iY, string text)
{
	if(iLevel > 3) return;
	// Wir kommen von > 2
	var target;
	if(GetAggroLevel() >= 2 && iLevel < 2)
		if(target = GetEffect("Aggro", this).var1)
			if(GetMacroCommand(1, 1) == target)
			{
				FinishMacroCommand(1, 0, 1);
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
	// Doppelsprung ausführen, wenn sinnvoll
	if( WildcardMatch( GetAction(),"Jump*")) if(GBackSolid(-20+GetDir()*40))
		this->~ControlUp();

	// Je schlechter der Botskill, desto öfter machen wir hier Pause
	if(Random(8-BotSkill(7))) return;

	// Kugeln ausweichen
	UTBotAIDodgeBullets();

	var target = no.var1;

	if(target && !PathFree(GetX(), GetY(), target->GetX(), target->GetY()))
		target = 0;

		// Wir haben ein Ziel?
	if(target)
	{
		EffectCall(this, no, "Fire");
		// ein Ausweichsprung
		if(ObjectDistance( no.var1 ) < 25 && this->~CheckEnemy(no.var1, this, false)) UTBotAIDodgeJump();
		return;
	}
	else
	{
		// Waffe soll zu schießen aufhören
		if(Contents()) Contents()->~StopAutoFire();
	}

	var weaprange = 400;
	if(Contents())
		weaprange = Contents()->~GetBotData(BOT_Range);

	target = UTBotAIFindEnemy(this, Max(400, weaprange));

	if(!target)
	{
		// Nichts gefunden :(
		// -> Bescheid geben!
		if(no.var99)
		{
			if(Contained())
				Contained()->~HandleAggroFinished(this);
			else if(this->~IsRiding())
				GetActionTarget()->~HandleAggroFinished(this);
		}
		// -> Waffen durchchecken
		return;
	}
	// Super
	no.var1 = target;
	no.var99 = true; // wir haben ein Ziel \o/}
}

public func FxAggroFire(object pTarget, int no)
{
	// Zusatzhack: BR-Bombe!
	if(GetID(Contents()) == GBRB)
		// Nichts tun :C
		return;
	// Nichts tun, wenn gerade verhindert
	if(!this->~ReadyToFire()) return;
	var y = no.var4;
	var x = no.var3;
	var dist = no.var2;
	var target = no.var1;
	var level = no.var0;
	var weaprange = 0;

	if(Contents())
		weaprange = Contents()->~GetBotData(BOT_Range);


	// Fahrzeugsteuerung
	if(Contained())
	{
		if(Contained()->~HandleAggro(this, level, target, dist, x, y))
			return 1;
		else
			return(AddCommand("Exit", nil, 0, 0, nil, 0, 0, 0, C4CMD_SilentSub));
	}

	// Ziel irgendwie weg?
	// (Pathfree wurde schon gecheckt)
	if(!this->~CheckTarget(target, this, Max(dist, weaprange), 0, 0, true))
	{
			no.var1 = 0;
			if(no.var0 == 2)
				ClearMacroCommands();
			if(Contents()) Contents()->~StopAutoFire();
			return;
	}
	// Ich hab nix? °-°
	if(!Contents()) return; // Lauf, Forest, lauf!
	// Waffe in die Hand nehmen
	if(!SelectWeapon(level, target, false))
	{
			// Bei Aggro_Follow können wir von unserem Pfade weg. D.h. eine Waffe und/oder Munition muss her
			if(GetAggroLevel() == Aggro_Follow)
			{
				// Waffen auffrischen?
				if(this->~CustomContentsCount("IsWeapon") <= 1)
					return(SearchWeapon(Aggro_Shoot));
				// Munition auffrischen
				return(SearchAmmo(Aggro_Shoot));
			}
			// ein Balrog, ein Feind gegen den ihr nichts ausrichten könnt...lauft!
			return;
	}
	// Stufe 1 - nur in die grobe Richtung ballern, lieber nicht anhalten oder sowas


	var pWeapon = Contents();

	 // Gut. Feuern wir bereits?
	if(pWeapon->IsRecharging()) return;

	// Er schießt nicht immer, wenn er kann
	var efficiency = pWeapon->~GetBotData( BOT_Rating );
	if( Random( efficiency - BotSkill(efficiency-1) )) return;

	UTBotAIAdjustDir( target );

	// Feuer!
	// Stufe 2 - verfolgen!
	if(no.var0 >= 2)
		if(GetMacroCommand(1) != "Follow" || GetMacroCommand(1, 1) != target)
			if(GetMacroCommand(0) != "Follow" || GetMacroCommand(0, 1) != target)
			{
				DebugLog("FxAggroFire - Adding Follow command","aggro");
				AddMacroCommand(nil, "MoveTo", nil, GetX(), GetY(), 0, level);
				AddMacroCommand(nil, "Follow", target, 0, 0, 0, level);
			}



	UTBotAIAimAt(target);
	// Waffe benutzen
	var override = pWeapon->~GetBotData(BOT_Override);
	if( override )
	{
		pWeapon->~UTBotAIUse( this, target );
		return;
	}


	if( Inside( ObjectDistance( pTarget ), pWeapon->~GetBotData(BOT_RangeMin, 2) , pWeapon->~GetBotData(BOT_Range, 2) ) )
	{
		UTBotAIFire( pWeapon, 2);
	}
	else
	{
		UTBotAIFire( pWeapon, 1);
	}

	return;
}

// Wenn iLevel = 1 (Aggro_Shoot) werden keine Waffen mit FM_Aim ausgewählt
public func SelectWeapon(int iLevel, object pTarget, bool fFireModes)
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

	if(selection) ShiftContents(false, GetID(selection));

	return( Contents() );
}


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

	return type;
}





// Das ganze Hazard-Ding ist ja mal ziemlich Banane gewesen, mal sehen, ob ich hier einen guten
// Algorithmus einbauen kann. Wir machen das jetzt mit Dijkstra - Floyd-Warshall kann zwar negative
// Kantengewichte und so, aber das kommt hier eh nicht vor, außerdem müsste man da erstmal eine
// Matrix aufstellen.

// gibt Pfad von Wegpunkt pStart bis pEnd als Array aus. An Stelle 0 ist pStart.
// wenn FindPath = 0, hat er keinen Pfad gefunden.
public func FindPath(object pStart, object pEnd, bool fJetpack)
{
	var pCurrent = pStart;

	var aNodes = []; // das Array, in welchem die Wegpunkte gespeichert werden
	var aDistance = []; // dieses Array gibt an, wie weit ich zum Wegpunkt laufen muss
						 // könnte ausschließlich mit ObjectDistance() gemacht werden
						 // stattdessen erlauben wir auch, dass Teleporter oder Jumppads
						 // eine geringere "Distanz" haben, weil sie den Clonk schneller
						 // ans Ziel bringen
	var aPrev = [];		 // die Vorgänger, daraus bauen wir einen Pfad zusammen
	var bPath = [];		 // aNodes hilft nur für die Nachbarschaft, bPath ist der gewählte Pfad
	var aSet = [];		 // Suchmenge

	var jetp = 0;
	var ammoload = JTPK->GetFMData(FM_AmmoLoad);

	// Initialisierung: Startpunkt in die Knotenmenge, Label ist 0
	aNodes = FindObjects(Find_ID(WAYP));
	aSet = aNodes;
	for(var i=0; i<GetLength(aNodes); i++)
		aDistance[i]=-1;
	aDistance[GetIndexOf(aNodes, pCurrent)] = 0;

	// jetzt der eigentliche Algorithmus
	var bEnd = false;
	while(GetLength(aSet)&&(!bEnd))
	{

		var aBest = [], pNode, iBest = -1;

		// besten Knoten auswählen
		for( pNode in aSet)
		{
			var dist = aDistance[GetIndexOf(aNodes, pNode)];
			//var dist = aDistance[i];
			if(dist < 0) continue;
			if(iBest == -1 || dist < iBest )
			{
				iBest = dist;
				DebugLog("Least Distance = %d (Obj %d)","dijkstra", iBest, ObjectNumber(aSet[i]));
			}
		}
		for( pNode in aSet )
		{
			if( aDistance[GetIndexOf(aNodes, pNode)] == iBest )
					PushBack(pNode, aBest);
		}
		DebugLog("Best Nodes: %v","dijkstra", aBest);
		pCurrent = aBest[Random(GetLength(aBest))];

		if(!pCurrent) break;

		RemoveArrayValue(aSet, pCurrent);

		var pNext = 0;
		var pathcount = (pCurrent->WAYP->GetPathCount());
		var iCurrent = GetIndexOf(aNodes, pCurrent);

		// alle Nachbarknoten des besten Knotens nach kürzeren Wegen absuchen
		for(var i=0; i<pathcount; ++i)
		{
			pNext = (pCurrent->WAYP->GetPathTarget(i));
			var iNext = GetIndexOf(aNodes, pNext);

			if(!Check4Jetpack(pCurrent, i, fJetpack, jetp, ammoload))
			{
				DebugLog("   Checking Path %d - Waypoint %d: Needs Jetpack","dijkstra", i, ObjectNumber(pNext));
				continue;
			}


			// jetzt die Distanz updaten
			var distnew = aDistance[iCurrent] + pCurrent->~GetPathLengthWP(i);
			DebugLog("   Checking Path %d - Waypoint %d: new %d, old %d","dijkstra", i, ObjectNumber(pNext), distnew, aDistance[iNext]);

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
	bPath=[pEnd];
	pCurrent = pEnd;
	while( pCurrent = aPrev[GetIndexOf(aNodes, pCurrent)])
	{
		PushFront(pCurrent, bPath);
	}

	return bPath;
}



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Eigenes Zeug, braucht man nicht für Hazard


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
		this->~ControlUp();
	else
		Jump();
}


public func UTBotAICalcAimAngle( object obj, object gun, int xmod, int ymod)
{
	var angle, skill = 22-BotSkill(-1);

	// fliegt gerade oder ist hitscan?
	if(!(gun->~GetFMData(FM_Hitscan)))
	{
		 angle=Angle(GetX(), GetY(), GetX(obj)+xmod, GetY(obj)+ymod);
		 angle=angle-Random(skill+1)+skill/2;
		 return angle;
	 }

		var prec = 1000;
		var t, x1, y1, x2, y2, v1x, v1y, v2x, v2y, v1, v2, f1, f2;

		// Position des Ziels
		x1 = prec*GetX(obj);
		y1 = prec*GetY(obj);
		v1x = obj->GetXDir(prec);
		v1y = obj->GetYDir(prec);
		v1 = Distance(v1x, v1y); // Betrag der Geschwindigket

		// Objekt fällt ballistisch?
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
		// Projektil fällt ballistisch?
		if( gun->~GetFMData(FM_Ballistic) )
			f2 = 1;
		else
			f2 = 0;


		// Flugzeit-Schätzung:
		// Mittelwert zwischen Direktverbindung und geschätzter ballistischer Strecke
		// geschätzte Ballistische Strecke = 	* Direktverbindung, wenn f1 = f2 = 0
		//										* Summe der Dreieckskanten im K-Sys, wenn f1 = f2 = 1
		//										* der Mittelwert davon, sonst

		var dist1 = Distance(x1, y1, x2, y2);
		var dist2 = ( (2-f1-f2)*dist1 + (f1+f2)*(Abs(x2-x1)+Abs(y2-y1)) )/2;

		t= (dist1 + dist2)/(2*(v1+v2));
		t= dist2 /(v1+v2);

		v2x = -(-t*v1x +x2-x1)/t;
		v2y = -(-t*v1y +y2-y1 + (t*t*(f2-f1)*prec*GetGravity())/1000)/t;

		angle = Angle(0, 0, v2x, v2y);
		angle=angle-Random(skill+1)+skill/2;
		return angle;
}


public func UTBotAIFindDodgeBullet()
{
	var obj;

	var bullets = FindObjects( Find_InRect(-180,-60, 360, 120), Find_Func( "IsBullet" ) );

	for( obj in bullets )
	{
		if(this->~CheckEnemy(obj, this)) return obj;
	}

	return 0;
}

public func UTBotAIFindEnemy( object pExclude, int iRadius)
{
	var obj;

	var enemy = this->~UTBotAIFindEnemies(pExclude, iRadius, false);

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
	if(this->~IsAiming())
	{
		var angle = UTBotAICalcAimAngle( target, Contents(), xmod, ymod);

		var tx = GetX()+Sin(angle, 1000);
		var ty = GetY()-Cos(angle, 1000);

		if (!this.crosshair) this->~InitCrosshair();

		this->~DoMouseAiming(tx, ty);
		return angle;
	}
}

