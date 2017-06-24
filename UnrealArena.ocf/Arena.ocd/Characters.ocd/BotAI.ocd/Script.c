
// Include the basic functionality
#include AI_Controller
// Include the different parts of the AI.
#include AI_Debugging
#include AI_HelperFunctions
#include AI_Protection
#include AI_TargetFinding
#include AI_HelperClonk
#include AI_HomePosition
#include AI_AttackEnemy
// Main execution logic
#include AI_TaskExecutor
// Other stuff
#include Library_MacroCommand

///////////////////////////////////////////////////////////////////////////////////////////////////
//
// Implementation

/* -- Callbacks -- */

public func OnAddAI(proplist controller)
{
	_inherited(controller);

	Task_BotAggression->AddTo(controller.Target);
}


///////////////////////////////////////////////////////////////////////////////////////////////////
//
// legacy code below: App-ClonkAI.C

/*-- Neues Script --*/


global func BotSkill(i)
{
	if (i == -1) return 20;
	return Min(i, 20);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Überladenes Hazard-Zeug
// die Wegfindung kann man z.B. auch fuer das normale Hazard verwenden



// Hier wurde nur der Timer geändert
func SetAggroLevel(int iLevel, int iDist, int iX, int iY, string text)
{
	if (iLevel > 3)
		return;
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
	if (!iLevel)
		return RemoveEffect("Aggro", this);
	// 1, 2 und 3
	var effect = GetEffect("Aggro", this);
	if (!effect)
		effect = AddEffect("Aggro", this, 50, 20, this); // Timer war 10;
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



func FxAggroTimer(object pTarget, proplist no)
{
	// Doppelsprung ausführen, wenn sinnvoll
	if (WildcardMatch(GetAction(), "Jump*"))
		if (GBackSolid(-20 + GetDir() * 40))
			this->~ControlUp();
	
	// Je schlechter der Botskill, desto �fter machen wir hier Pause
	if (Random(8 - BotSkill(7)))
		return;
	
	// Kugeln ausweichen
	UTBotAIDodgeBullets();
	
	var target = no.var1;
	
	if (target && !PathFree(GetX(), GetY(), target->GetX(), target->GetY()))
		target = 0;
	
	// Wir haben ein Ziel?
	if (target)
	{
		EffectCall(this, no, "Fire");
		// ein Ausweichsprung
		if (ObjectDistance(no.var1) < 25 && this->~CheckEnemy(no.var1, this, false))
			UTBotAIDodgeJump();
		return;
	}
	else
	{
		// Waffe soll zu schie�en aufh�ren
		if (Contents())
			Contents()->~StopAutoFire();
	}
	
	var weaprange = 400;
	if (Contents())
		weaprange = Contents()->~GetBotData(BOT_Range);
	
	target = UTBotAIFindEnemy(this, Max(400, weaprange));
	
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
		return;
	}
	// Super
	no.var1 = target;
	no.var99 = true; // wir haben ein Ziel \o/}
}

func FxAggroFire(object pTarget, proplist no)
{
	// Zusatzhack: BR-Bombe!
	if (Contents()->GetID() == GBRB)
		// Nichts tun :C
		return;
	// Nichts tun, wenn gerade verhindert
	if (!this->~ReadyToFire())
		return;
	var y = no.var4;
	var x = no.var3;
	var dist = no.var2;
	var target = no.var1;
	var level = no.var0;
	var weaprange = 0;
	
	if (Contents())
		weaprange = Contents()->~GetBotData(BOT_Range);
	

	// Fahrzeugsteuerung
	if (Contained())
	{
		if (Contained()->~HandleAggro(this, level, target, dist, x, y))
			return 1;
		else
			return AddCommand("Exit", nil, 0, 0, nil, 0, 0, 0, C4CMD_SilentSub);
	}
	
	// Ziel irgendwie weg?
	// (Pathfree wurde schon gecheckt)
	if (!this->~CheckTarget(target, this, Max(dist, weaprange), 0, 0, true))
	{
		no.var1 = 0;
		if (no.var0 == 2)
			ClearMacroCommands();
		if (Contents())
			Contents()->~StopAutoFire();
		return;
	}
	// Ich hab nix?
	if (!Contents())
		return; // Lauf, Forest, lauf!
	// Waffe in die Hand nehmen
	if (!SelectWeapon(level, target, false))
	{
		// Bei Aggro_Follow k�nnen wir von unserem Pfade weg. D.h. eine Waffe und/oder Munition muss her
		if (GetAggroLevel() == Aggro_Follow)
		{
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
	

	var pWeapon = Contents();
	
	// Gut. Feuern wir bereits?
	if (pWeapon->IsRecharging())
		return;
	
	// Er schie�t nicht immer, wenn er kann
	var efficiency = pWeapon->~GetBotData(BOT_Rating);
	if (Random(efficiency - BotSkill(efficiency - 1)))
		return;
	
	UTBotAIAdjustDir(target);
	
	// Feuer!
	// Stufe 2 - verfolgen!
	if (no.var0 >= 2)
		if (GetMacroCommand(1) != "Follow" || GetMacroCommand(1, 1) != target)
			if (GetMacroCommand(0) != "Follow" || GetMacroCommand(0, 1) != target)
			{
				DebugLog("FxAggroFire - Adding Follow command", "aggro");
				AddMacroCommand(nil, "MoveTo", nil, GetX(), GetY(), 0, level);
				AddMacroCommand(nil, "Follow", target, 0, 0, 0, level);
			}
	


	UTBotAIAimAt(target);
	// Waffe benutzen
	var override = pWeapon->~GetBotData(BOT_Override);
	if (override)
	{
		pWeapon->~UTBotAIUse(this, target);
		return;
	}
	

	if (Inside
	(
		ObjectDistance(pTarget),
		pWeapon->~GetBotData(BOT_RangeMin, 2),
		pWeapon->~GetBotData(BOT_Range, 2)
	))
	{
		UTBotAIFire(pWeapon, 2);
	}
	else
	{
		UTBotAIFire(pWeapon, 1);
	}
	
	return;
}

// Wenn iLevel = 1 (Aggro_Shoot) werden keine Waffen mit FM_Aim ausgewählt
func SelectWeapon(int iLevel, object pTarget, bool fFireModes)
{
	var arsenal, distance, selection, i, prio;
	
	distance = ObjectDistance(pTarget);
	
	arsenal = FindObjects(Find_Container(this), Find_Func("IsWeapon"));
	
	for (i = 0; i < GetLength(arsenal); i++)
	{
		if (distance <= (arsenal[i]->~GetBotData(BOT_Range)))
			if ((prio < (arsenal[i]->~GetBotData(BOT_Priority, 1))) && (ObjectDistance(pTarget) >= (arsenal[i]->~GetBotData(BOT_Priority, 1))))
			{
				prio = arsenal[i]->~GetBotData(BOT_Priority, 1); //BotInfoPriority();
				
				selection = arsenal[i];
			}
	}
	
	if (selection)
		ShiftContents(false, selection->GetID());
	return Contents();
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





// Das ganze Hazard-Ding ist ja mal ziemlich Banane gewesen, mal sehen, ob ich hier einen guten
// Algorithmus einbauen kann. Wir machen das jetzt mit Dijkstra - Floyd-Warshall kann zwar negative
// Kantengewichte und so, aber das kommt hier eh nicht vor, außerdem müsste man da erstmal eine
// Matrix aufstellen.

// gibt Pfad von Wegpunkt pStart bis pEnd als Array aus. An Stelle 0 ist pStart.
// wenn FindPath = 0, hat er keinen Pfad gefunden.
func FindPath(object pStart, object pEnd, bool fJetpack)
{
	var pCurrent = pStart;
	
	var aNodes = []; // das Array, in welchem die Wegpunkte gespeichert werden
	var aDistance = []; // dieses Array gibt an, wie weit ich zum Wegpunkt laufen muss
	// könnte ausschließlich mit ObjectDistance() gemacht werden
	// stattdessen erlauben wir auch, dass Teleporter oder Jumppads
	// eine geringere "Distanz" haben, weil sie den Clonk schneller
	// ans Ziel bringen
	var aPrev = []; // die Vorgänger, daraus bauen wir einen Pfad zusammen
	var bPath = []; // aNodes hilft nur für die Nachbarschaft, bPath ist der gewählte Pfad
	var aSet = []; // Suchmenge
	
	var jetp = 0;
	var ammoload = JTPK->GetFMData(FM_AmmoLoad);
	
	// Initialisierung: Startpunkt in die Knotenmenge, Label ist 0
	aNodes = FindObjects(Find_ID(WAYP));
	aSet = aNodes;
	for (var i = 0; i < GetLength(aNodes); i++)
		aDistance[i] = -1;
	aDistance[GetIndexOf(aNodes, pCurrent)] = 0;
	
	// jetzt der eigentliche Algorithmus
	var bEnd = false;
	while (GetLength(aSet) && (!bEnd))
	{
		
		var aBest = [], pNode, iBest = -1;
		
		// besten Knoten auswählen
		for (pNode in aSet) 
		{
			var dist = aDistance[GetIndexOf(aNodes, pNode)];
			//var dist = aDistance[i];
			if (dist < 0)
				continue;
			if (iBest == -1 || dist < iBest)
			{
				iBest = dist;
				DebugLog("Least Distance = %d (Obj %d)", "dijkstra", iBest, aSet[i]->ObjectNumber());
			}
		}
		for (pNode in aSet) 
		{
			if (aDistance[GetIndexOf(aNodes, pNode)] == iBest)
				PushBack(pNode, aBest);
		}
		DebugLog("Best Nodes: %v", "dijkstra", aBest);
		pCurrent = aBest[Random(GetLength(aBest))];
		
		if (!pCurrent) break;
		
		RemoveArrayValue(aSet, pCurrent);
		
		var pNext = 0;
		var pathcount = pCurrent->GetPathCount();
		var iCurrent = GetIndexOf(aNodes, pCurrent);
		
		// alle Nachbarknoten des besten Knotens nach k�rzeren Wegen absuchen
		for (var i = 0; i < pathcount; ++i)
		{
			pNext = (pCurrent->GetPathTarget(i));
			var iNext = GetIndexOf(aNodes, pNext);
			
			if (!Check4Jetpack(pCurrent, i, fJetpack, jetp, ammoload))
			{
				DebugLog
				(
					"   Checking Path %d - Waypoint %d: Needs Jetpack",
					"dijkstra",
					i,
					pNext->ObjectNumber()
				);
				continue;
			}
			

			// jetzt die Distanz updaten
			var distnew = aDistance[iCurrent] + pCurrent->~GetPathLengthWP(i);
			DebugLog
			(
				"   Checking Path %d - Waypoint %d: new %d, old %d",
				"dijkstra",
				i,
				pNext->ObjectNumber(),
				distnew,
				aDistance[iNext]
			);
			
			if (aDistance[iNext] < 0 || aDistance[iNext] > distnew)
			{
				aDistance[iNext] = distnew;
				aPrev[iNext] = pCurrent;
			}
			
			if (pNext == pEnd)
			{
				DebugLog("Found Endpoint", "dijkstra");
				bEnd = true;
				break;
			}
		}
	}
	
	// er findet keinen Weg dorthin, leider
	if (!bEnd)
		return false;
	
	// Jetzt den Weg aufbauen
	bPath = [pEnd];
	pCurrent = pEnd;
	while (pCurrent = aPrev[GetIndexOf(aNodes, pCurrent)])
	{
		PushFront(pCurrent, bPath);
	}
	
	return bPath;
}



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Eigenes Zeug, braucht man nicht für Hazard


func UTBotAIDodgeBullets()
{
	if (Random(20 - BotSkill(0)))
		return;
	if (!UTBotAIFindDodgeBullet())
		return;
	DebugLog("FoundDodgeBullet", "hazard");
	UTBotAIDodgeJump();
}


func UTBotAIDodgeJump()
{
	if (GetProcedure() == "FLIGHT")
		this->~ControlUp();
	else
		Jump();
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


func UTBotAIFindDodgeBullet()
{
	var obj;
	
	var bullets = FindObjects(Find_InRect(-180, -60, 360, 120), Find_Func("IsBullet"));
	
	for (obj in bullets) 
	{
		if (this->~CheckEnemy(obj, this))
			return obj;
	}
	
	return 0;
}

func UTBotAIFindEnemy(object pExclude, int iRadius)
{
	var obj;
	
	var enemy = this->~UTBotAIFindEnemies(pExclude, iRadius, false);
	
	for (obj in enemy) 
	{
		return obj;
	}
	
	return 0;
}

func UTBotAIFire(object pWeapon, int iMode)
{
	if (!pWeapon)
		return;
	pWeapon->SetFireMode(iMode);
	pWeapon->~ControlThrow(this); //Control2Contents("ControlThrow")
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

func UTBotAIAimAt(object target, int xmod, int ymod)
{
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
}

