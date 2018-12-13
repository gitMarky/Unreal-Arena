
// Include the basic functionality
#include AI_Controller
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

	controller->SetAgent(AI_Agent_UA); // sets the agent logic

	Behaviour_Aggression->AddTo(controller.Target);
	Behaviour_DoubleJump->AddTo(controller.Target);
	Behaviour_Protection->AddTo(controller.Target);

	Task_GetWeapon->AddTo(controller.Target, 1);
}


private func QueryRejectCallExecute(proplist controller, object agent, int time)
{
	if (_inherited(controller, agent, time)) return true;
	if (!RoundManager()->IsRoundActive()) return true;
	if (agent->Contained() && agent->Contained()->GetID() == RelaunchContainerEx) return true;
	return false;
}


///////////////////////////////////////////////////////////////////////////////////////////////////
//
// legacy code below: App-ClonkAI.C

/*-- Neues Script --*/


global func BotSkill(int i)
{
	if (i == -1) return 20;
	return Min(i, 20);
}


global func DelayBotBySkill(int delay, int skill)
{
	delay = delay ?? 20;
	skill = skill ?? delay;
	return Random(1 + delay - BotSkill(skill));
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Überladenes Hazard-Zeug
// die Wegfindung kann man z.B. auch fuer das normale Hazard verwenden




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
