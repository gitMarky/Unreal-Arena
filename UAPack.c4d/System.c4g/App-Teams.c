/*-- Teams --*/

#strict
#appendto TEAM

static const TEAM_TeamColumn = 0;
static const TEAM_PlayerColumn = 1;
static const TEAM_GoalColumn = 2;
static const TEAM_KillColumn = 3;
static const TEAM_DeathColumn = 4;

static const TEAM_KillColumn2 = 5;
static const TEAM_DeathColumn2 = 6;

// 1024 Spieler werden maximal für das Scoreboard unterstützt!
static const TEAM_iRow = 1024;
static const ROUND_iRow = 1090; // Und maximal 64 Teams (und ein paar zerquetschte? ^^)

local init, fNoTeamCheck;
local evaluation;

global func TeamID2String(int iTeam)
{
	if(iTeam == 1) return "r";
	if(iTeam == 2) return "b";
	return 0;
}

protected func Activate(iPlr) // Defaultnachrichten
{
	if (IsFulfilled())
	{
		if(aGamePoints[GetPlayerTeam(iPlr)] >= iWinScore)
			return(MessageWindow("$MsgGoalFulfilled$",iPlr));
		else
			return(MessageWindow("$MsgGoalLost$",iPlr));
	}
	return(MessageWindow(Format("$MsgGoalUnfulfilled$", GetWinScore() - aGamePoints[GetPlayerTeam(iPlr)]),iPlr));
}

// Variablen zum Speichern des Punktestandes

static aGamePoints;
static aGamesWon;
static aTeamAggro;

// Siegpunktzahl

static iWinScore;

// Spielerdinge

static aPlrRace;
static aPlrClass;
static aPlrFrags;
static aPlrDeaths; 
static aPlrSpecial; // Welches Special ist gewählt
static aPlrHead;
static aPlrRole; // KI-Rolle des Spielers

protected func Initialize()
{

	/*if(GetType(aGamePoints) != C4V_Array)*/ aGamePoints = CreateArray();
	if(GetType(aGamesWon) != C4V_Array) aGamesWon = CreateArray();
	if(GetType(aPlrFrags) != C4V_Array) aPlrFrags = CreateArray();
	if(GetType(aPlrDeaths) != C4V_Array) aPlrDeaths = CreateArray();
	if(GetType(aPlrRole) != C4V_Array) aPlrRole = CreateArray();
	if(GetType(aTeamAggro) != C4V_Array) aTeamAggro = CreateArray();
	SelectTeamStrategy();
	ScheduleCall(this(),"InitScoreboard",1);
	return(_inherited());
}

protected func InitializePlayer(int iPlr, int iX, int iY, object pBase, int iTeam)
{
	if(!init) return();

	if(!aGamePoints[iTeam])	aGamePoints[iTeam] = 0; // sehr sinnvoll, ich frage mich, für was das gut war ^^

	// Rolle aussuchen
	if(GetPlayerType(iPlr) == C4PT_User)
		aPlrRole[iPlr] = ROLE_Player;
	else
		SelectAIRole(iPlr);

	//Verfeindung setzen
	Hostility(iPlr);
	// Ins Scoreboard eintragen

	var teamp = GetTeamPlayerCount(iTeam);

	/*
	// Team ist neu
	if(teamp == 1)
		InitSingleplayerTeam(iPlr);
	// Team wechselt von Einzelspieler auf Mehrspieler
	if(teamp == 2)
	{
		InitMultiplayerTeam(iTeam);
		RemoveSingleplayerTeam(iPlr);
		var j = 0;
		while(GetTeamPlayer(iTeam, ++j) != -1)
		{
			InitPlayer(GetTeamPlayer(iTeam, j));
		}
	}
	// Spieler normal einfügen
	if(teamp > 2)
		InitPlayer(iPlr);*/
	if(teamp == 1)
		InitMultiplayerTeam(iTeam);

	InitPlayer(iPlr);

	// Sortieren
	SortTeamScoreboard();
}

//Die Initialisierung des Scoreboards. Damit alle Spalten/Zeilen etc. angezeigt werden
private func InitScoreboard()
{

	//Überschriften
	SetScoreboardData(SBRD_Caption, SBRD_Caption, Format("$Score$"), SBRD_Caption);
	SetScoreboardData(SBRD_Caption, TEAM_TeamColumn, "{{KILC}}", SBRD_Caption);
	SetScoreboardData(SBRD_Caption, TEAM_PlayerColumn, "{{HZCK}}", SBRD_Caption);
	SetScoreboardData(SBRD_Caption, TEAM_GoalColumn, Format("{{%i}}",GetID()), SBRD_Caption);
	SetScoreboardData(SBRD_Caption, TEAM_KillColumn, "{{PIWP}}", SBRD_Caption);
	SetScoreboardData(SBRD_Caption, TEAM_DeathColumn, "{{KAMB}}", SBRD_Caption);
	SetScoreboardData(SBRD_Caption, TEAM_KillColumn2, "{{PIWP}}", SBRD_Caption);
	SetScoreboardData(SBRD_Caption, TEAM_DeathColumn2, "{{KAMB}}", SBRD_Caption);

	var teams = CreateArray(), team;

	//Daten
	//erstmal rausfinden welche Teams wir anzeigen
	for(var i = 0 ; i < GetPlayerCount() ; i++)
	{
		if(!GetPlayerName(GetPlayerByIndex(i)))
			continue;

		var done=0, plrteam = GetPlayerTeam(GetPlayerByIndex(i));
		if(plrteam < 1) continue;

		for(var team in teams)
			if(team == plrteam)
				done++;
		if(!done)
		{
			teams[GetLength(teams)] = plrteam;
		}
	}

	//so, jetzt wissen wir die Teams. Jetzt noch nach Teamnummern ordnen. %(
	//Bubblesort!
	var t1, t2;
	for(t1 = 0; t1 < GetLength(teams); t1++) {
		for(t2 = t1; t2 < GetLength(teams); t2++)
			if(teams[t2] < teams[t1])
			{
				var tmp = teams[t1];
				teams[t1] = teams[t2];
				teams[t2] = tmp;
			}
	}
	//puh, ganz schön viel Aufwand für sowas..
	//aber jetzt müssen wir nur noch darstellen. :)
	for(var x=0; x < GetLength(teams); x++)
	{
		var t = teams[x];
		// Team hat keinen Spieler -> Höh?
		if(!GetTeamPlayerCount(t)) continue;
		// Team hat nur einen Spieler -> "Einzelspielerteam"
		if(GetTeamPlayerCount(t) == 1)
		{
			InitSingleplayerTeam(GetTeamPlayer(t, 1));
			continue;
		}
		// Team einfügen, 1024 Spieler werden maximal unterstützt!
		InitMultiplayerTeam(t);
		// Spieler aus dem Team einfügen
		var j = 0;
		while(GetTeamPlayer(t, ++j) != -1)
		{
			InitPlayer(GetTeamPlayer(t, j));
		}
	}

	InitRoundInfo();

	// Sortieren
	SortTeamScoreboard();
	// Fertig
	init = true;
}

/* Scoreboard */

// Funktionen zum Eintragen

// Die Überschrift für Gesamtsiege pro Runde machen und so Zeug

private func InitRoundInfo()
{
	SetScoreboardData(ROUND_iRow, SBRD_Caption, "");
	SetScoreboardData(ROUND_iRow, TEAM_TeamColumn,	"$RoundsWon$", -1);
	SetScoreboardData(ROUND_iRow, TEAM_PlayerColumn, "");
	SetScoreboardData(ROUND_iRow, TEAM_GoalColumn,	"{{CTFL}}", -1);
	SetScoreboardData(ROUND_iRow, TEAM_KillColumn,	"", 0);
	SetScoreboardData(ROUND_iRow, TEAM_DeathColumn, "", 0);
	SetScoreboardData(ROUND_iRow, TEAM_KillColumn2,	"", 0);
	SetScoreboardData(ROUND_iRow, TEAM_DeathColumn2, "", 0);
}

// Team eintragen, dass mehrere Spieler hat
private func InitMultiplayerTeam(int iTeam)
{
	if(iTeam < 1) return();

	var col = GetTeamColor(iTeam);

	SetScoreboardData(TEAM_iRow + iTeam, SBRD_Caption, "");
	SetScoreboardData(TEAM_iRow + iTeam, TEAM_TeamColumn,	Format("<c %x>%s</c>", col, GetTeamName(iTeam)), iTeam);
	SetScoreboardData(TEAM_iRow + iTeam, TEAM_PlayerColumn, "");
	SetScoreboardData(TEAM_iRow + iTeam, TEAM_GoalColumn,	Format("<c %x>%d / %d</c>", col, aGamePoints[iTeam], iWinScore), aGamePoints[iTeam]);
	// aktuelle Kills
	SetScoreboardData(TEAM_iRow + iTeam, TEAM_KillColumn,	Format("<c %x>%d</c>", col, TeamGetKills(iTeam,true)), TeamGetKills(iTeam,true)+1);
	SetScoreboardData(TEAM_iRow + iTeam, TEAM_DeathColumn, Format("<c %x>%d</c>", col, TeamGetDeath(iTeam,true)));
	// Gesamt-Kills (alle Runden)
	SetScoreboardData(TEAM_iRow + iTeam, TEAM_KillColumn2,	Format("<c %x>%d</c>", col, TeamGetKills(iTeam,false)), TeamGetKills(iTeam,false)+1);
	SetScoreboardData(TEAM_iRow + iTeam, TEAM_DeathColumn2, Format("<c %x>%d</c>", col, TeamGetDeath(iTeam,false)));

	SetScoreboardData(ROUND_iRow + iTeam, SBRD_Caption, "");
	SetScoreboardData(ROUND_iRow + iTeam, TEAM_TeamColumn,	Format("<c %x>%s</c>", col, GetTeamName(iTeam)));
	SetScoreboardData(ROUND_iRow + iTeam, TEAM_PlayerColumn, "");
	SetScoreboardData(ROUND_iRow + iTeam, TEAM_GoalColumn,	Format("<c %x>%d</c>", col, aGamesWon[iTeam]), -1);
	SetScoreboardData(ROUND_iRow + iTeam, TEAM_KillColumn,	"",aGamesWon[iTeam]);
	SetScoreboardData(ROUND_iRow + iTeam, TEAM_DeathColumn, "");
	SetScoreboardData(ROUND_iRow + iTeam, TEAM_KillColumn2, "");
	SetScoreboardData(ROUND_iRow + iTeam, TEAM_DeathColumn2, "");
	SortTeamScoreboard();
}

// Mehrspielerteam austragen
private func RemoveMultiplayerTeam(int iTeam)
{
	if(iTeam < 1) return();

	SetScoreboardData(TEAM_iRow + iTeam, TEAM_TeamColumn);
	SetScoreboardData(TEAM_iRow + iTeam, TEAM_PlayerColumn);
	SetScoreboardData(TEAM_iRow + iTeam, TEAM_GoalColumn);
	SetScoreboardData(TEAM_iRow + iTeam, TEAM_KillColumn);
	SetScoreboardData(TEAM_iRow + iTeam, TEAM_DeathColumn);
	SetScoreboardData(TEAM_iRow + iTeam, TEAM_KillColumn2);
	SetScoreboardData(TEAM_iRow + iTeam, TEAM_DeathColumn2);

	SetScoreboardData(ROUND_iRow + iTeam, TEAM_TeamColumn);
	SetScoreboardData(ROUND_iRow + iTeam, TEAM_PlayerColumn);
	SetScoreboardData(ROUND_iRow + iTeam, TEAM_GoalColumn);
	SetScoreboardData(ROUND_iRow + iTeam, TEAM_KillColumn);
	SetScoreboardData(ROUND_iRow + iTeam, TEAM_DeathColumn);
	SetScoreboardData(ROUND_iRow + iTeam, TEAM_KillColumn2);
	SetScoreboardData(ROUND_iRow + iTeam, TEAM_DeathColumn2);

	SortTeamScoreboard();
}

// Team eintragen, dass nur einen Spieler hat
private func InitSingleplayerTeam(int iPlr)
{
	if(iPlr == -1) return();

	var col = GetPlrColorDw(iPlr);

	SetScoreboardData(iPlr, SBRD_Caption, "");
	SetScoreboardData(iPlr, TEAM_TeamColumn, GetTaggedPlayerName(iPlr), GetPlayerTeam(iPlr));
	SetScoreboardData(iPlr, TEAM_PlayerColumn, "");
	SetScoreboardData(iPlr, TEAM_GoalColumn,	Format("<c %x>%d / %d</c>", col, aGamePoints[GetPlayerTeam(iPlr)],iWinScore), aGamePoints[GetPlayerTeam(iPlr)]);
	SetScoreboardData(iPlr, TEAM_KillColumn,	Format("<c %x>%d</c>", col, TeamGetKills(GetPlayerTeam(iPlr),true)), TeamGetKills(GetPlayerTeam(iPlr),true)+1);
	SetScoreboardData(iPlr, TEAM_DeathColumn, Format("<c %x>%d</c>", col, aDeath[iPlr]), aDeath[iPlr]);
	SetScoreboardData(iPlr, TEAM_KillColumn2,	Format("<c %x>%d</c>", col, TeamGetKills(GetPlayerTeam(iPlr),false)), TeamGetKills(GetPlayerTeam(iPlr),true)+1);
	SetScoreboardData(iPlr, TEAM_DeathColumn2, Format("<c %x>%d</c>", col, aPlrDeaths[iPlr]), aPlrDeaths[iPlr]);

	SortTeamScoreboard();
}

// Einzelspielerteam austragen
private func RemoveSingleplayerTeam(int iPlr)
{
	if(iPlr == -1) return();

	SetScoreboardData(iPlr, TEAM_TeamColumn);
	SetScoreboardData(iPlr, TEAM_PlayerColumn);
	SetScoreboardData(iPlr, TEAM_GoalColumn);
	SetScoreboardData(iPlr, TEAM_KillColumn);
	SetScoreboardData(iPlr, TEAM_DeathColumn);
	SetScoreboardData(iPlr, TEAM_KillColumn2);
	SetScoreboardData(iPlr, TEAM_DeathColumn2);
	SortTeamScoreboard();
}

// Spieler eintragen
private func InitPlayer(int iPlr)
{
	// Team 0 oder -1 können wir nicht behandeln
	if(GetPlayerTeam(iPlr) < 1) return();
	if(iPlr == -1) return();

	// Eintragen
	SetScoreboardData(iPlr, SBRD_Caption, "");
	SetScoreboardData(iPlr, TEAM_TeamColumn, Format("<c %x>\\</c>",GetTeamColor(GetPlayerTeam(iPlr))), GetPlayerTeam(iPlr));
	SetScoreboardData(iPlr, TEAM_PlayerColumn, GetTaggedPlayerName(iPlr));
	SetScoreboardData(iPlr, TEAM_GoalColumn, "", aGamePoints[GetPlayerTeam(iPlr)]);
	SetScoreboardData(iPlr, TEAM_KillColumn, Format("%d", aKill[iPlr]), aKill[iPlr]);
	SetScoreboardData(iPlr, TEAM_DeathColumn, Format("%d", aDeath[iPlr]), aDeath[iPlr]);
	SetScoreboardData(iPlr, TEAM_KillColumn2, Format("%d", aPlrFrags[iPlr]), aPlrFrags[iPlr]);
	SetScoreboardData(iPlr, TEAM_DeathColumn2, Format("%d", aPlrDeaths[iPlr]), aPlrDeaths[iPlr]);
	SortTeamScoreboard();
}

// Spieler austragen
private func RemoveScoreboardPlayer(int iPlr)
{
	if(iPlr == -1) return();

	if(GetTeamPlayerCount(GetPlayerTeam(iPlr)) == 1) return(RemoveSingleplayerTeam(iPlr));

	SetScoreboardData(iPlr, TEAM_TeamColumn);
	SetScoreboardData(iPlr, TEAM_PlayerColumn);
	SetScoreboardData(iPlr, TEAM_GoalColumn);
	SetScoreboardData(iPlr, TEAM_KillColumn);
	SetScoreboardData(iPlr, TEAM_DeathColumn);
	SetScoreboardData(iPlr, TEAM_KillColumn2);
	SetScoreboardData(iPlr, TEAM_DeathColumn2);

	SortTeamScoreboard();

	// Team nicht prüfen
	if(fNoTeamCheck) return();
	fNoTeamCheck = true;

	// Team wechselt zum Einzelspielerteam
	if(GetTeamPlayerCount(GetPlayerTeam(iPlr)) == 2) // 2, weil der Spieler in diesem Moment noch im Spiel ist
	{
		var j = 0, p;
		while(GetTeamPlayer(GetPlayerTeam(iPlr), ++j) != -1)
		{
			p = GetTeamPlayer(iPlr, j);
			if(p == iPlr) continue;
			//Log("%d", p);
//			RemovePlayer(p, true);
			RemoveMultiplayerTeam(GetPlayerTeam(iPlr));
			InitSingleplayerTeam(p);
		}
	}
	fNoTeamCheck = false;
}

// Engine-Call
public func RemovePlayer(int iPlr)
{
	if(iPlr == -1) return();

	// Auswertungsdialog
	AddEvaluationData(Format("{{PIWP}}$Kills$: %d", aPlrFrags[iPlr]), GetPlayerID(iPlr));
	AddEvaluationData(Format("{{KAMB}}$Death$: %d", aPlrDeaths[iPlr]), GetPlayerID(iPlr));

	RemoveScoreboardPlayer(iPlr);
	aPlrDeaths[iPlr] = 0;
	aPlrFrags[iPlr] = 0;
}

// Sortieren
private func SortTeamScoreboard()
{
	// Nach Toden (Spieler)
	SortScoreboard(TEAM_DeathColumn);
	// Nach Kills (Spieler)
	SortScoreboard(TEAM_KillColumn, true);
	// Nach Teams (Spieler)
	SortScoreboard(TEAM_TeamColumn);
	// Nach Punkten (Teams)
	SortScoreboard(TEAM_GoalColumn, true);
}

/* Helferlein */

// Schaut, wie viele Spieler in einem Team sind
global func GetTeamPlayerCount(int iTeam)
{
	for(var i = GetPlayerCount()-1,j ; i > -1 ; i--)
		if(GetPlayerTeam(GetPlayerByIndex(i)) == iTeam)
			j++;
	return(j);
}

// Liefert nacheinander die einzelnen Spieler eines Teams (Parameter 1 - x)
global func GetTeamPlayer(int iTeam, int iNo)
{
	for(var i = GetPlayerCount()-1,j ; i > -1 ; i--)
		if(GetPlayerTeam(GetPlayerByIndex(i)) == iTeam)
			if(!--iNo)
				return(GetPlayerByIndex(i));
	return(-1);

}

// Liefert alle Kills eines Teams
private func TeamGetKills(int iTeam, bool bLocal)
{
	for(var i = GetPlayerCount()-1,j ; i > -1 ; i--)
		if(GetPlayerTeam(GetPlayerByIndex(i)) == iTeam)
		{
			if(!bLocal)
				j += aPlrFrags[GetPlayerByIndex(i)];
			else
				j += aKill[GetPlayerByIndex(i)];
		}
	return(j);
}

// Liefert alle Death eines Teams
// *copy'n'paste + -Kill +Death!
private func TeamGetDeath(int iTeam, bool bLocal)
{
	for(var i = GetPlayerCount()-1,j ; i > -1 ; i--)
		if(GetPlayerTeam(GetPlayerByIndex(i)) == iTeam)
		{
			if(!bLocal)
				j += aPlrDeaths[GetPlayerByIndex(i)];
			else
				j += aDeath[GetPlayerByIndex(i)];
		}
	return(j);
}

//aktualisiert den Punktestand
private func UpdateScoreboard(int iTeam)
{

	if(GetTeamPlayerCount(iTeam) > 1)
	{
		SetScoreboardData(TEAM_iRow + iTeam, TEAM_GoalColumn, Format("<c %x>%d / %d</c>", GetTeamColor(iTeam), aGamePoints[iTeam], iWinScore), aGamePoints[iTeam]);
		SetScoreboardData(TEAM_iRow + iTeam, TEAM_KillColumn,	Format("<c %x>%d</c>", GetTeamColor(iTeam), TeamGetKills(iTeam,true)), TeamGetKills(iTeam,true)+1);
		SetScoreboardData(TEAM_iRow + iTeam, TEAM_DeathColumn, Format("<c %x>%d</c>", GetTeamColor(iTeam), TeamGetDeath(iTeam,true)));
		SetScoreboardData(TEAM_iRow + iTeam, TEAM_KillColumn2,	Format("<c %x>%d</c>", GetTeamColor(iTeam), TeamGetKills(iTeam,false)), TeamGetKills(iTeam,false)+1);
		SetScoreboardData(TEAM_iRow + iTeam, TEAM_DeathColumn2, Format("<c %x>%d</c>", GetTeamColor(iTeam), TeamGetDeath(iTeam,false)));

		SetScoreboardData(ROUND_iRow + iTeam, TEAM_GoalColumn, Format("<c %x>%d</c>", GetTeamColor(iTeam), aGamesWon[iTeam]), -1);
		SetScoreboardData(ROUND_iRow + iTeam, TEAM_KillColumn,	"", aGamesWon[iTeam]);
		SetScoreboardData(ROUND_iRow + iTeam, TEAM_DeathColumn, "");
		SetScoreboardData(ROUND_iRow + iTeam, TEAM_KillColumn2,	"");
		SetScoreboardData(ROUND_iRow + iTeam, TEAM_DeathColumn2, "");

		var p, j=0;
		while(GetTeamPlayer(iTeam, ++j) != -1)
		{
			p = GetTeamPlayer(iTeam, j);
			SetScoreboardData(p, TEAM_GoalColumn, "", aGamePoints[iTeam]);
		}
	}
 	else
	{
	 		SetScoreboardData(GetTeamPlayer(iTeam, 1), TEAM_GoalColumn, Format("<c %x>%d / %d</c>", GetPlrColorDw(GetTeamPlayer(iTeam, 1)), aGamePoints[iTeam], iWinScore), aGamePoints[iTeam]);
	 		SetScoreboardData(GetTeamPlayer(iTeam, 1), TEAM_KillColumn,	Format("<c %x>%d</c>", GetPlrColorDw(GetTeamPlayer(iTeam, 1)), TeamGetKills(iTeam,true)), TeamGetKills(iTeam,true)+1);
	 		SetScoreboardData(GetTeamPlayer(iTeam, 1), TEAM_DeathColumn, Format("<c %x>%d</c>", GetPlrColorDw(GetTeamPlayer(iTeam, 1)), TeamGetDeath(iTeam,true)));
	 		SetScoreboardData(GetTeamPlayer(iTeam, 1), TEAM_KillColumn2,	Format("<c %x>%d</c>", GetPlrColorDw(GetTeamPlayer(iTeam, 1)), TeamGetKills(iTeam,false)), TeamGetKills(iTeam,false)+1);
	 		SetScoreboardData(GetTeamPlayer(iTeam, 1), TEAM_DeathColumn2, Format("<c %x>%d</c>", GetPlrColorDw(GetTeamPlayer(iTeam, 1)), TeamGetDeath(iTeam,false)));
	}

	SortTeamScoreboard();
}

public func OnTeamSwitch( int iPlr, int iNewTeam, int iOldTeam )
{
	InitPlayer( iPlr ); // Der Team-Wechsel muss ja erkannt werden
	SortTeamScoreboard();
}

public func RejectTeamSwitch( int iPlr, int iNewTeam )
{
	if(GetTeamPlayerCount(GetPlayerTeam(iPlr))<=1) return(true);
}

private func Hostility(int iPlr)
{
	for(var x; x < GetPlayerCount(); x++)
		if(GetPlayerByIndex(x) && iPlr != x) {
			if(GetPlayerTeam(iPlr) != GetPlayerTeam(GetPlayerByIndex(x)))
				SetHostility(iPlr, GetPlayerByIndex(x), 1, 1);
			else
				SetHostility(iPlr, GetPlayerByIndex(x), 0, 1);
		}
}

public func Destruction()
{
	//Scoreboard löschen
	for(var x=1; GetTeamName(x); x++) {
		var iColID = 1;
		SetScoreboardData(x,			 SBRD_Caption, 0);
		SetScoreboardData(x,			 iColID,			 0);
	}
}

/* Aufrufe */

public func WinScoreChange(int iNewScore)
{
	iWinScore = iNewScore;
}

public func ScoreChange(int iTeam, int iChange, bool fRound)
{
	if( fRound )
	{
		aGamesWon[iTeam] += iChange;
	}
	else
	{
		aGamePoints[iTeam] += iChange;

		if(aGamePoints[iTeam] >= GetWinScore(this))
		{
			DoTeamRoundScore( iTeam, 1);
		}
		else
		{
			SelectTeamStrategy();
		}

	}
	UpdateScoreboard(iTeam);
}

public func IsFulfilled() // Siegreiches Team?
{
/*
	var team;
	for(var iScore in aGamePoints) {
		if(iScore >= iWinScore)
		{
			EliminateLosers();
			Message("@<c %x>$WinMsg$</c>",0,GetTeamColor(team),GetTeamName(team));
			//Leben noch Verlierer? -> Ligainkompatibilität!
			if(LosersAlive())
				return(0);

			// Auswertungsdialog
			if(!evaluation)
			for(var i=0 ; i < GetPlayerCount() ; i++)
			{
				var plr = GetPlayerByIndex(i);
				AddEvaluationData(Format("{{PIWP}}$Kills$: %d", aPlrFrags[plr]), GetPlayerID(plr));
				AddEvaluationData(Format("{{KAMB}}$Death$: %d", aPlrDeaths[plr]), GetPlayerID(plr));
				
			}
			evaluation = true;

			return(1);
		}
		team++;
 }
*/
}

private func EliminateLosers()	// Hier trennt sich die Spreu vom Weizen
{
	for(var i=0 ; i < GetPlayerCount() ; i++)
		if(aGamePoints[GetPlayerTeam(GetPlayerByIndex(i))] < iWinScore)
			EliminatePlayer(GetPlayerByIndex(i)); // HARHAR
}

private func LosersAlive()
{
	for(var x = 0 ; x < GetPlayerCount() ; x++)
		if(aGamePoints[GetPlayerTeam(GetPlayerByIndex(x))] < iWinScore)
			if(GetPlayerByIndex(x) >= 0)
				return(1);

	return(0);
}

/* Globale Funktionen */

global func DoTeamScore(int iTeam, int iChange)
{
	for(var pGoal in FindObjects(Find_Func("IsTeamGoal")))
		pGoal->~ScoreChange(iTeam, iChange, false);

	UA_Announcer(Format("an_%steam_scores",TeamID2String(iTeam)));

}

global func DoTeamRoundScore(int iTeam, int iChange)
{
	for(var pGoal in FindObjects(Find_Func("IsTeamGoal")))
		pGoal->~ScoreChange(iTeam, iChange, true);

	Sound("WinARound",true);
	UA_Announcer(Format("an_%steam_winner",TeamID2String(iTeam)));
	GameCallEx("OnWinRound",iTeam);
	RestartRound();
}

global func SetWinScore(int iNewScore, object pGoal)
{
	if(pGoal)
	{
		 pGoal->~WinScoreChange(iNewScore);
		return(0);
	}
	for(var goal in FindObjects(Find_Func("IsTeamGoal")))
		goal->~WinScoreChange(iNewScore);
}

global func GetWinScore(object pGoal)
{
	return(LocalN("iWinScore", pGoal));
}


/* Status */

public func IsTeamGoal() { return(1); }

/* Dolle Funktionen */

global func GetPlrRace(int iPlr){ return( aPlrRace[iPlr] ); }
global func GetPlrClass(int iPlr){ return( aPlrClass[iPlr] ); }
global func GetPlrFrags(int iPlr){ return( aPlrFrags[iPlr] ); }
global func GetPlrDeaths(int iPlr){ return( aPlrDeaths[iPlr] ); }
global func GetPlrSpecial(int iPlr){ return( aPlrSpecial[iPlr] ); }
global func GetPlrHead(int iPlr){ return( aPlrHead[iPlr] ); }
global func GetTeamScore( int iTeam ){ return(aGamePoints[iTeam]); }

global func SetPlrRace(int iPlr, int iSet){ aPlrRace[iPlr]=iSet; }
global func SetPlrClass(int iPlr, int iSet){ aPlrClass[iPlr]=iSet; }
global func SetPlrFrags(int iPlr, int iSet){ aPlrFrags[iPlr]=iSet; }
global func SetPlrDeaths(int iPlr, int iSet){ aPlrDeaths[iPlr]=iSet; }
global func SetPlrSpecial(int iPlr, int iSet){ aPlrSpecial[iPlr]=iSet; }
global func SetPlrHead(int iPlr, int iSet){ aPlrHead[iPlr]=iSet; }

private func TeamGetScore(int iTeam){ return GetTeamScore(iTeam); }


global func ChangePlrFrags(int iPlr, int iSet)
{
	aPlrFrags[iPlr]+=iSet;

	var obj;
	if( obj=FindObject2(Find_Func("IsTeamGoal"))) obj->~UpdatePlayerStats( iPlr );
}
global func ChangePlrDeaths(int iPlr, int iSet)
{
	aPlrDeaths[iPlr]+=iSet;

	var obj;
	if( obj=FindObject2(Find_Func("IsTeamGoal"))) obj->~UpdatePlayerStats( iPlr );
}



global func CreateBots()
{
	var bots = g_iPlayerCount-GetPlayerCount();
	var i,obj,team_count;

	team_count = CreateArray();

	//for( obj in FindObjects( Find_ID( _CSC)))
	//{
	//	team_count[ obj->~GetTeam() ]++;
	//}

	for(var k = 0; k < GetPlayerCount(); k++)
	{
		team_count[ GetPlayerTeam(GetPlayerByIndex(k)) ]++;
	}

	DebugLog("Verteile Bot-Teams: Team0 %d, Team1 %d","teams",team_count[1],team_count[2]);

	while( i++ < bots)
	{
		var team_sel = 1;

		if( team_count[1] >= team_count[2] ) team_sel = 2;

		DebugLog("Selektiere Bot-Teams: Team1 %d, Team2 %d, Auswahl %d","teams",team_count[1],team_count[2], team_sel);

		CreateScriptPlayer(GetUAName(),RGB(50+Random(205),50+Random(205),50+Random(205)),team_sel);

		team_count[team_sel]++;

	}

	return(1);
}


global func GetUAName()
{
	var i =Random(25);

	if( i == 0 ) return("Satin");
	if( i == 1 ) return("Saphire");
	if( i == 2 ) return("Diva");
	if( i == 3 ) return("Domina");
	if( i == 4 ) return("Wraith");
	if( i == 5 ) return("Frostbite");
	if( i == 6 ) return("Blackjack");
	if( i == 7 ) return("Scarecrow");
	if( i == 8 ) return("Malcolm");
	if( i == 9 ) return("Loque");
	if( i == 10 ) return("Lilith");
	if( i == 11 ) return("Lauren");
	if( i == 12 ) return("Rylissa");
	if( i == 13 ) return("Agony");
	if( i == 14 ) return("Mortalis");
	if( i == 15 ) return("Brutalis");
	if( i == 16 ) return("Romeo");
	if( i == 17 ) return("Vick");
	if( i == 18 ) return("Brock");
	if( i == 19 ) return("Cleopatra");
	if( i == 20 ) return("Seth");
	if( i == 21 ) return("Visse");
	if( i == 22 ) return("Sarena");
	if( i == 23 ) return("Kragoth");
	if( i == 24 ) return("Necris");

	// female
	/*
	Commando:
		Ivana
		Nikita
		Gromida
	Mercenary:
		Kyla
		Mariana
		Jayce
		Tanya
	Necirs
		Cryss
		Freyla
		Visse
		Malise
	Aphex
		Idina
		Portia
	Soldier
		Lauren
		Rylisa
		Sara
		Shyann
	Metal Guard
		Isis
		Risa
		Tasha
		Vixen
	Venom
		Athena
		Cilia
		Sarena
		Tara
	Marine
		Azure
		Aryss
		Annaka
		Olga
		Ryanna
	War Machine
		Cathode
		Fury
		Lilith
	 */
	// male
	/*
	Commando
		Blake
		Gorn
		Nickolai
		Whitman
	Merc
		Boris
		Luthor
		Ramirez
		Graves
	Necris
		Kragoth
		Malakai
		Necrotic
		Grail
	Marine
		Malcolm
		Othello
		Rike
	Metal Guard
		Drake
		Radkin
		Wraith
		Von
	RawSteel
		Arkon
		Bruce
		Kregore
		Manwell
		Slain
	Soldier
		Brock
		Harlin
		Johnson
		Rankin
	War Machine
		Matrix
		Vector
		Tensor
	 */
	return("Bot");
}

public func UpdatePlayerStats( int iPlr )
{
	if(iPlr == -1 || !GetPlayerName(iPlr)) return();
	
	if(GetTeamPlayerCount(GetPlayerTeam(iPlr)) > 1)
	{
		SetScoreboardData(iPlr, TEAM_DeathColumn, Format("%d", aDeath[iPlr]), aDeath[iPlr]);
		SetScoreboardData(iPlr, TEAM_DeathColumn2, Format("%d", aPlrDeaths[iPlr]), aPlrDeaths[iPlr]);
	}
	else
	{
		SetScoreboardData(iPlr, TEAM_DeathColumn, Format("<c %x>%d</c>", GetPlrColorDw(iPlr), aDeath[iPlr]), aDeath[iPlr]);
		SetScoreboardData(iPlr, TEAM_DeathColumn, Format("<c %x>%d</c>", GetPlrColorDw(iPlr), aPlrDeaths[iPlr]), aPlrDeaths[iPlr]);
	}
	if(GetTeamPlayerCount(GetPlayerTeam(iPlr)) > 1)
	{
		SetScoreboardData(iPlr, TEAM_KillColumn, Format("%d", aKill[iPlr]), aKill[iPlr]);
		SetScoreboardData(iPlr, TEAM_KillColumn2, Format("%d", aPlrFrags[iPlr]), aPlrFrags[iPlr]);
	}
	else
	{
		SetScoreboardData(iPlr, TEAM_KillColumn, Format("<c %x>%d</c>", GetPlrColorDw(iPlr), aKill[iPlr]), aKill[iPlr]);
		SetScoreboardData(iPlr, TEAM_KillColumn2, Format("<c %x>%d</c>", GetPlrColorDw(iPlr), aPlrFrags[iPlr]), aPlrFrags[iPlr]);
	}

	UpdateScoreboard(GetPlayerTeam(iPlr));
}




public func RelaunchPlayer(int iPlr, object pClonk, int iMurdererPlr)
{
	if(iPlr == -1 || !GetPlayerName(iPlr)) return();

	UpdateHUDs();

	//Tode++
	aDeath[iPlr]++; 	// lokal, für die Runde, wird für LMS etc. gebraucht
	aPlrDeaths[iPlr]++;	// global, für die Statistik
	if(GetTeamPlayerCount(GetPlayerTeam(iPlr)) > 1)
	{
		SetScoreboardData(iPlr, TEAM_DeathColumn, Format("%d", aDeath[iPlr]), aDeath[iPlr]);
		SetScoreboardData(iPlr, TEAM_DeathColumn2, Format("%d", aPlrDeaths[iPlr]), aPlrDeaths[iPlr]);
	}
	else
	{
		SetScoreboardData(iPlr, TEAM_DeathColumn, Format("<c %x>%d</c>", GetPlrColorDw(iPlr), aDeath[iPlr]), aDeath[iPlr]);
		SetScoreboardData(iPlr, TEAM_DeathColumn2, Format("<c %x>%d</c>", GetPlrColorDw(iPlr), aPlrDeaths[iPlr]), aPlrDeaths[iPlr]);
	}
	if(GetTeamPlayerCount(GetPlayerTeam(iPlr)) > 1)
	{
		SetScoreboardData(TEAM_iRow + GetPlayerTeam(iPlr), TEAM_DeathColumn, Format("<c %x>%d</c>", GetTeamColor(GetPlayerTeam(iPlr)), TeamGetDeath(GetPlayerTeam(iPlr),true)), TeamGetDeath(GetPlayerTeam(iPlr),true)+1);
		SetScoreboardData(TEAM_iRow + GetPlayerTeam(iPlr), TEAM_DeathColumn2, Format("<c %x>%d</c>", GetTeamColor(GetPlayerTeam(iPlr)), TeamGetDeath(GetPlayerTeam(iPlr),false)), TeamGetDeath(GetPlayerTeam(iPlr),false)+1);
	}
	//kein Selfkill? kein Teamkill?
	if(iMurdererPlr != -1 && GetPlayerTeam(iPlr) != GetPlayerTeam(iMurdererPlr))
	{
		//kill++
		aKill[iMurdererPlr]++;
		aPlrFrags[iMurdererPlr]++;
		SetScoreboardData(iMurdererPlr, TEAM_KillColumn, Format("<c %x>%d</c>", GetPlrColorDw(iMurdererPlr), aKill[iMurdererPlr]), aKill[iMurdererPlr]);
		SetScoreboardData(iMurdererPlr, TEAM_KillColumn2, Format("<c %x>%d</c>", GetPlrColorDw(iMurdererPlr), aPlrFrags[iMurdererPlr]), aPlrFrags[iMurdererPlr]);
		//team: kill++
		if(GetTeamPlayerCount(GetPlayerTeam(iMurdererPlr)) > 1)
		{
			SetScoreboardData(TEAM_iRow + GetPlayerTeam(iMurdererPlr), TEAM_KillColumn, Format("<c %x>%d</c>", GetTeamColor(GetPlayerTeam(iMurdererPlr)), TeamGetKills(GetPlayerTeam(iMurdererPlr),true)), TeamGetKills(GetPlayerTeam(iMurdererPlr),true)+1);
			SetScoreboardData(iMurdererPlr, TEAM_KillColumn, Format("%d", aKill[iMurdererPlr]), aKill[iMurdererPlr]);

			SetScoreboardData(TEAM_iRow + GetPlayerTeam(iMurdererPlr), TEAM_KillColumn2, Format("<c %x>%d</c>", GetTeamColor(GetPlayerTeam(iMurdererPlr)), TeamGetKills(GetPlayerTeam(iMurdererPlr),false)), TeamGetKills(GetPlayerTeam(iMurdererPlr),false)+1);
			SetScoreboardData(iMurdererPlr, TEAM_KillColumn2, Format("%d", aPlrFrags[iMurdererPlr]), aPlrFrags[iMurdererPlr]);
}
	}
	SortTeamScoreboard();
	// Geld
	Money(iPlr, pClonk, iMurdererPlr);
}


public func ChooserFinished()
{
	UpdateHUDs();
}

public func SelectTeamStrategy()
{
}

// Rollen können z.B. beim Klassensystem gemixt werden
// so kann die KI sinnvolle Klassen nach ihrer Rolle im Team auswählen
static const ROLE_Player = 0; // menschliche Spieler machen eh was sie wollen, denen kann man leider keine Rolle geben
static const ROLE_Assault = 1;
static const ROLE_Support = 2;
static const ROLE_Sniper = 4;
static const ROLE_Defend = 8;

// Sagt dem KI-Spieler, wie viele Bots was tun sollen
public func TeamQuota( iTeam, iType )
{
	var attitude = aTeamAggro[iTeam];

	if( iType==0 ) return 10;

	if(attitude == 2) // Agressiv
	{
		if(iType==ROLE_Assault) return(3); // Assault
		if(iType==ROLE_Support) return(5); // Support
		if(iType==ROLE_Sniper) return(1); // Sniper
		if(iType==ROLE_Defend) return(1); // Defend
	}
	if(attitude == 1) // Moderat
	{
		if(iType==ROLE_Assault) return(2); // Assault
		if(iType==ROLE_Support) return(4); // Support
		if(iType==ROLE_Sniper) return(2); // Sniper
		if(iType==ROLE_Defend) return(2); // Defend
	}
	if(attitude == 0) // Defensiv
	{
		if(iType==ROLE_Assault) return(2); // Assault
		if(iType==ROLE_Support) return(2); // Support
		if(iType==ROLE_Sniper) return(3); // Sniper
		if(iType==ROLE_Defend) return(3); // Defend
	}
}


public func SelectAIRole( int iPlr )
{
	aPlrRole[iPlr] = ROLE_Assault;

	var amount,assault,support,defend,sniper,team = GetPlayerTeam(iPlr);
	var plrs = GetTeamPlayerCount(team);

	amount = TeamQuota(team,0);
	assault = TeamQuota(team,ROLE_Assault);
	support = TeamQuota(team,ROLE_Support);
	defend = TeamQuota(team,ROLE_Defend);
	sniper = TeamQuota(team,ROLE_Sniper);

	var role;
	if(GetRoleCount(ROLE_Sniper,team) < Max(1,sniper*plrs/amount) && !role ) role = ROLE_Sniper;
	if(GetRoleCount(ROLE_Defend,team) < Max(1,defend*plrs/amount) && !role ) role = ROLE_Defend;
	if(GetRoleCount(ROLE_Support,team) < Max(1,support*plrs/amount) && !role ) role = ROLE_Support;
	//if( !role ) role = ROLE_Assault;

	if( role ) aPlrRole[iPlr] = role;

	DebugLog("I chose role: %d, S %d D %d S %d A %d, C %d T %d","teams",aPlrRole[iPlr],sniper*plrs/amount,
																		  defend*plrs/amount,
																		  support*plrs/amount,
																		  assault*plrs/amount,
																		  plrs,team);

}

public func GetRoleCount( int iRole, int iTeam )
{
	var j = 0;
	for(var i = 0; i<GetPlayerCount(); i++)
	{
		if(aPlrRole[GetPlayerByIndex(i)] == iRole && (!iTeam || GetPlayerTeam(GetPlayerByIndex(i))== iTeam)) j++;
	}

	return j;
}

global func GetPlayerAIRole( int iPlr )
{
	if(GetType(aPlrRole) != C4V_Array) return 0;
	return aPlrRole[iPlr];
}
