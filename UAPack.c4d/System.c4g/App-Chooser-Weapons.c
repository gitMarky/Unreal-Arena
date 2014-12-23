/*-- Neues Script --*/

#strict 2
#appendto CHOS


public func OpenWeaponMenu( iddef, j)
{
	ChooserSmallSelect();
	var pClonk = GetCursor(GetPlayerByIndex(g_iRoundPlr, C4PT_User));
	if(!pClonk) return;
	CloseMenu(pClonk);

	CreateMenu(GetID(), pClonk, 0, 0, "$WeaponMenu$", 0, 1);

	// Regeln einfügen
	for(var i=0, idR, def, j, check; idR = GetDefinition(i, Chooser_Cat) ; i++)
    if(DefinitionCall(idR, "IsWeaponSet") /* && !GetLength(FindObjects(Find_ID(idR)))*/)
    {
    	def = CreateObject(idR, 0,0, -1);
    	if( idR == gIDWeaponSet )
        {
    		SetGraphics("Chosen", def, WPCH, 1, GFXOV_MODE_Picture);
    		SetObjDrawTransform(650,0,5000,0,650,5000, def, 1);
        }
   		AddMenuItem("%s", "ChangeWeaponSet", idR, pClonk, 0, i, 0, 4, def);
   		RemoveObject(def);
    }
	// Eigenes Set bauen
	if( gIDWeaponSet == CHOS )
	{
    	var def = CreateObject(WPCH, 0,0, -1);
		SetGraphics("",       def, CHOS, 0, GFXOV_MODE_Base);
		SetGraphics("Chosen", def, WPCH, 1, GFXOV_MODE_Picture);
		SetObjDrawTransform(650,0,5000,0,650,5000, def, 1);
   		AddMenuItem("$WeaponCustom$", "ChangeWeaponSet", CHOS, pClonk, 0, i, 0, 4, def);
   		RemoveObject(def);
	}
	else
	{
		AddMenuItem("$WeaponCustom$", "ChangeWeaponSet", CHOS, pClonk,0,0,0,0,0);
	}
	// Fertig
	AddMenuItem("$Finished$", "OpenMenu", CHOS, pClonk,0,0,0,2,3);
	// Menüeintrag auswählen
	SelectMenuItem(j, pClonk);
}


public func ChangeWeaponSet( id idSet, int iSel)
{
	ChooserSmallSelect();
	gIDWeaponSet = idSet;
	if( idSet == CHOS )
	{
		SetupCustomSet();
	}
	else
	{
		idSet->~ChooserFinished();
		OpenWeaponMenu(0,iSel);
		UpdateSpawnPoints();
	}
}

public func SetupCustomSet( def, j)
{
	ChooserSmallSelect();
	var pClonk = GetCursor(GetPlayerByIndex(g_iRoundPlr, C4PT_User));
	if(!pClonk) return;
	CloseMenu(pClonk);

	CreateMenu(GetID(), pClonk, 0, 0, "$WeaponCustom$", 0, 1);
	for( var i = 1; i<= 15; i++)
	{
		var szName = GetName(0,eval(Format("gWeaponPos%02d",i)) );
		if(!szName) szName = "---";
		AddMenuItem("%s",Format("SetupCustomSlot(%d,\"IsChooseableWeapon\",%i,0)",i,eval(Format("gWeaponPos%02d",i)) ),eval(Format("gWeaponPos%02d",i)),pClonk);
	}

	//AddMenuItem(Format("Slot  1: %s($WeaponMelee$)",GetName(0,weaponpos01)),"SetupCustomSlot01",weaponpos01);

	// Fertig
	AddMenuItem("$Finished$", "OpenWeaponMenu", CHOS, pClonk,0,0,0,2,3);
	// Menüeintrag auswählen
	SelectMenuItem(j, pClonk);
}


public func SetupCustomSlot( int iSlot, szCall, id idCurrent, int iSel )
{
	var pClonk = GetCursor(GetPlayerByIndex(g_iRoundPlr, C4PT_User));
	if(!pClonk) return;
	CloseMenu(pClonk);

	//idCurrent = eval(Format("gWeaponPos%02d",iSlot);

	CreateMenu(GetID(), pClonk, 0, 0, 0, 0, 1);

	for(var i=0, idR, def, j=0; idR = GetDefinition(i, C4D_Object) ; i++)
	if(DefinitionCall( idR, szCall ))
    {
    	def = CreateObject(idR, 0,0, -1);
    	if( idR == idCurrent )
        {
    		SetGraphics("Chosen", def, WPCH, 1, GFXOV_MODE_Picture);
    		SetObjDrawTransform(650,0,5000,0,650,5000, def, 1);
        }
   		AddMenuItem("%s", Format("SetupCustomAdd(%i,%d,%d,\"%s\")",idR,j,iSlot,szCall), idR, pClonk, 0, j, 0, 4, def);
   		RemoveObject(def);
   		j++;
    }
	// Fertig
	AddMenuItem("$Finished$", "SetupCustomSet", CHOS, pClonk,0, iSlot-1,0,2,3);
	// Menüeintrag auswählen
	SelectMenuItem(iSel, pClonk);
}

public func SetupCustomAdd( id idWeap, int iSel, int iSlot, string szCall)
{
	var ammoID = idWeap->~AmmoPackID();
	if (!ammoID) ammoID = NONE;
	eval(Format("gWeaponPos%02d = %i",iSlot,idWeap));
	if (iSlot > 0 && iSlot <= 10) eval(Format("gAmmoPos%02d = %i",iSlot,ammoID));

	SetupCustomSlot( iSlot, szCall, idWeap, iSel );

	UpdateSpawnPoints();
}

