/*-- CTF --*/

#strict
#include TEAM

local aFlagCarrier;
local bNoReturnMessage;


protected func Initialize()
{
	//Anzahl Gewinne wird durch Anzahl Aktivierungen eingestellt
	var pGoal;
	if (pGoal = FindObject(GetID()))
	{
		LocalN("iWinScore", pGoal)++;
		return(RemoveObject());
	}
	iWinScore = 1;
	aFlagCarrier = CreateArray();
	return(_inherited());
}

public func GetHUDInfo(int player, object hud) {
	
	var team = GetPlayerTeam(player);
	if(team < 1) return(inherited(player));
	var ourFlag = GetFriendlyFlag(player);
	
	// Flag stolen?
	if(ourFlag->GetCarrier()) {
		// stolen
		SetHUDGraphics(0,hud,GetID(),2,2,"FlagStolen");
	}
	else if(ourFlag->IsAtHome())
		SetHUDGraphics(0,hud,GetID(),2,2);
	else
		SetHUDGraphics(0,hud,GetID(),2,2,"FlagLoose");
		
	
	// flag captured
	if(GetFriendlyFlagCarrier(player)) {
		SetHUDGraphics(0,hud,GetID(),3,2,"FlagCaptured");
	}
	else
		SetHUDGraphics(0,hud,GetID(),3,2);
	
	return(inherited(player));
}

/* Globale Funktionen */

global func CreateFlag(int iTeam, int iX, int iY, int iColor) // Platziert eine Flagge für Team iTeam, Position und Farbe angeben
{
	var pBase = CreateObject(FLBS, AbsX(iX), AbsY(iY), -1);
	var pFlag = CreateObject(FLA2, AbsX(iX), AbsY(iY), -1);
	pFlag->Activate(pBase, iTeam, iColor);
	if(FindObject(GCTF)) FindObject(GCTF)->InitTeam(iTeam);
}

/* Geld */

// Clonks mit Flaggen sind mehr wert
public func Money(int iPlr, object pClonk, int iMurdererPlr)
{

	if(iMurdererPlr != NO_OWNER)
	{
		var extra;
		var flag = FindObject2(Find_ID(FLA2), Find_ActionTarget(pClonk));
		if(flag)
			extra = 30;

		if (pClonk)
		{
		// Teamkill! Wert des Clonks wird abgezogen
		if(GetPlayerTeam(iPlr) == GetPlayerTeam(iMurdererPlr))
			DoWealth(iMurdererPlr, - (pClonk->GetValue(0,0,0, iMurdererPlr) + extra));
		// Regulärer Kill, Wert des Clonks wird vergeben
		else
			DoWealth(iMurdererPlr, + pClonk->GetValue(0,0,0, iMurdererPlr) + extra);
		}
	}
	// Geld für den getöteten, halber Wert des Clonks wird gegeben
	if (pClonk) DoWealth(iPlr, + pClonk->GetValue(0,0,0, iPlr) / 2);
}

/* Scoreboard */

public func InitTeam(int iTeam)
{
	aFlagCarrier[iTeam] = -1;
}

public func FlagCaptured(int iTeam, int iCTeam, object pClonk)
{
	UA_Announcer(Format("an_ctf_%sflag_taken",TeamID2String(iTeam)));
	aFlagCarrier[iTeam] = GetOwner(pClonk);
	SetFlagMarker(GetOwner(pClonk), iTeam); // pClonk hat die flagge von iTeam
	UpdateHUDs();
}

public func FlagLost(int iTeam)
{
	UA_Announcer(Format("an_ctf_%sflag_dropped",TeamID2String(iTeam)));
	UpdateHUDs();
	if(aFlagCarrier[iTeam] == -1) return();
	SetFlagMarker(aFlagCarrier[iTeam], 0, true);
	aFlagCarrier[iTeam] = -1;
}

public func FlagScored(int iTeam, int iCTeam, object pClonk)
{
	bNoReturnMessage = true;
	Sound("Scored",true);
	//Sound("WinARound");

	UpdateHUDs();
	if(aFlagCarrier[iTeam] == -1) return;
	SetFlagMarker(aFlagCarrier[iTeam], 0, true);
	aFlagCarrier[iTeam] = -1;
}


public func FlagReturned(int iTeam)
{
	if(!bNoReturnMessage) UA_Announcer(Format("an_ctf_%sflag_returned",TeamID2String(iTeam)));
		bNoReturnMessage = false;
	SetFlagMarker(aFlagCarrier[iTeam], 0, true);
	aFlagCarrier[iTeam] = -1;
	UpdateHUDs();
}

private func SetFlagMarker(int iPlr, int iCTeam, bool fRemove)
{
	var iCol = TEAM_PlayerColumn, iData;
//	var iPlr = GetOwner(pPlr);
	if(GetTeamPlayerCount(GetPlayerTeam(iPlr)) == 1)
	{
		iCol = TEAM_TeamColumn;
		iData = GetPlayerTeam(iPlr);
	}
	if(!fRemove)
		SetScoreboardData(iPlr, iCol, Format("<c %x>{{FLBS}}</c> %s", GetTeamColor(iCTeam), GetTaggedPlayerName(iPlr)), iData);
	else
		SetScoreboardData(iPlr, iCol, GetTaggedPlayerName(iPlr), iData);
}

// Kann mittels des Spielzielauswählers konfiguriert werden
public func IsConfigurable() { return(1); }


// Returns the enemy flag carrier if any
public func GetEnemyFlagCarrier(int iOwner) {
	var ourFlag = GetFriendlyFlag(iOwner);
	return(ourFlag -> GetCarrier());
}

// Returns the friendly flag carrier if any. 
public func GetFriendlyFlagCarrier(int iOwner) {
	// The flag...
	for(var pFlag in FindObjects(Find_ID(FLA2))) {
		// ... that is attached to a clonk...
		if(WildcardMatch(pFlag->GetAction(), "*Attach*")) {
			var flagger = pFlag->GetActionTarget();
			var flagger_team = GetPlayerTeam(flagger->GetOwner());
			// ... which is in our team ...
			if( flagger_team == GetPlayerTeam(iOwner) ) {
				// ...his flag
				return(flagger);
			}
		}
	}
	return(0);
}

public func GetFriendlyFlag(int iOwner) {
	// The flag...
	for(var pFlag in FindObjects(Find_ID(FLA2))) {
		// that is not ours
		if( pFlag->GetTeam() == GetPlayerTeam(iOwner) ) {
			return(pFlag);
		}
	}
	return(0);
}

// Returns the flag of a random enemy
public func GetEnemyFlag(int iOwner) {
	var flags = CreateArray();
	// A flag...
	for(var pFlag in FindObjects(Find_ID(FLA2))) {
		// that is not ours
		if( pFlag->GetTeam() != GetPlayerTeam(iOwner) ) {
			flags[GetLength(flags)] = pFlag;
		}
	}
	// Choose a flag randomly
	return(flags[Random(GetLength(flags))]);
}

// Returns the own flag base
public func GetOwnFlagBase(int iOwner) {
	// The flag...
	for(var pFlag in FindObjects(Find_ID(FLA2))) {
		// ... of our team ...
		if( pFlag->GetTeam() == GetPlayerTeam(iOwner) ) {
			// ...its base
			return(pFlag->GetBase());
		}
	}
	return(0);
}

// Some constants
private func MaxFlagBaseDistance()	{ return(250); }
























public func OnRestartRound( int iPlr, int iTeam )
{
	if( iPlr == -1 )
	{
		var flags = FindObjects(Find_ID(FLA2)), obj;

		for( obj in flags) obj->~Return2Base();
	}
}

public func SelectTeamStrategy()
{
	DebugLog("Neue Team-Strategie:","tactics");
	if(GetType(aTeamAggro) != C4V_Array) aTeamAggro = [];

	var goaldiff = [];
	var teamdiff = [];

	var max = 0;
	// erstmal wählen alle Teams eine zufällige Strategie!
	for(var i = 0; i < GetTeamCount(); i++)
	{
		var j = GetTeamByIndex(i);
		var p = aGamePoints[j];
		aTeamAggro[j]=Random(3);

		goaldiff[j] = 100*(iWinScore - p)/iWinScore;

		if(max < p ) max = p;
	}
	// tja, jetzt müssen wir nochmal durch!
	for(var i = 0; i < GetTeamCount(); i++)
	{
		var j = GetTeamByIndex(i);
		var p = aGamePoints[j];
		aTeamAggro[j]=Random(3);

		teamdiff[j] = 100*(max - p)/max;

		// wir werden defensiv, wenn ein Team schon fast gewonnen hat
		for(var k = 0; k < GetTeamCount(); k++)
		if( goaldiff[GetTeamByIndex(k)] < 20 ) aTeamAggro[j]= 0;

		// wir werden aggressiv, wenn wir dem besten Team stark hinterherhinken
		if(teamdiff[j] >= 50) aTeamAggro[j]=2;

		DebugLog("-> Team %d: %d","tactics",j,aTeamAggro[j]);
	}
}


// KI Taktik
public func AITactic(object pAIPlayer)
{
	var owner = pAIPlayer->GetOwner();

	var pEFC = GetEnemyFlagCarrier(owner);
	var flag = GetFriendlyFlag(owner);
	if(!pEFC)
	if(!(flag->IsAtHome()))
		pEFC = flag;
	var pFFC = GetFriendlyFlagCarrier(owner);
	var flagbase = GetOwnFlagBase(owner);
	var eflagbase = GetEnemyFlag(owner);

	if(pEFC) DebugLog("Flag was stolen by %s (%d,%d)","tactics",GetName(pEFC),GetX(pEFC),GetY(pEFC));
	if(pFFC) DebugLog("%s (%d,%d) got the enemy flag","tactics",GetName(pFFC),GetX(pFFC),GetY(pFFC));

	var crewcount = GetCrewCount(owner);

	for(var i=0, pCrew ; pCrew = GetCrew(owner, i) ; i++)
	{
		if(pCrew->Contained()) continue;
		pCrew->CheckInventory();

		// flag carrier returns home
		if(pCrew == pFFC)
		{

			// return to own flag if own flag is not stolen or he is too far from the flag base
			if(!pEFC || ObjectDistance(pCrew,flagbase) > MaxFlagBaseDistance())
			{
				//if(/*GetCrewCount(pCrew->GetOwner())>1 ||*/ !pEFC) // auskommentiert, da hier jeder Spieler nur 1 Bot hat
				if(GetTeamPlayerCount(GetPlayerTeam(pCrew->GetOwner()))>1 || !pEFC) // wenn es mehr als 1 Spieler im Team gibt!!
				{
					pAIPlayer->Run(pCrew,flagbase,"Returning home!");
					continue;
				}
				else if(pEFC) // ich bin der einzige Spieler meines Teams, dann muss ich den feindlichen Flaggenträger jagen
				{
					pAIPlayer->Follow(pCrew,pEFC,0,"Attacking enemy flag carrier");
					continue;
				}
			}
			// own flag is stolen - ich bin der Flaggenträger und man hat meine Flagge gestohlen
			else
			{
					// I'm not the only one
					//if(GetCrewCount(pCrew->GetOwner())>1)
					if(GetTeamPlayerCount( GetPlayerTeam(owner) ) > 1)
					{
						pAIPlayer->Guard(pCrew,"Guarding our flag base!"); // passt :)
						continue;
					}
					// however, if I'm the only one, I need to perhaps follow the enemy flagger...
					else if(pEFC)
					{
						pAIPlayer->Follow(pCrew,pEFC,0,"Attacking enemy flag carrier");
						continue;
					}
			}
		}
		else // ich bin nicht der Flaggenträger meines Teams
		{
			var intercept = 0;
			// flag was stolen. Everyone who can intercept, does that
			if(pEFC)
			{
				intercept = pAIPlayer->Interceptor(pCrew,eflagbase,pEFC);
				 if(intercept)
				 {
					pAIPlayer->Follow(pCrew,pEFC,0,"Intercepting enemy flag carrier!");
					continue;
				}
			}
			if(!intercept) // Hier kann also die ganz normale Routine rein, mit Rollenunterscheidungen
			{
				var role = GetPlayerAIRole(owner);
				// friendly flag carrier around
				if(pFFC && !pEFC)
				{
					var follower = pAIPlayer->Interceptor(pCrew,flagbase,pFFC);
					// if I am a sniper, then I don't leave my spot!!
					if( role == ROLE_Sniper) follower = 0;
					// between flagger and own flag: follow flagger
					if(follower)
					{
						pAIPlayer->Follow(pCrew,pFFC,0,"Escorting friendly flag carrier!");
						continue;
					}
				}

				if( role == ROLE_Assault || role == ROLE_Support)
				{
					// else just go to enemy flag/enemy flag carrier
					if(!pEFC)
					{
						if(!(pCrew->GetCommand() || pCrew->GetMacroCommand()))
						{
							pCrew->SetMacroCommand(this(), "MoveTo",	eflagbase,0,0,0,Aggro_Follow);
							DebugMessage("@Going to enemy flag base.", "tactics");
						}
					}
					else
					{
						pAIPlayer->Follow(pCrew,pEFC,0,"Hunting down enemy flag carrier!");
					}
				}
				/*else if( role == ROLE_Defend )
				{
					if(!(pCrew->GetCommand() || pCrew->GetMacroCommand()))
					{
						if( ObjectDistance(pCrew,flagbase) > MaxFlagBaseDistance() )
						{
							pCrew->SetMacroCommand(this(), "MoveTo",	flagbase,0,0,0,Aggro_Shoot);
							DebugMessage("@Going to enemy flag base.", "tactics");
						}
						else
						{
							pAIPlayer->Guard(pCrew,"Guarding our flag base!"); // passt :)
						}
					}
				}
				else if( role == ROLE_Sniper )
				{
					Message("I'm the Sniper and I have nothing to do",pCrew);
				}*/
				// Also, die Sache ist so: der Sniper tat das, was der Defender tun sollte,
				// nämlich in der Nähe der Basis rumlaufen -> sehr gut
				// der Defender stand einfach nur rum (logischerweise) im Guardmodus,
				// also machte er das, was der Sniper tun soll :D
				else if( role == ROLE_Defend )
				{
					//Message("Defending!!",pCrew);
					if(!(pCrew->GetCommand() || pCrew->GetMacroCommand()))
					{
						var wps = FindObjects(Find_ID(WAYP), Find_Func("CheckInfo",WPInfo_Defend,GetPlayerTeam(owner)));
						var wp = wps[Random(GetLength(wps))];
						DebugLog("Found defending points %v","tactics",wps);

						//Message("@**!**",wp);

						if(wp) pCrew->SetMacroCommand(this(), "MoveTo",	wp,0,0,0,Aggro_Follow);
						DebugMessage("@Going to a random guard point", "tactics");
					}
				}
				else if( role == ROLE_Sniper )
				{
						DebugLog("I'm the Sniper and I have nothing to do","tactics");//,pCrew);
						/*if(!(pCrew->GetCommand() || pCrew->GetMacroCommand()))
						{
							if( ObjectDistance(pCrew,flagbase) > MaxFlagBaseDistance() )
							{
								pCrew->SetMacroCommand(this(), "MoveTo",	flagbase,0,0,0,Aggro_Shoot);
								DebugMessage("@Going to enemy flag base.", "tactics");
							}
							else
							{
								pAIPlayer->Guard(pCrew,"Guarding our flag base!"); // passt :)
							}
						}*/
						if(!(pCrew->GetCommand() || pCrew->GetMacroCommand()))
						{
							pAIPlayer->Sniper(pCrew);
						}
				}
			}
		}
	}
}
