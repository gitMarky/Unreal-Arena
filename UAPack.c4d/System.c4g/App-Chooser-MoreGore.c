/*-- Neues Script --*/

#strict 2
#appendto CHOS

public func OpenMenuCustomEntries( object pClonk )
{
	_inherited( pClonk );
	DebugLog("OpenMenuCustomEntries2 %d","chooser",GetRuleSelected(ID_Rule_MoreGore));

	if(GetRuleSelected(ID_Rule_MoreGore))
	{
		AddMenuItem("%s", "OpenMutatorMenu", ID_Rule_MoreGore, pClonk,0,0,"$DarknessChose$");
	}


}


protected func OpenMutatorMenu(id dummy, int iSelection)
{
	ChooserSmallSelect();
	if(!pGoal) return(OpenMenu());
	var pClonk = GetCursor();
	// Men� aufmachen
	CreateMenu(GetID(), pClonk, 0,0,GetName(0,dummy),0, 1);
	// Anzeige
	AddMenuItem(" ", "OpenMutatorMenu", dummy, pClonk, GetRuleSelected(dummy,true), 0, " ");
	// Z�hler erh�hen
	AddMenuItem("$MoreWinScore$", Format("ChangeMutatorConf(%i,+1)",dummy), CHOS, pClonk, 0, +1, "$MoreWinScore$",2,1);
	// Z�hler senken
	AddMenuItem("$LessWinScore$", Format("ChangeMutatorConf(%i,-1)",dummy), CHOS, pClonk, 0, -1, "$LessWinScore$",2,2);
	// Fertig
	AddMenuItem("$Finished$", "OpenMenu", CHOS, pClonk,0,0,"$Finished$",2,3);
	// Letzten Eintrag ausw�hlen
	SelectMenuItem(iSelection, pClonk);
}

protected func ChangeMutatorConf(id dummy, int iChange)
{
	var i = GetRuleSelected(dummy,0,true);
	// Regel wurde nicht ausgew�hlt
	if(!aRules[i])
	{
		aRulesNr[i] = 0;
	}
	// Regel wurde abgew�hlt
	else
	{
		aRulesNr[i] = BoundBy( aRulesNr[i] +iChange , 1, GetDefMaxUserSelect(dummy));
	}
	// Ger�usch
	//Sound("Grab", 1,0,0,1);
	// Regelmen� wieder aufmachen
	OpenMutatorMenu(dummy, iChange);
}
