#strict 2
#appendto CHOS


protected func OpenTeamMenu(id dummy, int iSelection)
{
	if(GetType(aTeamClass) != C4V_Array)
	{
		Log("Reset aTeamClass");
		aTeamClass = [];
	}
	ChooserSmallSelect();

	var pClonk = GetCursor(GetPlayerByIndex(g_iRoundPlr, C4PT_User));
	//Menü erstellen
	CreateMenu(GetID(), pClonk, 0,0,"$TeamArrangement$",0, 1);
	//Teams auflisten
	for(var j = 0; j < GetPlayerCount(); j++)
		AddMenuItem(Format("%s (%s)", GetTaggedPlayerName(GetPlayerByIndex(j)), GetTeamName(GetPlayerTeam(j))), "SwitchTeam", HZCK, pClonk, 0, GetPlayerByIndex(j));

	//Team-Klasse festlegen
	var bFixedTeams = false;
	for(var i=0, idR, def, j, check ; idR = GetDefinition(i, Chooser_Cat) ; i++)
    if(DefinitionCall(idR, "IsChooseable") && !GetLength(FindObjects(Find_ID(idR))))
    	if(idR == ID_Rule_FixedTeams)
    		if(aRules[i])
    			bFixedTeams = true;

	if( bFixedTeams )
	{
		for(var j=0; j<GetTeamCount(); j++)
		{
			var i = GetTeamByIndex(j);
				// zufälliges Team generieren
				OpenTeamClassSelection(0,i,0,true);
				// Team-Name und Klasse schreiben
				var iClass = aTeamClass[i], clName;
				var idIcon, facet;
				if(!iClass)
				{
					clName = "$Random$";
					idIcon = CTFL;
					facet = 0;
				}
				else
				{
					clName = GetCData(iClass,CData_Name);
					idIcon = GetCData(iClass,CData_Icon);
					facet = GetCData(iClass,CData_Facet);
				}
				var szName = Format("%s (%s)",ColorizeString(GetTeamName(i),GetTeamColor(i)),clName);


				AddMenuItem(szName,"OpenTeamClassSelection",idIcon,pClonk,0,i,0,2,facet);
		}
	}
	//Fertig
	AddMenuItem("$Finished$", "OpenMenu", CHOS, pClonk,0,0, "$Finished$",2,3);
	//Letzten Eintrag auswählen
	SelectMenuItem(iSelection, pClonk);

	for(var j = 0; j < GetPlayerCount(); j++)
	{
		if(j == 0) continue;
		//Für jeden Spieler das selbe Menü auch nochmal erstellen
		var clnk = GetCursor(GetPlayerByIndex(j));
		CreateMenu(GetID(), clnk, 0, 0, 0, 0, 1);
		for(var k = 0; k < GetPlayerCount(); k++)
		{
			AddMenuItem(Format("%s (%s)", GetTaggedPlayerName(GetPlayerByIndex(k)), GetTeamName(GetPlayerTeam(k))), 0, PCMK, clnk, 0, GetPlayerByIndex(k));
		}
	}
}

protected func SwitchTeam(id dummy, int iPlr)
{
	var team = GetPlayerTeam(iPlr);
	if(GetTeamName(GetTeamByIndex(team)))
		team = GetTeamByIndex(team);
	else
		team = GetTeamByIndex(0);

	SetPlayerTeam(iPlr, team);

	var sel = GetMenuSelection(GetCursor(GetPlayerByIndex(g_iRoundPlr, C4PT_User)));

	//Geräusch
	//Sound("Grab", 1,0,0,1);

	for(var j = 0; j < GetPlayerCount(); j++)
	{
		if(j == 0) continue;
		CloseMenu(GetCursor(GetPlayerByIndex(j)));
	}

	//Menü wieder eröffnen
	OpenTeamMenu(0, iPlr);
	return true;
}

protected func InitializePlayer(int iPlr, int iX, int iY, object pBase, int iTeam, id idExtra)
{
	if(GetPlayerType(iPlr) == C4PT_Script)
		for(var i = 1 ; i < aAI[iTeam] ; i++)
			CreateClonk(iPlr);
	aAI[iTeam] = 0;
	if(Death) return;
	//Alle Clonks des Spielers verstauen
	for(var i=0, pCrew, tmp ; pCrew = GetCrew(iPlr, i) ; i++)
	{
		tmp = CreateObject(TIM1, GetX(pCrew), GetY(pCrew), -1);
		SetCategory(GetCategory(tmp) | C4D_Foreground,tmp);
		SetGraphics(0, tmp, GetID(pCrew), 1, 5, 0, 1, pCrew);
		Enter(tmp, pCrew);
		Eastern(tmp);
	}
}


protected func OpenTeamClassSelection( id def, int iTeam, int iSel, bool bCancel)
{
	ChooserSmallSelect();
	var pClonk = GetCursor(GetPlayerByIndex(g_iRoundPlr, C4PT_User));
	//Menü erstellen
	CreateMenu(GetID(), pClonk, this, 0, "$NewTeam$", 0, C4MN_Style_Dialog, false);

	var i,classes = [];
	while(GetCData(++i, CData_Name))
		if( GetCData(i, CData_Condition, -1) && GetCData(i,CData_IsTeam,-1))
			classes[GetLength(classes)] = i;

	// Zufällige Vorauswahl
	if(!aTeamClass[iTeam]) aTeamClass[iTeam]=classes[Random(GetLength(classes))];

	// damit das Menu die Vorauswahl direkt am Anfang treffen kann
	if(bCancel) return;

	for( var j = 0; j< GetLength( classes); j++ )
	{
		i = classes[j];
		var def, icon;
		icon = GetCData(i, CData_Icon);
    	def = CreateObject(icon, 0,0, -1);

    	// Hervorherbung, welche Klasse gewählt ist
    	if( aTeamClass[iTeam] == i )
        {
    		SetGraphics("Chosen", def, WPCH, 1, GFXOV_MODE_Picture);
    		SetObjDrawTransform(650,0,5000,0,650,5000, def, 1);
        }


		var szName = GetCData(i,CData_Name);
		AddMenuItem(szName, Format("SetupTeamClass(%d, %d, %d)", iTeam, i,j), 0, pClonk, 0, 0, 0, 4, def);
   		RemoveObject(def);
	}
	// Fertig
	AddMenuItem("$Finished$", "OpenTeamMenu", CHOS, pClonk,0,0,0,2,3);
	// Menüeintrag auswählen
	SelectMenuItem(iSel, pClonk);

}

protected func SetupTeamClass( int iTeam, int iClass, int iSel)
{
	//Log("SetupTeamClass");
	if(GetType(aTeamClass) != C4V_Array)
	{
		//Log("Reset aTeamClass");
		aTeamClass = [];
	}
	aTeamClass[iTeam]=iClass;
	//Log("aTeamClass %v",aTeamClass);
	OpenTeamClassSelection(0,iTeam,iSel);
}

public func GetCData(int iClass, int iData, int iPlr)
{
	var obj = CreateObject(ID_Rule_ClassSelection,0,0,-1);

	var result = obj ->~GetCData(iClass,iData,iPlr);
	RemoveObject(obj);
	return result;
}
