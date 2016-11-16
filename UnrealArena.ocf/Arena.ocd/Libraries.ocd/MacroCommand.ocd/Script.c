
/* KI-Zeugs */

// Array mit Effektnummern
// Effektvariablen:
// 0: Kommandoname (string)
// 1: Zielobjekt (objekt)
// 2: Ziel-X (int)
// 3: Ziel-Y (int)
// 4: Verzögerung (int)
// 5: Callbackobjekt (object)
// 6: Aggrolevel
local aMacroCommandList;
// Wegpunktearray
local aPath;

// iAggro = -1 bedeutet keine Änderung am Aggrolevel
// Einen Makrobefehl setzen
func SetMacroCommand(object pCallback, string szCommand, object pTarget, int iX, int iY, int iDelay, int iAggro)
{
	// kein ordentlicher Befehl -> nichts tun
	if (!szCommand) return;
	if (szCommand != "MoveTo" && szCommand != "Follow" && szCommand != "Wait" && szCommand != "None")
	{
		FatalError(Format("Unknown macro command: %s", szCommand));
		return;
	}
	// Parameter evtl. vorbearbeiten oder besondere Checks
	if (szCommand eq "MoveTo")
		if (pTarget)
		{
			iX = pTarget->GetX();
			iY = pTarget->GetY();
		}
	if (szCommand eq "Follow")
		if (!pTarget)
		{
			FatalError("Too few arguments for FOLLOW (no target object)");;
			return;
		}
	// Aktuelle Beschäftigung beenden
	var currCom = GetMacroCommand(0, 0);
	if (currCom)
	{
		ClearScheduleCall(this, Format("MacroCom%s", currCom));
		SetCommand("None");
		SetComDir(COMD_Stop);
	}
	// Liste löschen
	//Log("%s #%d: ClearMacroCom by SetMacroCom", GetName(), ObjectNumber());
	ClearMacroCommands();
	// Befehl eintragen
	var iEffect = AddEffect("MacroCommand", this, 300, 0, this);
	aMacroCommandList[0] = iEffect;
	iEffect.var0 = szCommand;
	iEffect.var1 = pTarget;
	iEffect.var2 = iX;
	iEffect.var3 = iY;
	iEffect.var4 = iDelay;
	iEffect.var5 = pCallback;
	iEffect.var6 = iAggro;
	// Ausführung beantragen
	ClearScheduleCall(this, "MacroCommandMoveTo");
	ClearScheduleCall(this, "MacroCommandFollow");
	ClearScheduleCall(this, "MacroCommandWait");
	//Log("%s #%d: SetMacroCom: '%s' %d/%d Target: %s #%d", GetName(), ObjectNumber(), szCommand, iX, iY, GetName(pTarget), ObjectNumber(pTarget));
	if (szCommand != "None")
		ScheduleCall(this, Format("MacroCom%s", szCommand), 1);
	else
	{
		ClearMacroCommands();
		if (iAggro != -1) SetAggroLevel(iAggro, 0, 0, 0,"SetMacroCommand");
	}
	return 1;
}

// Befehl an den Anfang setzen
func AddMacroCommand(object pCallback, string szCommand, object pTarget, int iX, int iY, int iDelay, int iAggro)
{//Log("%s #%d: AddMacroCom", GetName(), ObjectNumber());
	// Befehlsliste zwischenspeichern
	var aComs = aMacroCommandList;
	// Und leeren
	var currCom = GetMacroCommand(0, 0);
	if (currCom)
	{
		ClearScheduleCall(this, Format("MacroCom%s", currCom));
		SetCommand("None");
		SetComDir(COMD_Stop);
	}
	aMacroCommandList = CreateArray();
	// Kommando sollen nicht gelöscht werden
	AddEffect("MacroCommandLocked", this, 301);
	// Befehl einsetzen
	var ret = SetMacroCommand(pCallback, szCommand, pTarget, iX, iY, iDelay, iAggro);
	// Andere Befehle wieder eintragen
	if (ret)
	{
		for (var iCom in aComs)
			if (iCom)
				aMacroCommandList[GetLength(aMacroCommandList)] = iCom;
	}
	else
		aMacroCommandList = aComs;
	// Kommandos freigeben
	RemoveEffect("MacroCommandLocked", this);
	// Rückgabe
	return ret;
}

// Befehl ans Ende setzen
func AppendMacroCommand(object pCallback, string szCommand, object pTarget, int iX, int iY, int iDelay, int iAggro)
{
	// kein ordentlicher Befehl -> nichts tun
	if (!szCommand) return;
	if (szCommand != "MoveTo" && szCommand != "Follow" && szCommand != "Wait" && szCommand != "None")
	{
		FatalError(Format("Unknown macro command: %s", szCommand));
		return;
	}
	// Parameter evtl. vorbearbeiten oder besondere Checks
	if (szCommand eq "MoveTo")
		if (pTarget)
		{
			iX = pTarget->GetX();
			iY = pTarget->GetY();
		}
	if (szCommand eq "Follow")
		if (!pTarget)
		{
			FatalError("Too few arguments for FOLLOW (no target object)");;
			return;
		}
	// Befehl eintragen
	var iEffect = AddEffect("MacroCommand", this, 300, 0, this);
	aMacroCommandList[GetLength(aMacroCommandList)] = iEffect;
	iEffect.var0 = szCommand;
	iEffect.var1 = pTarget;
	iEffect.var2 = iX;
	iEffect.var3 = iY;
	iEffect.var4 = iDelay;
	iEffect.var5 = pCallback;
	iEffect.var6 = iAggro;
	// Starten?
	if (GetLength(aMacroCommandList) == 1)
	{
		if (szCommand != "None")
			Call(Format("MacroCom%s", szCommand));
		else
			ClearMacroCommands();
	}
	return 1;
}

// Abfrage
func GetMacroCommand(int iCommandNum, int iElement)
{
	// An der Stelle gibt es nichts?
	if (iCommandNum < 0 || GetLength(aMacroCommandList) <= iCommandNum) return;
	// Element zurückgeben
	return aMacroCommandList[iCommandNum][Format("var%d", iElement)];
}

// Beenden (wenn, dann nur extern gebraucht)
func FinishMacroCommand(bool fNoCallback, bool fNoSuccess, int iCommandNum)
{
	// An der Stelle gibt es nichts?
	if (iCommandNum < 0 || GetLength(aMacroCommandList) <= iCommandNum) return;
	// mit Callback -> Callback machen
	if (!fNoCallback)
	{
		if (!fNoSuccess)
			return MacroComSuccess(iCommandNum);
		else
			return MacroComSuccessFailed(iCommandNum);
	}
	// Ohne Callback -> kein Callback ;)
	RemoveMacroCommand(iCommandNum);
	// Nächstes Kommando starten
	if (GetMacroCommand() && !iCommandNum)
		if (GetMacroCommand() != "None")
			Call(Format("MacroCom%s", GetMacroCommand()));
	return 1;
}

// Löscht alle Makrobefehle (ohne Callbacks)
func ClearMacroCommands()
{
	while(GetMacroCommand())
		RemoveMacroCommand();
	aMacroCommandList = CreateArray();
	ClearScheduleCall(this, "MacroComMoveTo");
	ClearScheduleCall(this, "MacroComWait");
	ClearScheduleCall(this, "MacroComFollow");
	ClearScheduleCall(this, "MacroComSuccessFailed");
	ClearScheduleCall(this, "JumppadCheck");
}

/* Makros */

// MoveTo
func MacroComMoveTo()
{
	// Erstes Kommando ist gar nicht MoveTo?
	if (GetMacroCommand(0, 0) != "MoveTo") return;
	// Kein Ziel? (bewirkt, dass der Clonk nicht nach 0, 0 laufen kann!)
	if (!GetMacroCommand(0, 2) && !GetMacroCommand(0, 3))
		if (!GetMacroCommand(0, 4))
			return MacroComSuccessFailed(0, Macro_MoveToNoTarget);
	// Pfad suchen
	aPath = CreateArray();
	var start, end;
	start = FindWaypoint(GetX(), GetY());
	end = FindWaypoint(GetMacroCommand(0, 2), GetMacroCommand(0, 3));
	if (!start || !end) return MacroComSuccessFailed(0, Macro_NoPath);
	aPath = FindPath(start, end, true);
	// Kein Pfad konnte gefunden werden?
	if (!aPath) return MacroComSuccessFailed(0, Macro_NoPath);

	// ist der Endpunkt ein Transportpunkt? (ein Wegpunkt mit nur einem Ausgang
	// und dieser Ausgang zeigt nicht zum Eingang)
	if (end->GetPathCount() == 1) {
		var before = 0;
		var leng = GetLength(aPath);
		if (start != end)
			before = aPath[leng-2];
		if (end->GetPathTarget(0) != before) {
			// Ziel ändern (Regel: Pfad darf nicht auf Transportpunkt enden)
			aPath[leng] = end->GetPathTarget(0);
			// Ziel ändern auf folgenden Wegpunkt, Position entsprechend.
			aMacroCommandList[0].var1 = end->GetPathTarget(0);
			aMacroCommandList[0].var2 = GetX(end->GetPathTarget(0));
			aMacroCommandList[0].var3 = GetY(end->GetPathTarget(0));
		}
	}
	// In Bewegung setzen (der erste Wegpunkt sollte durch MoveTo erreichbar sein, wenn nicht -> doof
	// Spezialhack: reitet!
	if (this->~IsRiding()) SetAction("Walk");
	SetCommand("MoveTo", aPath[0]);
	AppendCommand("Call", this, 1, 0, nil, 0, "MacroComMoveToStep");
	// Automatischer Abbruch
	ScheduleCall(this, "MacroComSuccessFailed", 500, 0, 0, Macro_PathImpossible);
	// Falls Startwegpunkt = Jumppadpunkt
	if (GetLength(aPath) > 1 && aPath[0]->GetPathCount() == 1)
		ScheduleCall(this, "JumppadCheck", 5, 0, aPath[1], 2);
	// Aggrolevel
	if (GetMacroCommand(0, 6) != -1)
		SetAggroLevel(GetMacroCommand(0, 6), 0, 0, 0,"MacroComMoveTo");
	//Log("%s #%d: MacroComMoveTo", GetName(), ObjectNumber());
}

func MacroComMoveToStep(object dummy, int iStep) // MoveTo-Schritt
{
	// Erstes Kommando ist gar nicht MoveTo?
	if (GetMacroCommand(0, 0) != "MoveTo") return;
	// Wird haben das Ende des Pfades erreicht?
	if (GetLength(aPath) == iStep)
	{//Log("%s #%d: MacroComMoveTo finished", GetName(), ObjectNumber());
		// Zum Ziel laufen und beenden
		var x = GetMacroCommand(0, 2), y = GetMacroCommand(0, 3);
		SetCommand("MoveTo", nil, x, y);
		AppendCommand("Call", this, 0, 0, nil, 0, "MacroComSuccess");
		return 1;
	}
	// Aus irgendeinem Grund ist kein Wegpunkt da?
	if (!aPath[iStep]) return MacroComSuccessFailed(0, Macro_PathBroken);
	if (!aPath[iStep-1]) return MacroComSuccessFailed(0, Macro_PathBroken);
	// Nächsten Wegpunkt begehen
	MoveAlongPath(aPath[iStep-1], aPath[iStep], iStep+1);
	AppendCommand("Call", this, iStep+1, 0, nil, 0, "MacroComMoveToStep");
	// Automatischer Abbruch
	ClearScheduleCall(this, "MacroComSuccessFailed");
	// 200 Frames pro 100 Pixel Abstand sind angesetzt
	var breaktime = Max(ObjectDistance(aPath[iStep-1], aPath[iStep]) / 100 * 200, 100);
	ScheduleCall(this, "MacroComSuccessFailed", breaktime, 0, 0, Macro_PathImpossible);
	//Log("%s #%d: MacroComMoveToStep", GetName(), ObjectNumber());
	return 1;
}

func MacroComMoveToStepFailed() // MoveTo-Schritt fehlgeschlagen
{//Log("%s #%d: MacroComMoveToStepFailed", GetName(), ObjectNumber());
	// Erstes Kommando ist gar nicht MoveTo?
	if (GetMacroCommand(0, 0) != "MoveTo") return;
	// Fehlschlag auf ganzer Linie
	MacroComSuccessFailed(0, Macro_PathImpossible);
	return 1;
}

// Follow
func MacroComFollow(bool fStarted)
{
	// Sonderbehandlung: wurde schon gestartet
	if (fStarted)
	{
		DebugLog(Format("%s #%d: MacroComFollow timer", GetName(), ObjectNumber()), "MacroComFollow");
		// Prüfen, ob an der Stelle iPos in der Kette "Follow" steht
		if (GetMacroCommand(1) != "Follow" && GetMacroCommand(0) != "Follow")
			// Steht nicht da, ergo muss sich ein anderes Kommando reingeschaltet haben -> nichts tun
			return;
		// Wir haben kein MoveTo mehr? Dann sind wir angekommen (wir starten einfach neu)
		if (GetMacroCommand() != "MoveTo") return MacroComFollow();
		// Unser Ziel ist irgendwie weg?
		if (!GetMacroCommand(1, 1))
		{
			DebugLog(Format("%s #%d: MacroComFollow target lost", GetName(), ObjectNumber()), "MacroComFollow");
			// MoveTo löschen
			RemoveMacroCommand();
			// Fehlschlag ausgeben
			return MacroComSuccessFailed(0, Macro_FollowTargetLost);
		}
		// Schauen, ob unser Bewegungsziel noch ok ist
		var x, y, target;
		x = GetMacroCommand(0, 2);
		y = GetMacroCommand(0, 3);
		target = GetMacroCommand(0, 1);
		// Distanz > 30 Pixel?
		if (Distance(AbsX(x), AbsY(y), AbsX(target->GetX()), AbsY(target->GetY())) > 30)
		{
			DebugLog(Format("%s #%d: MacroComFollow new MoveTo", GetName(), ObjectNumber()), "MacroComFollow");
			// MoveTo löschen
			if (GetMacroCommand() eq "MoveTo") RemoveMacroCommand();
			// Neues Kommando setzen
			AddMacroCommand(nil, "MoveTo", GetMacroCommand(0, 1), 0, 0, 0, GetMacroCommand(0, 6));
		}
		// Follow Weiterlaufen lassen
		return ScheduleCall(this, "MacroComFollow", 30, 0, true);
	}
	DebugLog(Format("%s #%d: MacroComFollow started", GetName(), ObjectNumber()), "MacroComFollow");
	// Erstes Kommando ist gar nicht Follow?
	if (GetMacroCommand(0, 0) != "Follow") return;
	// Kein Ziel? -> Fehlschlag
	if (!GetMacroCommand(0, 1))
		return MacroComSuccessFailed();
	// Wir sind dem Ziel noch nicht nahe genug?
	if (ObjectDistance(GetMacroCommand(0, 1)) > 30)
		// Follow bedeutet viele MoveTos -> loslaufen bitte
		AddMacroCommand(nil, "MoveTo", GetMacroCommand(0, 1), 0, 0, 0, GetMacroCommand(0, 6));
	// Aggrolevel
	//if (GetMacroCommand(0, 6) != -1)
		//SetAggroLevel(GetMacroCommand(0, 6));
	// Alle 30 Frames aktualisieren wir
	return ScheduleCall(this, "MacroComFollow", 30, 0, true);
}

// Wait
func MacroComWait(bool fEnd)
{
	// Erstes Kommando ist gar nicht Wait?
	if (GetMacroCommand(0, 0) != "Wait") return;
	// Beenden?
	if (fEnd)
		return MacroComSuccess(0);
	// Aggrolevel
	if (GetMacroCommand(0, 6) != -1)
		SetAggroLevel(GetMacroCommand(0, 6), 0, 0, 0,"MacroComWait");
	// Dann warten wir jetzt eine Runde
	return ScheduleCall(this, "MacroComWait", GetMacroCommand(0, 4), 0, true);
}

/* Makro-Funktionen */

// Makrobefehl an der Stelle iCommand ist erfolgreich
func MacroComSuccess(iCommand, int iCmd2)
{//Log("%s #%d: MacroComSuccess", GetName(), ObjectNumber());
	// Für Aufrufe aus SetCommand(... "Call");
	if (GetType(iCommand) == C4V_C4Object) iCommand = iCmd2;
	// An der Stelle gibt es nichts?
	if (iCommand < 0 || GetLength(aMacroCommandList) <= iCommand) return;
	// Hat super geklappt! Callbackobjekt benachrichtigen, sofern vorhanden
	var callback = GetMacroCommand(iCommand, 5);
	if (callback) callback->~OnMacroCommandSuccess(this);
	// Kommando aus der Liste entfernen
	RemoveMacroCommand(iCommand);
	// Evtl. Weg löschen
	aPath = CreateArray();
	// Nächstes Kommando starten
	if (GetMacroCommand()) Call(Format("MacroCom%s", GetMacroCommand()));
	return 1;
}

func MacroComTumble()
{//Log("%s #%d: MacroComTumble", GetName(), ObjectNumber());
	MacroComSuccessFailed(0, Macro_PathImpossible);
	this->~CheckStuck();
}

// Fehlercodes:
static const Macro_NoPath = 0; // Keinen Weg gefunden
static const Macro_PathImpossible = 1; // Weg konnte abgelaufen werden
static const Macro_PathBroken = 2; // Pfad wurde beim Ablaufen unterbrochen (Wegpunkt gelöscht o.ä.)
static const Macro_FollowTargetLost = 3; // Target für Follow verloren
static const Macro_MoveToNoTarget = 4; // MoveTo hat kein Ziel bekommen

// Makrobefehl an der Stelle iCommand ist fehlgeschlagen
func MacroComSuccessFailed(int iCommand, int iReason)
{//Log("%s #%d: MacroComSuccessFailed: %d", GetName(), ObjectNumber(), iCommand);
	// An der Stelle gibt es nichts?
	if (iCommand < 0 || GetLength(aMacroCommandList) <= iCommand) return;
	// Hat gar nicht geklappt! Callbackobjekt benachrichtigen, sofern vorhanden
	var callback = GetMacroCommand(iCommand, 5);
	if (callback) callback->~OnMacroCommandFailed(this, iReason);
	// Kommando aus der Liste entfernen
	RemoveMacroCommand(iCommand);
	if (!iCommand) SetCommand("None");
	// Evtl. Weg löschen
	aPath = CreateArray();
	// Nächstes Kommando starten
	if (GetMacroCommand()) Call(Format("MacroCom%s", GetMacroCommand()));
	return 1;
}

// Makrobefehle aus der Liste entfernen
func RemoveMacroCommand(int iCommand)
{//Log("%s #%d: RemoveMacroCommand: %d", GetName(), ObjectNumber(), iCommand);
	// An der Stelle gibt es nichts?
	if (iCommand < 0 || GetLength(aMacroCommandList) <= iCommand) return;
	// Entfernen
	for (var i = 0, iEff, checked ; i < GetEffectCount("MacroCommand", this) ; i++)
	{
		iEff = GetEffect("MacroCommand", this, i);
		if (iEff == aMacroCommandList[iCommand])
		{
			RemoveEffect(nil, this, iEff);
			continue;
		}
		if (GetEffect("MacroCommandLocked", this)) continue;
		for (var mac in aMacroCommandList)
			if (mac == iEff)
				checked = true;
		if (!checked) RemoveEffect(nil, this, iEff, true);
		checked = false;
	}
/*	if (RemoveEffect(0, this, aMacroCommandList[iCommand]))
		Log("Effect removed");*/
	var aNewMacroComList = CreateArray();
	i = 0;
	for (var iCom in aMacroCommandList)
	{
		if (i == iCommand) continue;
		aNewMacroComList[GetLength(aNewMacroComList)] = iCom;
		i++;
	}
	// Erfolgreich entfernt
	aMacroCommandList = aNewMacroComList;
	return 1;
}

func FxMacroCommandStop(object pTarget)
{
	// Wenn wir gesichert sind, werden wir nicht entfernt
	if (GetEffect("MacroCommandLocked", pTarget)) return -1;
}

/* Wegfindung */

// gibt Pfad von Wegpunkt pStart bis pEnd als Array aus. An Stelle 0 ist pStart.
// wenn FindPath = 0, hat er keinen Pfad gefunden.
func FindPath(object pStart, object pEnd, bool fJetpack)
{
	// naive Wegfindung. Wir gehen davon aus, dass wenn der WP links von einem ist,
	// nach links gegangen werden muss und gehen einfach Schritt fuer Schritt in diese
	// Richtung.

	// maximale Iterationen fuer Wegfindung (abhaengig von Anzahl Wegpunkte)
	var iterationLimit = ObjectCount(WAYP);
	
	var ex = GetX(pEnd);
	var ey = GetY(pEnd);

	var pCurrent = pStart;

	var aWaypoints = CreateArray();
	
	var jetp = 0;
	var ammoload = JTPK->GetFMData(FM_AmmoLoad);

	for (var j=0; j< iterationLimit; ++j) {

		aWaypoints[j] = pCurrent;

		// fertig!
		if (pCurrent == pEnd)
			return aWaypoints;

		// ansonsten...
		var cx = GetX(pCurrent);
		var cy = GetY(pCurrent);
		var cangle = Angle(cx, cy, ex, ey);

		var pNext = 0;
		var pathcount = (pCurrent->WAYP->GetPathCount());
		var aBest = CreateArray();
		var good = 0;
		var neutral = 0;
		var bad = 0;

		// alle Nachbarknoten...
		for (var i=0; i<pathcount; ++i) {
			pNext = (pCurrent->WAYP->GetPathTarget(i));

			if (!Check4Jetpack(pCurrent, i, fJetpack, jetp, ammoload))
				continue;

			// Fuer die richtige Richtung zaehlt nicht der Winkel des naechsten Knotens, sondern
			// wenn pNext nur einen Nachfolger hat, zaehlt die Richtung in die der Nachfolger geht
			// (was rekursiv fortsetzbar ist)
			var jetpgedacht = 0;
			while((pNext->WAYP->GetPathCount()) == 1) {
				// Endknoten? Wir brauchen nicht weiteriterieren
				if (pNext == pEnd) break;
				// für den gedachten Pfad muss noch gecheckt werden, ob da nicht Jetpack benötigt wird
				var check = Check4Jetpack(pNext, 0, fJetpack, jetp+jetpgedacht, ammoload);
				if (check)
					pNext = (pNext->WAYP->GetPathTarget(0));
				if (check == 2)
					jetpgedacht += ammoload;
			}

			// Endknoten? Super, fertig!
			if (pNext == pEnd) {
				aBest[0] = i;
				good = 1;
				break;
			}

			// ansonsten
			var nx = GetX(pNext);
			var ny = GetY(pNext);
			var nangle = Angle(cx, cy, nx, ny);
 
			// diffangle: Nimmt Werte von 0 (genau richtig) bis 180 (genau falsch) an
			var diffangle = Abs(Normalize(nangle-cangle,-180));
			// Schummeln: der WP von dem man grad gekommen ist, wird als "bad" eingestuft
			if (j>0)
				if (pNext == aWaypoints[j-1])
					diffangle = 180;
			// möglichst ein Array aus Pfaden zusammenstellen, die etwa in die gewünschte Richtung
			// führen... (Differenz <= 55°)
			if (diffangle <= 55) {
				aBest[good] = i;
				++good;
			}
			// dann vielleicht nicht ganz so zielführende...? (nur wenn kein Pfad in richtige Richtung führt)
			if (diffangle <= 110 && diffangle > 55 && !good) {
				aBest[neutral] = i;
				++neutral;
			}
			// ansonsten dann ein Array aus nicht-zielführenden...
			if (diffangle > 110 && !neutral && !good) {
				aBest[bad] = i;
				++bad;
			}
		}
		// Debug
		if (Debug("waypoints")) {
			var dbg = 0;
			var str = " ";
			var wayp = 0;
			if (good) { str = "good"; dbg = good; }
			else if (neutral) { str = "neutral"; dbg = neutral; }
			else { str = "bad"; dbg = bad; }
			if (dbg>1) {
				DebugLog("	Choosing path from %s...","waypoints", str);
				for (var d = 0; d<dbg; ++d) {
					wayp = (pCurrent->WAYP->GetPathTarget(aBest[d]));
					DebugLog("	waypoint (%d), angle %d","waypoints", ObjectNumber(wayp), Abs(Normalize(Angle(GetX(pCurrent), GetY(pCurrent), GetX(wayp), GetY(wayp))-cangle,-180)));
				}
			}
		}
		
		// zufällig aus möglichen Pfaden auswählen
		var chosenone = -1;
		if (good)			{ chosenone = aBest[Random(good)]; }
		else if (neutral)	{ chosenone = aBest[Random(neutral)]; }
		else if (bad)		{ chosenone = aBest[Random(bad)]; }
		else {
			DebugLog("Failure: Selected waypoint leads into a dead end.","waypoints");
			return false;
		}
		// ansonsten ok
		pNext = (pCurrent->WAYP->GetPathTarget(chosenone));

		// geschätzter Benzinverbrauch bei Jetpack hochzählen
		if (pCurrent->WAYP->GetPathJetpack(chosenone))
			jetp += ammoload;

		DebugLog("Next %s waypoint (%d), angle %d","waypoints", str, ObjectNumber(pNext), Abs(Normalize(Angle(GetX(pCurrent), GetY(pCurrent), GetX(pNext), GetY(pNext))-cangle,-180)));

		// Fertig
		if (pNext == pEnd) {
			DebugLog("Success: Complete path consists of %d waypoints.","waypoints", GetLength(aWaypoints));
		}		

		// kommt bei naechster Iteration ins Array
		pCurrent = pNext;
	}
	DebugLog("Failure: No path found after %d iterations.","waypoints", iterationLimit);
	return false;
}

// nächsten Wegpunkt von einer Position aus suchen (geht nur, wenn PathFree!)
func FindWaypoint(int iX, int iY)
{
	var wp;
	for (wp in FindObjects(Find_ID(WAYP), Find_AtPoint(iX, iY))) {
		if (PathFree(iX, iY, wp->GetX(), wp->GetY()) || GBackSolid(wp->GetX(), wp->GetY()))
			return wp;
	}
	// Nix gefunden
	return;
}

// Vorbedingung für Jetpack prüfen
func Check4Jetpack(object pCurrent, int path, bool fJetpack, int jetp, int ammoload)
{
	// kein Jetpackpath... dann ist ja gut
	if (!(pCurrent->WAYP->GetPathJetpack(path)))
		return 1;
	// ansonsten Vorbedingungen checken
	if (fJetpack)
		if (this->~HasJetpack())
			//if (ammoload <= GetAmmo(GSAM)-jetp)
				return 2;
	return 0;
}

// Lässt den Clonk von einem Wegpunkt zum anderen laufen, wobei er die jeweilige Pfadeigenschaft beachten muss
func MoveAlongPath(object pCurrentWp, object pNextWp, int iNextStep)
{
	// Rausfinden, welchen Weg wir gehen müssen
	for (var i = 0, path = -1 ; i < pCurrentWp->GetPathCount() ; i++)
		if (pCurrentWp->GetPathTarget(i) == pNextWp)
		{
			path = i;
			break;
		}
	// Kein Weg? -> Fehler
	if (path < 0) return MacroComSuccessFailed(0, Macro_PathBroken);
	// Sonderbehandlung für Zielwegpunkte, die nur einen Pfad von sich wegführen haben (da könnten Jumppads o.ä. sein)
	ClearScheduleCall(this, "JumppadCheck"); // Zur Sicherheit
	if (GetLength(aPath) > iNextStep && pNextWp->GetPathCount() == 1)
		ScheduleCall(this, "JumppadCheck", 5, 0, pNextWp, iNextStep);
	// Jetpackfliegen
	if (pCurrentWp->GetPathJetpack(path))
		ScheduleCall(this, "StartJetpack", pCurrentWp->GetPathJetpack(path), 0, pCurrentWp->GetPathJetpackFlag(path));
	// Flag rausfinden und entsprechend agieren
	var flag = pCurrentWp->GetPathFlag(path);
	if (flag == Path_MoveTo)
	{
		// Hinlaufen
		SetCommand("MoveTo", pNextWp);
		return AddSpecialCommands(pCurrentWp, path);
	}
	if (flag == Path_Jump)
	{
		// Springen und dann laufen
		var dir = pCurrentWp->GetPathDir(path);
		if (dir < 0) dir = 0;
		SetDir(dir);
		Jump();
		SetCommand("MoveTo", pNextWp);
		return AddSpecialCommands(pCurrentWp, path);
	}
	if (flag == Path_Backflip)
	{
		// Backflippen und dann laufen
		var dir = pCurrentWp->GetPathDir(path);
		if (dir < 0) dir = 0;
		SetDir(!dir);
		var iEff = AddEffect("ControlStack", this, 110, 5, this);
		if (GetDir() == DIR_Left)
			iEff.var0 = COMD_Right;
		else
			iEff.var0 = COMD_Left;
		Jump();
		ScheduleCall(this, "JumpStart", 1, 1, true);

		SetCommand("MoveTo", pNextWp);
		return AddSpecialCommands(pCurrentWp, path);
	}
	if (flag == Path_Lift)
	{
		// Wir suchen den Lift und warten oder betreten ihn
		LiftControl(nil, ObjectNumber(pCurrentWp), ObjectNumber(pNextWp));
		return AddSpecialCommands(pCurrentWp, path);
	}
	// Unbekanntes flag, MoveTo versuchen
	SetCommand("MoveTo", pNextWp);
	return AddSpecialCommands(pCurrentWp, path);
}

/* Sonderfunktionen */

func JumppadCheck(object pTargetWp, int iNextStep)
{
	// Pfad hat sich irgendwie geändert
	if (GetLength(aPath) <= iNextStep) return;
	var pNextWp = aPath[iNextStep];
	// Feststellen, ob pTargetWp immernoch unser Bewegungsziel ist
	if (aPath[iNextStep-1] != pTargetWp)
		return;
	// Schauen, ob wir näher an pNextWp dran sind
	if (ObjectDistance(pTargetWp) > ObjectDistance(pNextWp))
		// Wegpunkt wird übersprungen
		return MacroComMoveToStep(nil, iNextStep);
	// Weiterlaufen
	ScheduleCall(this, "JumppadCheck", 5, 0, pTargetWp, iNextStep);
}

func ClimbLadder()
{
	if (!GetMacroCommand() && !GetCommand()) return;
	// Testen, ob wir noch klettern
	if (GetAction() != "ScaleLadder") return;
	// Feststellen, ob nach oben oder unten
	var targetx = GetCommand(this, 2);
	var targety = GetCommand(this, 3);
	
	/*
	if (GetActTime() > 5) {
		if (targetx < GetX() && GetDir() != DIR_Right)
			this->~ControlLadder("ControlLeft");
		if (targetx > GetX() && GetDir() != DIR_Left)
			this->~ControlLadder("ControlRight");
	}
	*/
	
	if (targety < GetY()) SetComDir(COMD_Up);
	if (targety > GetY()) SetComDir(COMD_Down);
	
	// 2do: Wir müssen irgendwie feststellen, ob die Leiter aus ist. :(
	
	
	// Wegpunkt oben? früher abspringen.
	if (Inside(GetY()-targety, 5, 15) && Abs(GetX()-targetx) < 30)
	{
		var comd = GetComDir();
		if (targetx < GetX())
		{
			this->~ControlLadder("ControlLeft");
			if (GetDir() == DIR_Left)
				SetComDir(comd);
	 	}
		if (targetx > GetX())
		{
			this->~ControlLadder("ControlRight");
			if (GetDir() == DIR_Right)
				SetComDir(comd);
	 	}
	}
		
	// Sind wir ungefähr da?
	if (Inside(GetY(), targety-5, targety+5) && PathFree(GetX(), GetY(), targetx, targety))
	{
		// Wir versuchen mal, abzuspringen (das klappt spätestens beim 2. Mal(theoretisch))
		
		
		//wenn Ziel weiter oben: immer nach oben abspringen
		if (GetY() > targety)
			SetComDir(COMD_Up);
		//Ziel weiter unten? Nach unten abspringen.
		else if (Abs(GetY()-targety) > 50)
			SetComDir(COMD_Down);
		//Ziel weiter weg? Backflip! :D
		else if (Abs(GetX()-targetx) > 80)
			SetComDir(COMD_Stop);
		
		var comd = GetComDir();
		if (targetx < GetX())
		{
			this->~ControlLadder("ControlLeft");
			if (GetDir() == DIR_Left)
				SetComDir(comd);
	 	}
		if (targetx > GetX())
		{
			this->~ControlLadder("ControlRight");
			if (GetDir() == DIR_Right)
				SetComDir(comd);
	 	}
	}
	// Weiter checken
	ScheduleCall(this, "ClimbLadder", 10);
}

func StartJetpack(int iDir)
{
	// Kein Jetpack mehr? oO
	var jetpack = this->~HasJetpack();
	if (!jetpack) return;
	// Richtung zünden
	if (iDir == Jetpack_Left) // Links
	{
		SetDir(DIR_Left);
		jetpack->ControlLeftDouble(this);
	}
	if (iDir == Jetpack_UpLeft) // Oben links
	{
		SetDir(DIR_Left);
		jetpack->ControlUpDouble(this);
	}
	if (iDir == Jetpack_UpRight) // Oben rechts
	{
		SetDir(DIR_Right);
		jetpack->ControlUpDouble(this);
	}
	if (iDir == Jetpack_Right) // Rechts
	{
		SetDir(DIR_Right);
		jetpack->ControlRightDouble(this);
	}
}

func LiftControl(object dummy, int pCurrentWp, int pNextWp)
{
	// Fässt den Lift schon an?
	if (GetAction() eq "Push")
	{
		var lift = GetActionTarget();
		if (!lift) return;
		// Nah genug am Ziel? Absteigen
		if (ObjectDistance(Object(pNextWp)) <= 50)
			return AddCommand("MoveTo", Object(pNextWp));
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
	if (x1 > x2) { x1 = x2; x2 = Object(pCurrentWp)->GetX(); }
	var lift = FindObject(Find_Func("IsLift"), Find_InRect(AbsX(x1), -25, x2-x1, 50));
	// Kein Lift? -> Fehlschlag
	if (!lift) return MacroComSuccessFailed(0, Macro_PathBroken);
	// Lift nah genug? -> Einsteigen
	if (ObjectDistance(lift) <= 50)
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
func AddSpecialCommands(object pCurrentWp, int path)
{
	if (!(pCurrentWp->GetArriveCommandCount(path))) return;
	for (var i= pCurrentWp->GetArriveCommandCount(path) - 1; i > -1 ; i--)
		AddCommand(pCurrentWp->GetArriveCommand(path, i, 0, 0, this),
											 pCurrentWp->GetArriveCommand(path, i, 1, 0, this),
											 pCurrentWp->GetArriveCommand(path, i, 2, 0, this),
											 pCurrentWp->GetArriveCommand(path, i, 3, 0, this),
											 pCurrentWp->GetArriveCommand(path, i, 4, 0, this), 0,
											 pCurrentWp->GetArriveCommand(path, i, 5, 0, this));
}

/* KI-Kampf */

// Setzt einen bestimmten Aggro-Wert für den Clonk
// 0 - keinerlei eigenmächtiges Handeln
// 1 - schießen auf Feinde, ohne dabei vom aktuellen Kommando abzuweichen
// 2 - auf Feinde wird geschossen und sie werden über kurze Strecken verfolgt
// 3 - Position (iX, iY) bewachen und dabei nur iDist weit weglaufen (wenn Feind verfolgt werden)
// Konstanten:
static const Aggro_Nothing = 0;
static const Aggro_Shoot	 = 1;
static const Aggro_Follow	= 2;
static const Aggro_Guard	 = 3;

func SetAggroLevel(int iLevel, int iDist, int iX, int iY, string text)
{
//	if (GetOwner() < -1) Message("@SetAggroLevel: %d: %s", this, iLevel, text);
	// > 3
	if (iLevel > 3) return;
	// Wir kommen von > 2
	var target;
	if (GetAggroLevel() >= 2 && iLevel < 2)
		if (target = GetEffect("Aggro", this).var1)
			if (GetMacroCommand(1, 1) == target)
			{
				FinishMacroCommand(1, 0, 1);
				FinishMacroCommand(1);
			}
	// 0
	if (!iLevel) return RemoveEffect("Aggro", this);
	// 1, 2 und 3
	var effect = GetEffect("Aggro", this);
	if (!effect) effect = AddEffect("Aggro", this, 50, 10, this);
	effect.var0 = iLevel;
	// Parameter
	// iDist, Default = 500
	if (!iDist)
		iDist = 500;
	effect.var2 = iDist;
	// iX und iY, Default = GetX() & GetY()
	if (!iX && !iY && iLevel == 3)
	{
		iX = GetX();
		iY = GetY();
	}
	effect.var3 = iX;
	effect.var4 = iY;
	return 1;
}

func GetAggroLevel()
{
	var effect = GetEffect("Aggro", this);
	if (!effect) return;
	return effect.var0;
}

// Setzt sofort das Angriffsziel
func SetAggroTarget(object pTarget)
{
	if (GetAggroLevel() == Aggro_Nothing) return;
	GetEffect("Aggro", this).var1 = pTarget;
	return 1;
}

func GetAggroTarget()
{
	return GetEffect("Aggro", this).var1;
}

func FxAggroTimer(object pTarget, int no)
{
	// Wir haben ein Ziel?
	if (no.var1) { EffectCall(this, no, "Fire"); return 1; }
	// Zielen beenden
	if (this->~IsAiming()) this->~StopAiming();
//	Message("@No target", this);
	// Ziel suchen
	var dir = GetDir()*2-1;
	// Vorne
	var target = this->~GetTarget(90*dir, 90);
	// Hinten
	if (!target)
		if ((!GetCommand() && !GetMacroCommand()) || no.var0 != 1)
			target = this->~GetTarget(-90*dir, 90);
	// Gefunden?
	if (!target)
	{
		// Nichts gefunden :(
		// -> Bescheid geben!
		if (no.var99)
		{
			if (Contained())
				Contained()->~HandleAggroFinished(this);
			else if (this->~IsRiding())
				GetActionTarget()->~HandleAggroFinished(this);
		}
		// -> Waffen durchchecken
		CheckIdleWeapon();
		return;
	}
	// Super
	no.var1 = target;
	no.var99 = true; // wir haben ein Ziel \o/
}

func FxAggroFire(object pTarget, int no)
{
	// Zusatzhack: BR-Bombe!
	if (GetID(Contents()) == GBRB)
		// Nichts tun :C
		return;
	// Nichts tun, wenn gerade verhindert
	if (!this->~ReadyToFire()) return;
	var y = no.var4;
	var x = no.var3;
	var dist = no.var2;
	var target = no.var1;
	var level = no.var0;
	var pathfree = true;
	
	// Fahrzeugsteuerung
	if (Contained())
	{
		if (Contained()->~HandleAggro(this, level, target, dist, x, y))
			return 1;
		else
			return AddCommand("Exit", nil, 0, 0, nil, 0, 0, 0, C4CMD_SilentSub);
	}
	if (this->~IsRiding())
	{
		if (GetActionTarget()->~HandleAggro(this, level, target, dist, x, y))
			return 1;
		else
			return SetAction("Walk");
	}
	
	// Zu weit von der Wachposition entfernt?
	if (level == 3) {
		if (Distance(GetX(), GetY(), x, y) > dist)
		{
			if (GetMacroCommand(1, 1) == target)
			{
				FinishMacroCommand(1, 0, 1);
				FinishMacroCommand(1);
			}
			AddMacroCommand(nil, "MoveTo", nil, x, y, 0, level);
			no.var1 = 0;
		//			Message("@Returning to guarded position", this);
			return;
		}
	}
	
	var maxdist = dist;
	if (!PathFree(GetX(), GetY(), target->GetX(), target->GetY()))
	{
		if (level == 1) maxdist = 0;
		if (level >= 2) maxdist = dist/2;
		pathfree = false;
	}
	
	// Ziel irgendwie weg?
	// (Pathfree wurde schon gecheckt)
	if (!this->~CheckTarget(target, this, maxdist, 0, 0, true))
		{
			no.var1 = 0;
			if (no.var0 == 2)
				ClearMacroCommands();
			if (this->~IsAiming())
				this->~StopAiming();
			return;
		}
	// Ich hab nix? °-°
	if (!Contents()) return; // Lauf, Forest, lauf!
	// Waffe in die Hand nehmen
	if (!SelectWeapon(level, target, false))
		// Evtl. Feuermodus wechseln (dann muss erst nachgeladen werden, aber besser als nichts)
		if (!SelectWeapon(level, target, true))
		{
			// Bei Aggro_Follow können wir von unserem Pfade weg. D.h. eine Waffe und/oder Munition muss her
			if (GetAggroLevel() == Aggro_Follow)
			{
//			Message("@Searching for weapons / ammo", this);
				// Waffen auffrischen?
				if (this->~CustomContentsCount("IsWeapon") <= 1)
					return SearchWeapon(Aggro_Shoot);
				// Munition auffrischen
				return SearchAmmo(Aggro_Shoot);
			}
			// ein Balrog, ein Feind gegen den ihr nichts ausrichten könnt...lauft!
			return;
		}
	// Stufe 1 - nur in die grobe Richtung ballern, lieber nicht anhalten oder sowas

	// Schaue ich in die richtige Richtung?
	if (GetX() < target->GetX())
	{
		if (GetDir() != DIR_Right)
			SetDir(DIR_Right);
	}
	else
	{
		if (GetDir() != DIR_Left)
			SetDir(DIR_Left);
	}

	// Zielen, muss auch mal sein
 if ((!GetCommand() && !GetMacroCommand()) || level != 1 || this->~IsAiming())
 {
	if (pathfree && Contents()->GetBotData(BOT_Range) > 30) // Weg frei und keine Nahkampfwaffe?
	{
		var angle = Angle(GetX(), GetY(), target->GetX(), target->GetY());
		if (((!Inside(angle, 70, 120) && !Inside(angle, 250, 290)) || Contents()->GetFMData(FM_Aim)>0)
				 && maxdist != 300 && ObjectDistance(target) < 300)
		{
			if (!this->~IsAiming()) this->~StartSquatAiming();
			if (this->~IsAiming())
			{
		
			var tx = target->GetX();
		var ty = target->GetY();
		
				if (Contents()->GetBotData(BOT_Ballistic))
					ty -= 15;
		
		this->~DoMouseAiming(tx, ty);
			}
		}
		else
			if (this->~IsAiming())
				this->~StopAiming();
	}
	if (this->~IsAiming() && !this->~CheckAmmo(Contents()->GetFMData(FM_AmmoID), Contents()->GetFMData(FM_AmmoLoad), Contents(), this))
	 	this->~StopAiming();
 }
 
	 // Gut. Feuern wir bereits?
	if (Contents()->IsRecharging() || Contents()->IsShooting()) return;
 
	// Feuer!
	if (maxdist != 300 && pathfree) this->~Control2Contents("ControlThrow");
//	Message("@My target: %s @%d/%d with level %d", this, target->GetName(), target->GetX(), target->GetY(), level);
	// Stufe 2 - verfolgen!
	if (no.var0 >= 2)
		if (GetMacroCommand(1) != "Follow" || GetMacroCommand(1, 1) != target)
			if (GetMacroCommand(0) != "Follow" || GetMacroCommand(0, 1) != target)
			{
				DebugLog("FxAggroFire - Adding Follow command","aggro");
				AddMacroCommand(nil, "MoveTo", nil, GetX(), GetY(), 0, level);
				AddMacroCommand(nil, "Follow", target, 0, 0, 0, level);
			}
}

// Wenn iLevel = 1 (Aggro_Shoot) werden keine Waffen mit FM_Aim ausgewählt
func SelectWeapon(int iLevel, object pTarget, bool fFireModes)
{
	// Entfernung zum Ziel
	var dist = ObjectDistance(pTarget);
	// Keine Waffen in Inventar?
	if (!this->~CustomContentsCount("IsWeapon")) return;
	// Bevorzugten Schadenstyp bestimmen
	var preftype = GetPrefDmgType(pTarget), type;
	// Alle durchgehen und passende prüfen
	for (var i=0, obj, fav, mode, favmode ; obj = Contents(i) ; mode++)
	{
		// Nix Waffe
		if (!(obj->~IsWeapon())) { i++; mode = -1; continue; }
		// Feuermodus
		if (mode && !fFireModes) { i++; mode = -1; continue; }
		if (!(obj->GetFMData(FM_Name, mode))) { i++; mode = -1; continue; }
		if (mode == obj->GetFireMode() && mode) continue;
		// Nix gut
		if (obj->GetFMData(FM_Aim, mode)>0)
			if (iLevel == 1 || !WildcardMatch(GetAction(), "*Walk*"))
				continue;
		// Keine Munition dafür?
		if (!(obj->GetCharge()) && !this->~GetAmmo(obj->GetFMData(FM_AmmoID, mode)))
			continue;
		// EMP nur gegen Maschinen
		if (obj->GetBotData(BOT_EMP, mode))
			if (!(pTarget->~IsMachine()))
				continue;
		// Kein Favorit bisher?
		if (!fav)
		{
			fav = obj;
			type = fav->GetBotData(BOT_DmgType, mode);
			favmode = mode;
		}
		else
		{
			// Favorit hat nicht genug Reichweite
			if (fav->GetBotData(BOT_Range, favmode) < dist)
			{
				// Neue Waffe hat mehr
				if (obj->GetBotData(BOT_Range, mode) > dist)
				{
					fav = obj;
					type = obj->GetBotData(BOT_DmgType, mode);
					favmode = mode;
				}
			}
			else
			{
				// Favorit hat genug Reichweite -> nur wechseln, wenn Schadenstyp besser
				if (pTarget->~OnDmg(obj->GetBotData(BOT_DmgType, mode)) < pTarget->~OnDmg(type) &&
					// Allerdings darf die Waffe nicht zu schwach sein
					fav->GetBotData(BOT_Power, favmode)-1 <= obj->GetBotData(BOT_Power, mode))
				{
					// Neuer Favorit
					fav = obj;
					type = fav->GetBotData(BOT_DmgType);
					favmode = mode;
				}
				else
				{
					// Stärke der neuen Waffe ist größer oder Favorit ist ein Langlader
					if (fav->GetBotData(BOT_Power, favmode) < obj->GetBotData(BOT_Power, mode) ||
						 (fav->GetBotData(BOT_Power, favmode) == BOT_Power_LongLoad && (fav->IsReloading() || !(fav->GetCharge()))))
					{
						// Waffe hat keine extralange Nachladezeit
						if (obj->GetBotData(BOT_Power, mode) != BOT_Power_LongLoad)
						{
							// Neuer Favorit
							fav = obj;
							type = fav->GetBotData(BOT_DmgType);
							favmode = mode;
						}
						// Waffe sollte nicht nachladen und nicht leer sein
						else if (obj->GetCharge() != 0 && !(obj->IsReloading()))
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
			if (fav->GetBotData(BOT_Range, favmode) >= dist)
				// Schadenstyp auch
				if (preftype == type)
					// Stärke auch
						if (fav->GetBotData(BOT_Power, favmode) >= BOT_Power_3)
							break;
		}
	}
	// Auswählen
	if (!fav) return;
	// Feuermodus wechseln?
	if (fFireModes)
		if (favmode && favmode != fav->GetFireMode())
			fav->SetFireMode(favmode);
	if (ContentsCount() == 1) return 1;
	return ShiftContents(false, fav->GetID());
}

func GetPrefDmgType(object pTarget)
{
	var min = pTarget->~OnDmg(DMG_Projectile);
	var type = DMG_Projectile;
	if (pTarget->~OnDmg(DMG_Melee) < min)
	{
		type = DMG_Melee;
		min = pTarget->~OnDmg(type);
	}
	if (pTarget->~OnDmg(DMG_Fire) < min)
	{
		type = DMG_Fire;
		min = pTarget->~OnDmg(type);
	}
	if (pTarget->~OnDmg(DMG_Explosion) < min)
	{
		type = DMG_Explosion;
		min = pTarget->~OnDmg(type);
	}
	if (pTarget->~OnDmg(DMG_Energy) < min)
	{
		type = DMG_Energy;
		min = pTarget->~OnDmg(type);
	}
	if (pTarget->~OnDmg(DMG_Bio) < min)
		type = DMG_Bio;
		
	return type;
}

// Sucht nach Waffen und läuft dorthin
func SearchWeapon(int iAggro)
{
	// Nächstbeste Spawnpunkte abklappern
	for (var pSpawn in FindObjects(Find_ID(SPNP), Sort_Random()))
		// Da ist eine Waffe drin?
		if (pSpawn -> Contents() ->~ IsWeapon())
			// Die haben wir auch noch nicht?
			if (!FindContents(pSpawn->Contents()->GetID()))
				// Einsammelbar?
				if (pSpawn->CheckCollect(GetOwner(), this))
					// Hinlaufen
					return SetMacroCommand(nil, "MoveTo", pSpawn, 0, 0, 0, iAggro);
}

// Sucht nach Munition und läuft dorthin
func SearchAmmo(int iAggro)
{
	// Nächstbeste Spawnpunkte abklappern
	for (var pSpawn in FindObjects(Find_ID(SPNP), Sort_Random()))
		// Da ist Munition drin?
		if (pSpawn -> Contents() ->~ IsAmmo())
			// Einsammelbar?
			if (pSpawn->CheckCollect(GetOwner(), this))
				// Hinlaufen (wir sind gutgläubig und denken, dass wir die auch brauchen)
				return SetMacroCommand(nil, "MoveTo", pSpawn, 0, 0, 0, iAggro);
}

/* Waffenbehandlung wenn nicht im Kampf */

func CheckIdleWeapon()
{
	if (Contents())
		// Hack - mit BR-Bombe tut er gar nichts
		if (Contents()->GetID() == GBRB) return;
	// Keine Waffen im Inventar
	if (!this->~CustomContentsCount("IsWeapon")) return;
	// nachladende Waffe in der Hand
	if (Contents()->~IsWeapon())
		if (Contents()->IsReloading())
			return;
	// Inventar nach Waffe durchsuchen, die man Nachladen könnte
	for (var i=0, mode=1, obj; obj = Contents(i) ; mode++)
	{
		// Keine Waffe
		if (!(obj->~IsWeapon()))
		{
			i++;
			mode = 0;
			continue;
		}
		// Waffe hat gar nicht so viele Modi
		if (!(obj->GetFMData(FM_Name, mode)))
		{
			i++;
			mode = 0;
			continue;
		}
		// Waffe ist voll geladen
		if (obj->GetAmmo(obj->GetFMData(FM_AmmoID, mode)) >= obj->GetFMData(FM_AmmoLoad, mode) / 2)
		{
			i++;
			mode = 0;
			continue;
		}
		// EMP-Modi erstmal nicht laden
		if (obj->GetBotData(BOT_EMP, mode)) continue;
		// Waffe ist nachladbar
		if (this->~CheckAmmo(obj->GetFMData(FM_AmmoID, mode), obj->GetFMData(FM_AmmoLoad, mode) - obj->GetAmmo(obj->GetFMData(FM_AmmoID, mode))))
		{
			mode = obj->GetFireMode();
			break;
		}
		// Nächsten Feuermodus prüfen
		if (mode == obj->GetFireMode()) continue;
		if (this->~CheckAmmo(obj->GetFMData(FM_AmmoID, mode), obj->GetFMData(FM_AmmoLoad, mode)))
			break;
	}
	// Nix gefunden
	if (!Contents(i)) return;
	// Aha! Waffe wechseln!
	ShiftContents(false, obj->GetID());
	// Feuermodus wechseln
	obj->SetFireMode(mode);
	// Und Muni reinhauen
	obj->Reload();
	// Klasse
	return 1;
}

/* Inventarbehandlung (wird von KI-Spieler aufgerufen) */

// Inventar behandeln
func CheckInventory()
{
	for (var i=0, obj ; obj = Contents(i) ; i++)
	{
			// BR-Hack
		if (Contents()->GetID() == GBRB)
		{
			this->~BombingRunBomb();
			break;
		}
		// Wenn man etwas hardcoden möchte, dann einfach die Funktion AI_Inventory im Objekt erstellen und true zurückgeben
		if (obj->~AI_Inventory(this))
			continue;
		// Waffe
		if (obj->~IsWeapon())
			continue;
		// Ausrüstungsgegenstand
		if (obj->~IsHazardGear())
		{
			// Die KI braucht kein Licht und keine doppelten Dinge
		// Kontext der Auskommentierung: http://cppp.tyron.at/bugtrack.php?shbug=833
		// KI legt Taschenlampen an damit Spieler sie in der Dunkelheit sehen können
			if (/*obj->GetGearType() == GEAR_Light || */this->~HasGear(obj->GetGearType()))
			{
				DropObject(obj);
				continue;
			}
			// Ausrüstung wird prinzipiell als was gutes angesehen -> anlegen
			ActivateGear(obj);
			continue;
		}
		// Mine
		if (obj->~IsMine())
		{
			// Platzieren
			PlaceMine(obj);
			continue;
		}
		// Upgrade
		if (obj->~IsUpgrade())
		{
			var wpn = GetUpgradeableWeapon(obj);
			// Eine passende Waffe gefunden -> upgraden (Upgrades sind immer gut.)
			if (wpn) UpgradeWeapon(obj, wpn);
			continue;
		}
		// Munition
		if (obj->~IsAmmoPacket())
		{
			ActivateAmmo(obj);
			continue;
		}
		// Objekt ist wertlos für uns
		DropObject(obj);
	}
}

// Objekt fallen lassen (verzögert, damit die Schleife nicht durcheinander kommt
func DropObject(object pObj)
{
	Schedule(this, Format("Exit(Object(%d), 0, 10);", ObjectNumber(pObj)), 1, 0, this);
	// Nicht wieder einsammeln
	var effect = AddEffect("CollectionException", pObj, 1, 36);
	effect.var0 = this;
}

// Ausrüstung anlegen (verzögert, damit die Schleife nicht durcheinander kommt
func ActivateGear(object pObj)
{
	ScheduleCall(pObj, "Activate", 1, 0, this);
}

// Mine platzieren (verzögert, damit die Schleife nicht durcheinander kommt
func PlaceMine(object pObj)
{
	ScheduleCall(pObj, "Activate", 1, 0, this);
}

// Waffe suchen, die man upgraden kann
func GetUpgradeableWeapon(object pObj)
{
	for (var i=0, obj ; obj = Contents(i) ; i++)
		if (obj->~IsUpgradeable(pObj->GetID()))
			return obj;
}

// Waffe upgraden (verzögert, damit die Schleife nicht durcheinander kommt
func UpgradeWeapon(object pObj, object pWeapon)
{
	ScheduleCall(pObj, "UpgradeObject", 1, 0, pWeapon);
}

// Munition aufnehmen (verzögert, damit die Schleife nicht durcheinander kommt
func ActivateAmmo(object pObj)
{
	ScheduleCall(pObj, "Activate", 1, 0, this);
}

// Hardgecodete Objekte

// Support-Drohne
// Zufällige Waffe für eine Drohne aussuchen
func GetRandomDroneWeapon()
{
	var aWps = CreateArray();
	for (var i=0, obj ; obj = Contents(i) ; i++)
		if (obj->~IsWeapon())
			if (obj->GetFMData(FM_Aim)<=0)
				aWps[GetLength(aWps)] = obj;
	// Zufällig zurückgeben
	if (!GetLength(aWps)) return;
	return aWps[Random(GetLength(aWps))];
}
