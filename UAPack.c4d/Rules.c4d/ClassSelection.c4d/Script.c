/*-- No Blood --*/

#strict 2

global func MOD_ClassSelection(){ return ObjectCount( ID_Rule_ClassSelection ); }

protected func Activate(iPlr)
{
  MessageWindow(GetDesc(),iPlr);
}

private func InfoMenuItems(){ return 5; }

// Kann mittels des Spielzielauswählers ausgewählt werden
public func IsChooseable() { return true; }
public func RuleForbid(){ return [ IGIB ];}
// den darf man mal wieder nich abbrechen
public func MenuQueryCancel(){return  true;}



// arrays mit Spielerinfos
//local aAgility, aAccuracy, aVitality, aVoice, aBlood, aGender;
static aAgility, aAccuracy, aVitality, aVoice, aBlood, aGender;

static aTeamClass;

local aCrew;
local aLastClass;
local iMenuSelection;
local aMenuSelection;
local aClassName;

local bNoMenuUpdate;

/* --- Hilfen ------------------------------------------------------------------------------------------------------ */

global func GetClassManager()
{
	if(GetID() == ID_Rule_ClassSelection) return this;
	return FindObject(ID_Rule_ClassSelection);
}

global func CrewGetClass( object pCrew )	{ return PlrGetClass(GetOwner(pCrew)); }
global func CrewGetAgility( object pCrew )	{ return PlrGetAgility(GetOwner(pCrew)); }
global func CrewGetVitality( object pCrew )	{ return PlrGetVitality(GetOwner(pCrew)); }
global func CrewGetAccuracy( object pCrew )	{ return PlrGetAccuracy(GetOwner(pCrew)); }
global func CrewGetClassName( object pCrew ){ return PlrGetClassName(GetOwner(pCrew)); }
global func CrewGetVoice( object pCrew )	{ return PlrGetVoice(GetOwner(pCrew)); }
global func CrewGetGender( object pCrew )	{ return PlrGetGender(GetOwner(pCrew)); }
global func CrewGetBlood( object pCrew )	{ return PlrGetBlood(GetOwner(pCrew)); }
/*
global func PlrGetClass( int iPlr )		{ if(!GetClassManager()) return -1; return LocalN("aLastClass",GetClassManager())[iPlr]; }
global func PlrGetAgility( int iPlr  )	{ if(!GetClassManager()) return 0;  return LocalN("aAgility",GetClassManager())[iPlr]; }
global func PlrGetVitality( int iPlr  )	{ if(!GetClassManager()) return 0;  return LocalN("aVitality",GetClassManager())[iPlr]; }
global func PlrGetAccuracy( int iPlr ) 	{ if(!GetClassManager()) return 0;  return LocalN("aAccuracy",GetClassManager())[iPlr]; }
global func PlrGetClassName( int iPlr  ){ if(!GetClassManager()) return 0;  return GetClassManager()->~GetCData( PlrGetClass(iPlr), CData_Name ); }
global func PlrGetVoice( int iPlr  )	{ if(!GetClassManager()) return 0;  return Format("%s%s", LocalN("aGender",GetClassManager())[iPlr], LocalN("aVoice",GetClassManager())[iPlr]); }
global func PlrGetGender( int iPlr  )	{ if(!GetClassManager()) return 0;  return LocalN("aGender",GetClassManager())[iPlr]; }
global func PlrGetBlood( int iPlr  )	{ if(!GetClassManager()) return 0;  return LocalN("aBlood",GetClassManager())[iPlr]; }
*/
global func PlrGetClass( int iPlr )		{ if(!GetClassManager()) return -1; return LocalN("aLastClass",GetClassManager())[iPlr]; }
global func PlrGetAgility( int iPlr  )	{ return aAgility[iPlr]; }
global func PlrGetVitality( int iPlr  )	{ return aVitality[iPlr]; }
global func PlrGetAccuracy( int iPlr ) 	{ return aAccuracy[iPlr]; }
global func PlrGetClassName( int iPlr  ){ if(!GetClassManager()) return 0;  return GetClassManager()->~GetCData( PlrGetClass(iPlr), CData_Name ); }
global func PlrGetVoice( int iPlr  )	{ return Format("%s%s", aGender[iPlr], aVoice[iPlr]); }
global func PlrGetGender( int iPlr  )	{ return aGender[iPlr]; }
global func PlrGetBlood( int iPlr  )	{ return aBlood[iPlr]; }


/* --- Initialisierung, Respawn usw. ------------------------------------------------------------------------------------------------------ */

protected func Initialize()
{
	SetPosition(10,10);
	aCrew = [];
	aLastClass = [];
	iMenuSelection = [];
	aMenuSelection = [];
	aClassName = [];

	aAccuracy = [];
	aAgility = [];
	aVitality = [];
	aVoice = [];
	aBlood = [];
	aGender = [];

}


func InitializePlayer(int iPlayer)
{
	var pClonk = GetCrew(iPlayer);
	if(!pClonk)
	{
		ScheduleCall(0,"InitializePlayer",1,0,iPlayer);
		return;
	}

	if(FindObject(CHOS))
		return;

	ScheduleCall(0,"InitClassMenu",1,0,pClonk);
}

func ChooserFinished()
{
	ScheduleCall(0,"InitPlayers",1);
}

func InitPlayers()
{
	for(var i = 0; i < GetPlayerCount(); i++)
		InitializePlayer(GetPlayerByIndex(i));
}

public func RelaunchPlayer(int iPlr, object pClonk)
{
	// Klasse zurücksetzen
	//if(GetType(aClassName) == C4V_Array)
	//	aClassName[iPlr]=0;

	//if(GetType(aLastClass) == C4V_Array)
	//	aLastClass[iPlr]=0;

	if(!pClonk)
    if(!(pClonk = GetCrew(iPlr)))
    	return ScheduleCall(this,"RelaunchPlayer",1,0,iPlr,pClonk);
	if(!GetAlive(pClonk))
		return ScheduleCall(this,"RelaunchPlayer",1,0,iPlr);

	//Menü zeitverzögert erstellen
	ScheduleCall(0,"InitClassMenu",10,0,pClonk);

	return;
}


/* --- Initialisierung, Respawn usw. ------------------------------------------------------------------------------------------------------ */


func InitClassMenu(object pClonk)
{
	//Kein Clonk?
	if(!pClonk) return;

	var iPlayer = GetOwner(pClonk);

	aCrew[iPlayer] = pClonk;

	//Clonk in Spawnpoint verschieben
	/*if(GetID(Contained(pClonk)) != TIM1)
	{
		var tmp = CreateObject(TIM1, AbsX(GetX(pClonk)), AbsY(GetY(pClonk)), iPlayer);
		SetCategory(GetCategory(tmp) | C4D_Foreground, tmp);
		SetGraphics(0, tmp, GetID(pClonk), 1, GFXOV_MODE_Object, 0, 1, pClonk);
		if(FindObject(GOCC))
			SetVisibility(VIS_None, tmp);
		else
			SetVisibility(VIS_Owner, tmp);
		Enter(tmp,pClonk);
	}*/

	//Zeitbegrenzung bei LMS, DM und AS
	//if(FindObject(GLMS) || FindObject(GTDM) || FindObject(GASS))
	//	AddEffect("Spawntimer", this, 100, 35, this, GetID(), iPlayer, pClonk, tmp);

	//Bereits ein Menü offen?
	if(GetMenu(pClonk)) CloseMenu(pClonk); //Menü schließen

	OpenClassMenu(pClonk);
}

public func ReOpenClassMenu( object pClonk )
{
	var iPlr = GetOwner(pClonk);
	// Klasse zurücksetzen
	if(GetType(aClassName) == C4V_Array)
		aClassName[iPlr]=0;

	if(GetType(aLastClass) == C4V_Array)
		aLastClass[iPlr]=0;

	OpenClassMenu( pClonk );
}

private func OpenClassMenu(object pClonk, int iSelection)
{
	var iOwner = GetOwner(pClonk);

	// mit fixen Teams eine Vorauswahl treffen
	if(MOD_FixedTeams())
	{
		if(GetType(aTeamClass) != C4V_Array) aTeamClass = [];

			var last = aTeamClass[GetPlayerTeam(iOwner)];
			if(!last) last = 1;
			if(!aLastClass[iOwner]) aLastClass[iOwner]=last;

			aClassName[iOwner]=GetCData(last, CData_Name, iOwner );
			OnClassSelectionTemp( pClonk, last );

			//if(aClassName[iOwner] && aClassName[iOwner] != GetCData(aLastClass[iOwner],CData_Name))
			//	return SetupClass(aLastClass[iOwner], iOwner, 0);

	}
	//else
	//{
		if(aClassName[iOwner] && aClassName[iOwner] != GetCData(aLastClass[iOwner],CData_Name))
			return SetupClass(aLastClass[iOwner], iOwner, 0);
	//}

	//Auswahl updaten
	if (!iSelection && aLastClass[iOwner] > 0)
		iSelection = aLastClass[iOwner] + InfoMenuItems();
	else
		iSelection = InfoMenuItems() + 1;
	if (GetMenu(pClonk))
		iSelection = GetMenuSelection(pClonk);

	//Die Klassen durchgehen - welche sind Verfügbar, welche nicht, etc.
	if(!aMenuSelection[iOwner] || aMenuSelection[iOwner] != C4V_Array)
	{
		aMenuSelection[iOwner] = [iSelection - InfoMenuItems()];
	}

	var i = 0;
	var j = 0;
	while(GetCData(++i, CData_Name))
	{
		if (GetCData(i, CData_Condition, iOwner))
		{
			aMenuSelection[iOwner][j]=i;
			j++;
		}
	}

	// Klasse für Beschreibung selektieren
	var iClass;

		iClass = aMenuSelection[iOwner][BoundBy(iSelection - InfoMenuItems()-1,0,GetLength(aMenuSelection[iOwner])-1)];

	//Log("Menu Selections: %v %d",aMenuSelection,iClass);

	//var iClass = iSelection - InfoMenuItems();

	//Menü öffnen
	CloseMenu(pClonk);
	CreateMenu(GetID(), pClonk, this, 0, 0, 0, C4MN_Style_Dialog, true);

	//Icon
	AddMenuItem(" | ", 0, GetCData(iClass, CData_Icon), pClonk, 0, 0, " ", 2, GetCData(iClass, CData_Facet));

	//Name
	AddMenuItem(Format("<c ffff33>%s</c>|%s", GetCData(iClass, CData_Name), GetCData(iClass, CData_Desc)), 0, NONE, pClonk, 0, 0, " ");

	//Leerzeile
	//AddMenuItem(" ", 0, NONE, pClonk, 0, 0, " ");

	//Clonktyp
	//AddMenuItem(Format("{{%i}} %s", GetCData(iClass, CData_Clonk), GetName(0, GetCData(iClass, CData_Clonk)), 0, NONE, pClonk, 0, 0, " "));

	//Stats
	var iStat, iStatZero, szStatName, szStatIcons, szStatSymbol;
	szStatSymbol = "*";

	szStatName = "{{FRFS}}";
	szStatIcons = "";
	iStat = GetCData(iClass, CData_Agility);
	iStatZero = 10-iStat;
	while(iStat)	 { szStatIcons = Format("%s%s",szStatIcons,ColorizeString(szStatSymbol,RGB(255,0,0))); iStat--; }
	while(iStatZero) { szStatIcons = Format("%s%s",szStatIcons,szStatSymbol); iStatZero--; }

	AddMenuItem(Format("   %s  %s",szStatName,szStatIcons), 0, NONE, pClonk, 0, 0, " ");

	szStatName = "{{GLWP}}";
	szStatIcons = "";
	iStat = GetCData(iClass, CData_Accuracy);
	iStatZero = 10-iStat;
	while(iStat)	 { szStatIcons = Format("%s%s",szStatIcons,ColorizeString(szStatSymbol,RGB(255,0,0))); iStat--; }
	while(iStatZero) { szStatIcons = Format("%s%s",szStatIcons,szStatSymbol); iStatZero--; }

	AddMenuItem(Format("   %s  %s",szStatName,szStatIcons), 0, NONE, pClonk, 0, 0, " ");

	szStatName = "{{HARM}}";
	szStatIcons = "";
	iStat = GetCData(iClass, CData_Vitality);
	iStatZero = 10-iStat;
	while(iStat)	 { szStatIcons = Format("%s%s",szStatIcons,ColorizeString(szStatSymbol,RGB(255,0,0))); iStat--; }
	while(iStatZero) { szStatIcons = Format("%s%s",szStatIcons,szStatSymbol); iStatZero--; }

	AddMenuItem(Format("   %s  %s",szStatName,szStatIcons), 0, NONE, pClonk, 0, 0, " ");


	//Ausrüstung
	/*var szItems = "", aItems = GetCData(iClass, CData_Items), nextline = false, first = true;
	for (var aEntry in aItems)
	{
		if (GetType(aEntry) != C4V_Array || GetType(aEntry[0]) != C4V_C4ID)
			continue;
		szItems = Format("%s%2dx {{%i}}     ", szItems, aEntry[1], aEntry[0]);
		//Nach jedem zweiten Item umbrechen, außer beim letzten
		if (!first && (nextline = !nextline) && GetIndexOf(aEntry, aItems) < GetLength(aItems) - 1)
			szItems = Format("%s|", szItems);
		first = false;
	}
	AddMenuItem(szItems, 0, NONE, pClonk, 0, 0, " ");

	//Granaten
	var szGrenades = "", aGrenades = GetCData(iClass, CData_Grenades);
	for (var aEntry in aGrenades)
	{
		if (GetType(aEntry) != C4V_Array || GetType(aEntry[0]) != C4V_C4ID || !aEntry[0]->~IsGrenade())
			continue;
		szGrenades = Format("%s%2dx {{%i}}     ", szGrenades, aEntry[1], aEntry[0]);
	}
	AddMenuItem(szGrenades, 0, NONE, pClonk, 0, 0, " ");*/

	//Leerzeile
	AddMenuItem(" ", 0, NONE, pClonk, 0, 0, " ");

	//Die Klassen
	var i = 0;
	var j = 0;
	for( i in aMenuSelection[iOwner])//while(GetCData(++i, CData_Name))
	{
		var szName = GetCData(i, CData_Name);
		//if (!GetCData(i, CData_Condition, iOwner))
		//	szName = Format("<c 777777>%s</c>", szName);
		//else
		//{
			szName = Format("<c ffff33>%s</c>", szName);

		//	aMenuSelection[iOwner][j]=i;
			AddMenuItem(szName, Format("SetupClass(%d, %d, %d)", i, iOwner, j), GetCData(i, CData_Icon), pClonk, 0, 0, 0, 2, GetCData(i, CData_Facet));
		//	j++;
		//}

	}

	if(!bNoMenuUpdate && iSelection >= 0)
	{
		bNoMenuUpdate = true;
		SelectMenuItem(iSelection, pClonk);
	}

	if(GetPlayerType( iOwner) == C4PT_Script)
	{
		var class = 0, class_selection = [], role = GetPlayerAIRole(iOwner);
		//SetupClass(aMenuSelection[iOwner][ Random(GetLength(aMenuSelection[iOwner])) ],iOwner,j);

		// Sammle Klassen, die zu meiner Rolle passen
		for(class in aMenuSelection[iOwner])
			if(role & GetCData(CData_AIRole,class))
				PushBack(class,class_selection);

		if(GetLength(class_selection))
			SetupClass(class_selection[ Random(GetLength(class_selection)) ],iOwner,j);
		else
			SetupClass(aMenuSelection[iOwner][ Random(GetLength(aMenuSelection[iOwner])) ],iOwner,j);
	}

	return true;
}

protected func OnMenuSelection(int iIndex, object pClonk)
{
	iMenuSelection[GetOwner(pClonk)] = iIndex;
	if(bNoMenuUpdate)
		bNoMenuUpdate = false;
	else
		OpenClassMenu(pClonk,iIndex);
}



/* Klassen */

static const CData_Name             = 1; // Name, der im Menu angezeigt wird
static const CData_Desc             = 2; // Beschreibungstext
static const CData_Clonk            = 3; // ID des Clonks
static const CData_Agility          = 4; // Stats
static const CData_Accuracy         = 5; // Stats
static const CData_Vitality         = 6; // Stats
static const CData_Icon             = 7; // Anzeige-Icon
static const CData_Condition        = 8; // Bedingung
static const CData_DisplayCondition = 9; // Bedingung
static const CData_Facet            = 10;// Facet
static const CData_DescArray		= 11;// Array mit Beschreibunstexten
static const CData_FinishSelection  = 12;// Soll die Auswahl beendet werden?
static const CData_Subclass         = 13;// Soll die Auswahl beendet werden?
static const CData_Voice			= 14;
static const CData_Blood			= 15;
static const CData_Gender			= 16;
static const CData_IsTeam			= 17; // kann die Klasse als Team ausgewählt werden?
static const CData_AIRole			= 18;

public func GetCData(int iClass, int iData, int iPlr)
{
  return PrivateCall(this, Format("Class%dInfo", iClass), iData, iPlr);
  //return ObjectCall(this, Format("Class%dInfo", iClass), iData, iPlr);
}


public func SetupClass(int iClass, int iPlr, int iSel)
{
	if(GetPlayerType( iPlr) == C4PT_Script)
	{
		//Log("Bot %d Selected %d", iPlr, iClass);
	}
	if(!GetCData(iClass, CData_Condition, iPlr)) return;

	//Speichern
	//Log("aLastClass was %d",aLastClass[iPlr] );
	aLastClass[iPlr] = iClass;
	//Log("aLastClass is %d",iClass);
	if(!GetCData(iClass,CData_Subclass,iPlr)) aClassName[iPlr] = GetCData(iClass, CData_Name, iPlr );
	var pCrew = aCrew[iPlr];

	// fertig?
	if( GetCData(iClass, CData_FinishSelection) )
	{
		Finish(pCrew, iClass);
	}
	// Sub-Klassen wählen
	else
	{
		this->~OnClassSelectionTemp( aCrew[iPlr], iClass);
		CloseMenu(pCrew);

		Schedule(Format("OpenClassMenu(Object(%d),0)",ObjectNumber(pCrew)),1);
	}

	return true;
}


func Finish(object pClonk, int iClass)
{
	if(!pClonk /*|| !Contained(pClonk)*/) return;
	var iPlayer = GetOwner(pClonk);

	//Menü schließen
	CloseMenu(GetCursor(iPlayer));
	CloseMenu(pClonk);

	//Effekt entfernen
	for(var i = 0; i < GetEffectCount("Spawntimer", this); i++)
	if(EffectVar(0, this, GetEffect("Spawntimer", this, i)) == iPlayer)
		RemoveEffect("Spawntimer", this, i);
	//PlayerMessage(iPlayer, "@");

	//Broadcasten
	GameCallEx("OnClassSelection", aCrew[iPlayer], iClass);
}

private func Default(int iData)
{
	if(iData == CData_Name)           return "<Classname>";
	if(iData == CData_Desc)           return "<Description>";
	if(iData == CData_Clonk)          return ID_Player;
	if(iData == CData_Agility)        return [6];
	if(iData == CData_Accuracy)       return [10];
	if(iData == CData_Vitality)       return [5];
	if(iData == CData_Icon)           return GetID();
	if(iData == CData_Condition)      return true;
	if(iData == CData_DisplayCondition)return true;
	if(iData == CData_Facet)          return;
	if(iData == CData_DescArray)		return[];
	if(iData == CData_FinishSelection)  return false;
	if(iData == CData_Subclass)         return false;
	if(iData == CData_Voice) 			return "m";
	if(iData == CData_Blood)			return "Human";
	if(iData == CData_Gender)			return "m";
	if(iData == CData_IsTeam)			return false;
	if(iData == CData_AIRole)			return ROLE_Assault|ROLE_Support|ROLE_Defend|ROLE_Sniper;
	return true;
}

private func Subclass(int iData)
{
	if(iData == CData_Name)           	return "";
	if(iData == CData_Desc)           	return "";
	if(iData == CData_Clonk)          	return ID_Player;
	if(iData == CData_Agility)        	return 0;
	if(iData == CData_Accuracy)       	return 0;
	if(iData == CData_Vitality)       	return 0;
	if(iData == CData_Icon)           	return 0;
	if(iData == CData_Condition)      	return  false;
	if(iData == CData_DisplayCondition)	return false;
	if(iData == CData_Facet)          	return 0;
	if(iData == CData_DescArray)		return [];
	if(iData == CData_FinishSelection)  return false;
	if(iData == CData_Subclass)         return true;
	return 0;
}

public func GetClassAmount()
{
	var i = 1;
	while(GetCData(i))  i++;
	return i;
}


private func Class0Info(int iData, int iPlr) // Standardclonk, wenn keine Klassenauswahl da ist
{
  if(iData == CData_Name)       return 0;//"NoClass";
  if(iData == CData_Desc)       return "";
  if(iData == CData_Agility)    return 5;	// mittlere Geschwindigkeit
  if(iData == CData_Accuracy)   return 7;	// volle Genauigkeit (war 10)
  if(iData == CData_Vitality)   return 4;	// 100 Energie
  if(iData == CData_Facet)      return 1;
  if(iData == CData_Condition)  return false;
  return Default(iData);
}

