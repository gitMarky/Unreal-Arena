/*-- Importiert benötigtes Zeug aus dem Hazardclonk --*/

#appendto UA_Clonk

local ammobag;
local wpneffect;

local LastDmgType;

local aMacroCommandList;
// Wegpunktearray
local aPath;

func Construction()
{
	aMacroCommandList = CreateArray();
	_inherited();
}


// iAggro = -1 bedeutet keine Änderung am Aggrolevel
// Einen Makrobefehl setzen
func SetMacroCommand(object pCallback, string szCommand, object pTarget, int iX, int iY, int iDelay, int iAggro)
{
	// kein ordentlicher Befehl -> nichts tun
	if (!szCommand)
		return;
	if (szCommand != "MoveTo" && szCommand != "Follow" && szCommand != "Wait" && szCommand != "None")
	{
		ErrorLog("Unknown macro command: %s", szCommand);
		return;
	}
	// Parameter evtl. vorbearbeiten oder besondere Checks
	if (szCommand == "MoveTo")
		if (pTarget)
		{
			iX = pTarget->GetX();
			iY = pTarget->GetY();
		}
	if (szCommand == "Follow")
		if (!pTarget)
		{
			ErrorLog("Too few arguments for FOLLOW (no target object)");
			return;
		}
	// Aktuelle Beschäftigung beenden
	var currCom = GetMacroCommand(0, 0);
	if (currCom)
	{
		ClearScheduleCall(this, Format("MacroCom%s", currCom));
		SetCommand(this, "None");
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
		if (iAggro != -1)
			SetAggroLevel(iAggro, 0, 0, 0, "SetMacroCommand");
	}
	return 1;
}

// Befehl an den Anfang setzen
func AddMacroCommand(object pCallback, string szCommand, object pTarget, int iX, int iY, int iDelay, int iAggro)
{
	//Log("%s #%d: AddMacroCom", GetName(), ObjectNumber());
	// Befehlsliste zwischenspeichern
	var aComs = aMacroCommandList;
	// Und leeren
	var currCom = GetMacroCommand(0, 0);
	if (currCom)
	{
		ClearScheduleCall(this, Format("MacroCom%s", currCom));
		SetCommand(this, "None");
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
	if (!szCommand)
		return;
	if (szCommand != "MoveTo" && szCommand != "Follow" && szCommand != "Wait" && szCommand != "None")
	{
		ErrorLog("Unknown macro command: %s", szCommand);
		return;
	}
	// Parameter evtl. vorbearbeiten oder besondere Checks
	if (szCommand == "MoveTo")
		if (pTarget)
		{
			iX = pTarget->GetX();
			iY = pTarget->GetY();
		}
	if (szCommand == "Follow")
		if (!pTarget)
		{
			ErrorLog("Too few arguments for FOLLOW (no target object)");
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
	if (iCommandNum < 0 || GetLength(aMacroCommandList) <= iCommandNum)
		return;
	// Element zurückgeben
	return EffectVar(iElement, this, aMacroCommandList[iCommandNum]);
}

// Beenden (wenn, dann nur extern gebraucht)
func FinishMacroCommand(bool fNoCallback, bool fNoSuccess, int iCommandNum)
{
	// An der Stelle gibt es nichts?
	if (iCommandNum < 0 || GetLength(aMacroCommandList) <= iCommandNum)
		return;
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
	while (GetMacroCommand())
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
	if (GetMacroCommand(0, 0) != "MoveTo")
		return;
	// Kein Ziel? (bewirkt, dass der Clonk nicht nach 0,0 laufen kann!)
	if (!GetMacroCommand(0, 2) && !GetMacroCommand(0, 3))
		if (!GetMacroCommand(0, 4))
			return MacroComSuccessFailed(0, Macro_MoveToNoTarget);
	// Pfad suchen
	aPath = CreateArray();
	var start, end;
	start = FindWaypoint(GetX(), GetY());
	end = FindWaypoint(GetMacroCommand(0, 2), GetMacroCommand(0, 3));
	if (!start || !end)
		return MacroComSuccessFailed(0, Macro_NoPath);
	aPath = FindPath(start, end, true);
	// Kein Pfad konnte gefunden werden?
	if (!aPath)
		return MacroComSuccessFailed(0, Macro_NoPath);
	
	// ist der Endpunkt ein Transportpunkt? (ein Wegpunkt mit nur einem Ausgang
	// und dieser Ausgang zeigt nicht zum Eingang)
	if (end->GetPathCount() == 1)
	{
		var before = 0;
		var leng = GetLength(aPath);
		if (start != end)
			before = aPath[leng - 2];
		if (end->GetPathTarget(0) != before)
		{
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
	if (IsRiding())
		SetAction("Walk");
	SetCommand(this, "MoveTo", aPath[0]);
	AppendCommand(this, "Call", this, 1, 0, 0, 0, "MacroComMoveToStep");
	// Automatischer Abbruch
	ScheduleCall(this, "MacroComSuccessFailed", 500, 0, 0, Macro_PathImpossible);
	// Falls Startwegpunkt = Jumppadpunkt
	if (GetLength(aPath) > 1 && aPath[0]->GetPathCount() == 1)
		ScheduleCall(this, "JumppadCheck", 5, 0, aPath[1], 2);
	// Aggrolevel
	if (GetMacroCommand(0, 6) != -1)
		SetAggroLevel(GetMacroCommand(0, 6), 0, 0, 0, "MacroComMoveTo");
	//Log("%s #%d: MacroComMoveTo", GetName(), ObjectNumber());
}

func MacroComMoveToStep(object dummy, int iStep) // MoveTo-Schritt
{
	// Erstes Kommando ist gar nicht MoveTo?
	if (GetMacroCommand(0, 0) != "MoveTo")
		return;
	// Wird haben das Ende des Pfades erreicht?
	if (GetLength(aPath) == iStep)
	{
		//Log("%s #%d: MacroComMoveTo finished", GetName(), ObjectNumber());
		// Zum Ziel laufen und beenden
		var x = GetMacroCommand(0, 2), y = GetMacroCommand(0, 3);
		SetCommand(this, "MoveTo", 0, x, y);
		AppendCommand(this, "Call", this, 0, 0, 0, 0, "MacroComSuccess");
		return 1;
	}
	// Aus irgendeinem Grund ist kein Wegpunkt da?
	if (!aPath[iStep])
		return MacroComSuccessFailed(0, Macro_PathBroken);
	if (!aPath[iStep - 1])
		return MacroComSuccessFailed(0, Macro_PathBroken);
	// Nächsten Wegpunkt begehen
	MoveAlongPath(aPath[iStep - 1], aPath[iStep], iStep + 1);
	AppendCommand(this, "Call", this, iStep + 1, 0, 0, 0, "MacroComMoveToStep");
	// Automatischer Abbruch
	ClearScheduleCall(this, "MacroComSuccessFailed");
	// 200 Frames pro 100 Pixel Abstand sind angesetzt
	var breaktime = Max(ObjectDistance(aPath[iStep - 1], aPath[iStep]) / 100 * 200, 100);
	ScheduleCall(this, "MacroComSuccessFailed", breaktime, 0, 0, Macro_PathImpossible);
	//Log("%s #%d: MacroComMoveToStep", GetName(), ObjectNumber());
	return 1;
}

func MacroComMoveToStepFailed() // MoveTo-Schritt fehlgeschlagen
{
	//Log("%s #%d: MacroComMoveToStepFailed", GetName(), ObjectNumber());
	// Erstes Kommando ist gar nicht MoveTo?
	if (GetMacroCommand(0, 0) != "MoveTo")
		return;
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
		DebugLog
		(
			Format("%s #%d: MacroComFollow timer", GetName(), ObjectNumber()),
			"MacroComFollow"
		);
		// Prüfen, ob an der Stelle iPos in der Kette "Follow" steht
		if (GetMacroCommand(1) != "Follow" && GetMacroCommand(0) != "Follow")
			// Steht nicht da, ergo muss sich ein anderes Kommando reingeschaltet haben -> nichts tun
			return;
		// Wir haben kein MoveTo mehr? Dann sind wir angekommen (wir starten einfach neu)
		if (GetMacroCommand() != "MoveTo")
			return MacroComFollow();
		// Unser Ziel ist irgendwie weg?
		if (!GetMacroCommand(1, 1))
		{
			DebugLog
			(
				Format("%s #%d: MacroComFollow target lost", GetName(), ObjectNumber()),
				"MacroComFollow"
			);
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
			DebugLog
			(
				Format("%s #%d: MacroComFollow new MoveTo", GetName(), ObjectNumber()),
				"MacroComFollow"
			);
			// MoveTo löschen
			if (GetMacroCommand() == "MoveTo")
				RemoveMacroCommand();
			// Neues Kommando setzen
			AddMacroCommand(0, "MoveTo", GetMacroCommand(0, 1), 0, 0, 0, GetMacroCommand(0, 6));
		}
		// Follow Weiterlaufen lassen
		return ScheduleCall(this, "MacroComFollow", 30, 0, true);
	}
	DebugLog
	(
		Format("%s #%d: MacroComFollow started", GetName(), ObjectNumber()),
		"MacroComFollow"
	);
	// Erstes Kommando ist gar nicht Follow?
	if (GetMacroCommand(0, 0) != "Follow")
		return;
	// Kein Ziel? -> Fehlschlag
	if (!GetMacroCommand(0, 1))
		return MacroComSuccessFailed();
	// Wir sind dem Ziel noch nicht nahe genug?
	if (ObjectDistance(GetMacroCommand(0, 1)) > 30)
		// Follow bedeutet viele MoveTos -> loslaufen bitte
		AddMacroCommand(0, "MoveTo", GetMacroCommand(0, 1), 0, 0, 0, GetMacroCommand(0, 6));
	// Aggrolevel
	//if(GetMacroCommand(0, 6) != -1)
	//SetAggroLevel(GetMacroCommand(0, 6));
	// Alle 30 Frames aktualisieren wir
	return ScheduleCall(this, "MacroComFollow", 30, 0, true);
}

// Wait
func MacroComWait(bool fEnd)
{
	// Erstes Kommando ist gar nicht Wait?
	if (GetMacroCommand(0, 0) != "Wait")
		return;
	// Beenden?
	if (fEnd)
		return MacroComSuccess(0);
	// Aggrolevel
	if (GetMacroCommand(0, 6) != -1)
		SetAggroLevel(GetMacroCommand(0, 6), 0, 0, 0, "MacroComWait");
	// Dann warten wir jetzt eine Runde
	return ScheduleCall(this, "MacroComWait", GetMacroCommand(0, 4), 0, true);
}

/* Makro-Funktionen */

// Makrobefehl an der Stelle iCommand ist erfolgreich
func MacroComSuccess(iCommand, int iCmd2)
{
	//Log("%s #%d: MacroComSuccess", GetName(), ObjectNumber());
	// Für Aufrufe aus SetCommand(... "Call");
	if (GetType(iCommand) == C4V_C4Object)
		iCommand = iCmd2;
	// An der Stelle gibt es nichts?
	if (iCommand < 0 || GetLength(aMacroCommandList) <= iCommand)
		return;
	// Hat super geklappt! Callbackobjekt benachrichtigen, sofern vorhanden
	var callback = GetMacroCommand(iCommand, 5);
	if (callback)
		callback->~OnMacroCommandSuccess(this);
	// Kommando aus der Liste entfernen
	RemoveMacroCommand(iCommand);
	// Evtl. Weg löschen
	aPath = CreateArray();
	// Nächstes Kommando starten
	if (GetMacroCommand())
		Call(Format("MacroCom%s", GetMacroCommand()));
	return 1;
}

func MacroComTumble()
{
	//Log("%s #%d: MacroComTumble", GetName(), ObjectNumber());
	MacroComSuccessFailed(0, Macro_PathImpossible);
	CheckStuck();
}

// Fehlercodes:
static const Macro_NoPath = 0; // Keinen Weg gefunden
static const Macro_PathImpossible = 1; // Weg konnte abgelaufen werden
static const Macro_PathBroken = 2; // Pfad wurde beim Ablaufen unterbrochen (Wegpunkt gelöscht o.ä.)
static const Macro_FollowTargetLost = 3; // Target für Follow verloren
static const Macro_MoveToNoTarget = 4; // MoveTo hat kein Ziel bekommen

// Makrobefehl an der Stelle iCommand ist fehlgeschlagen
func MacroComSuccessFailed(int iCommand, int iReason)
{
	//Log("%s #%d: MacroComSuccessFailed: %d", GetName(), ObjectNumber(), iCommand);
	// An der Stelle gibt es nichts?
	if (iCommand < 0 || GetLength(aMacroCommandList) <= iCommand)
		return;
	// Hat gar nicht geklappt! Callbackobjekt benachrichtigen, sofern vorhanden
	var callback = GetMacroCommand(iCommand, 5);
	if (callback)
		callback->~OnMacroCommandFailed(this, iReason);
	// Kommando aus der Liste entfernen
	RemoveMacroCommand(iCommand);
	if (!iCommand)
		SetCommand(this, "None");
	// Evtl. Weg löschen
	aPath = CreateArray();
	// Nächstes Kommando starten
	if (GetMacroCommand())
		Call(Format("MacroCom%s", GetMacroCommand()));
	return 1;
}

// Makrobefehle aus der Liste entfernen
func RemoveMacroCommand(int iCommand)
{
	//Log("%s #%d: RemoveMacroCommand: %d", GetName(), ObjectNumber(), iCommand);
	// An der Stelle gibt es nichts?
	if (iCommand < 0 || GetLength(aMacroCommandList) <= iCommand)
		return;
	// Entfernen
	for (var i = 0, iEff, checked; i < GetEffectCount("MacroCommand", this); i++)
	{
		iEff = GetEffect("MacroCommand", this, i);
		if (iEff == aMacroCommandList[iCommand])
		{
			RemoveEffect("MacroCommand", this, i);
			continue;
		}
		if (GetEffect("MacroCommandLocked", this))
			continue;
		for (var mac in aMacroCommandList) 
			if (mac == iEff)
				checked = true;
		if (!checked)
			RemoveEffect("MacroCommand", this, i, true);
		checked = false;
	}
	/*  if(RemoveEffect(0, this(), aMacroCommandList[iCommand]))
    Log("Effect removed");*/
	var aNewMacroComList = CreateArray();
	i = 0;
	for (var iCom in aMacroCommandList) 
	{
		if (i == iCommand)
			continue;
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
	if (GetEffect("MacroCommandLocked", pTarget))
		return -1;
}


