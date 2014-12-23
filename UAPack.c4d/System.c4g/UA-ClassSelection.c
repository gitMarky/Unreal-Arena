#strict 2
#appendto R_M3

global func SelectNoClass(pCrew)
{
	var iPlr = GetOwner(pCrew);
	var cID = UBOT;

	if(cID != GetID(pCrew)) pCrew->~Redefine(cID);

	//Auswählen
	SelectCrew(iPlr, pCrew, 1);
	SetPlrView(iPlr, pCrew);
	SetCursor(iPlr, pCrew, true, false);

	var iClassAgility, iClassAccuracy,iClassVitality;
	iClassAgility =  5;
	iClassAccuracy = 7;
	iClassVitality = 4;
	DebugLog("SelectNoClass","classselect");

	ClonkSetClassStats( pCrew, iClassAgility, iClassAccuracy, iClassVitality );

	var gender = "m";
	var voice = "m";

	var i = Random(4);
	if( i == 1 )
		voice = "s";
	else if( i == 2 )
		voice = "g";
	else if( i == 3 )
		voice = "j";

	aVoice[iPlr] = voice;
	aGender[iPlr] = gender;
}

public func OnClassSelection( object pCrew, iClass )
{
	if(!iClass) return; // das hier soll vllt wieder weg? weil ich habe ja die Klasse 0 auch definiert!
	OnClassSelectionTemp( pCrew, iClass );

	var iPlr = GetOwner(pCrew);
	var cID = GetCData(iClass, CData_Clonk, iPlr);

	if(cID != GetID(pCrew)) pCrew->~Redefine(cID);

	//Auswählen
	SelectCrew(iPlr, pCrew, 1);
	SetPlrView(iPlr, pCrew);
	SetCursor(iPlr, pCrew, true, false);

	var iClassAgility, iClassAccuracy,iClassVitality;

	iClassAgility =  GetCData(iClass, CData_Agility, iPlr);
	iClassAccuracy = GetCData(iClass, CData_Accuracy, iPlr);
	iClassVitality = GetCData(iClass, CData_Vitality, iPlr);

	ClonkSetClassStats( pCrew, iClassAgility, iClassAccuracy, iClassVitality );
}

global func ClonkSetClassStats( pCrew, iClassAgility, iClassAccuracy, iClassVitality )
{
	var iPlr = GetOwner(pCrew);
	var iPhysEn, iPhysWa, iPhysJu, iPhysFl;

	DebugLog("%d %d %d","classselect", iClassAgility, iClassAccuracy, iClassVitality);

	if(iClassVitality > 0)
	{
		iPhysEn=80000  + iClassVitality*5000; // 85 - 130
		aVitality[iPlr] = iClassVitality;

		SetPhysical("Energy",iPhysEn,2,pCrew);
		SetPhysical("Energy",iPhysEn,0,pCrew);
		DoEnergy(1000,pCrew);
	}
	if(iClassAccuracy > 0) aAccuracy[iPlr] = iClassAccuracy;
	if(iClassAgility > 0)
	{
		iPhysWa=100000 + iClassAgility*6000;  // 106 - 160
		iPhysJu= 30000 + iClassAgility*1200 ; // 312 - 420
		iPhysFl=   100 + iClassAgility*40;    // 140 - 500
		aAgility[iPlr] = iClassAgility;

		SetPhysical("Walk",iPhysWa,2,pCrew);     SetPhysical("Walk",iPhysWa,0,pCrew);
		SetPhysical("Jump",iPhysJu,2,pCrew);     SetPhysical("Jump",iPhysJu,0,pCrew);
		SetPhysical("Float",iPhysFl,2,pCrew);   SetPhysical("Float",iPhysFl,0,pCrew);
	}

}

public func OnClassSelectionTemp( object pCrew, iClass )
{
	if(!iClass) return;
	var iPlr = GetOwner(pCrew);
	var szVoice = GetCData(iClass, CData_Voice, iPlr), szBlood = GetCData(iClass, CData_Blood, iPlr), szGender = GetCData(iClass, CData_Gender, iPlr);

	//Log("voice, blood, gender: %s, %s, %s",szVoice, szBlood, szGender);
	if(szVoice) aVoice[iPlr] = szVoice;
	if(szBlood) aBlood[iPlr] = szBlood;
	if(szGender) aGender[iPlr] = szGender;
}




/* Die Klassen */

private func Class1Info(int iData, int iPlr)
{
  if(iData == CData_Name)       return "Marine";
  if(iData == CData_Desc)       return "Pipipipipi||Stärken:";
  if(iData == CData_Agility)    return 8;
  if(iData == CData_Accuracy)   return 5;
  if(iData == CData_Vitality)   return 7;
  if(iData == CData_Facet)      return 1;
  if(iData == CData_Condition)
  {
	  //Log("Condition Check PlrGetClassName(%d) == %s",iPlr, PlrGetClassName(0));
	  //return !eval(Format( "PlrGetClassName(%d) == \"Marine\"",iPlr) );
	  return !eval(Format( "PlrGetClassName(%d)",iPlr) );
  }
  if(iData == CData_Voice )		return "s";
  if(iData == CData_IsTeam)		return true;
  return Default(iData);
}

private func Class2Info(int iData, int iPlr)
{
  if(iData == CData_Name)       return "Mercenary";
  if(iData == CData_Desc)       return "Pipipipipi||Stärken:";
  if(iData == CData_Agility)    return 7;
  if(iData == CData_Accuracy)   return 8;
  if(iData == CData_Vitality)   return 6;
  if(iData == CData_Facet)      return 1;
  if(iData == CData_Condition)
  {
	  //return !eval(Format( "PlrGetClassName(%d) == \"Mercenary\"",iPlr) );
	  return !eval(Format( "PlrGetClassName(%d)",iPlr) );
  }
  if(iData == CData_IsTeam)		return true;
  return Default(iData);
}

private func Class3Info(int iData, int iPlr)
{
  if(iData == CData_Name)       return "Juggernaut";
  if(iData == CData_Desc)       return "Pipipipipi||Stärken:";
  if(iData == CData_Agility)    return 7;
  if(iData == CData_Accuracy)   return 4;
  if(iData == CData_Vitality)   return 10;
  if(iData == CData_Facet)      return 1;
  if(iData == CData_Condition)
  {
	  //return !eval(Format( "PlrGetClassName(%d) == \"Juggernaut\"",iPlr) );
	  return !eval(Format( "PlrGetClassName(%d)",iPlr) );
  }
  if(iData == CData_Voice )		return "j";
  if(iData == CData_IsTeam)		return true;
  return Default(iData);
}

private func Class4Info(int iData, int iPlr)
{
  if(iData == CData_Name)       return "Necris";
  if(iData == CData_Desc)       return "Pipipipipi||Stärken:";
  if(iData == CData_Agility)    return 7;
  if(iData == CData_Accuracy)   return 5;
  if(iData == CData_Vitality)   return 8;
  if(iData == CData_Facet)      return 1;
  if(iData == CData_Condition)
  {
	  //return !eval(Format( "PlrGetClassName(%d) == \"Necris\"",iPlr) );
	  return !eval(Format( "PlrGetClassName(%d)",iPlr) );
  }
  if(iData == CData_Voice )		return "n";
  if(iData == CData_IsTeam)		return true;
  return Default(iData);
}

private func Class5Info(int iData, int iPlr)
{
  if(iData == CData_Name)       return "Nakthi";
  if(iData == CData_Desc)       return "Pipipipipi||Stärken:";
  if(iData == CData_Agility)    return 6;
  if(iData == CData_Accuracy)   return 9;
  if(iData == CData_Vitality)   return 6;
  if(iData == CData_Facet)      return 1;
  if(iData == CData_Condition)
  {
	  //return !eval(Format( "PlrGetClassName(%d) == \"Nakthi\"",iPlr) );
	  return !eval(Format( "PlrGetClassName(%d)",iPlr) );
  }
  if(iData == CData_IsTeam)		return true;
  return Default(iData);
}

private func Class6Info(int iData, int iPlr)
{
  if(iData == CData_Name)       return "Skaarj";
  if(iData == CData_Desc)       return "Pipipipipi||Stärken:";
  if(iData == CData_Agility)    return 4;
  if(iData == CData_Accuracy)   return 8;
  if(iData == CData_Vitality)   return 9;
  if(iData == CData_Facet)      return 1;
  if(iData == CData_Condition)
  {
	  //return !eval(Format( "PlrGetClassName(%d) == \"Skaarj\"",iPlr) );
	  return !eval(Format( "PlrGetClassName(%d)",iPlr) );
  }
  if(iData == CData_Voice )		return "a";
  if(iData == CData_Blood )		return "Alien";
  if(iData == CData_IsTeam)		return true;
  return Default(iData);
}

private func Class7Info(int iData, int iPlr)
{
  if(iData == CData_Name)       return "Liandri";
  if(iData == CData_Desc)       return "Pipipipipi||Stärken:";
  if(iData == CData_Agility)    return 7;
  if(iData == CData_Accuracy)   return 6;
  if(iData == CData_Vitality)   return 8;
  if(iData == CData_Facet)      return 1;
  if(iData == CData_Condition)
  {
	  //return !eval(Format( "PlrGetClassName(%d) == \"Liandri\"",iPlr) );
	  return !eval(Format( "PlrGetClassName(%d)",iPlr) );
  }
  if(iData == CData_Voice )		return "r";
  if(iData == CData_Blood )		return "Machine";
  if(iData == CData_IsTeam)		return true;
  return Default(iData);
}

private func Class8Info(int iData, int iPlr)
{
  if(iData == CData_Name)       return "Metal Guard";
  if(iData == CData_Desc)       return "Pipipipipi||Stärken:";
  if(iData == CData_Agility)    return 8;
  if(iData == CData_Accuracy)   return 6;
  if(iData == CData_Vitality)   return 7;
  if(iData == CData_Facet)      return 1;
  if(iData == CData_Condition)
  {
	  //return !eval(Format( "PlrGetClassName(%d) == \"Metal Guard\"",iPlr) );
	  return !eval(Format( "PlrGetClassName(%d)",iPlr) );
  }
  if(iData == CData_Voice )		return "g";
  if(iData == CData_IsTeam)		return true;
  return Default(iData);
}


private func Class9Info(int iData, int iPlr)
{
  if(iData == CData_Name)       return "Assault";
  if(iData == CData_AIRole)		return ROLE_Assault;
  if(iData == CData_Desc)       return "Pipipipipi||Stärken:";
  if(iData == CData_Accuracy)
  {
	  //if(PlrGetClassName(iPlr) == "Marine" ) return 6;
	  if(aClassName[iPlr] == "Marine" ) return 6;
	  if(aClassName[iPlr] == "Mercenary" ) return 5;
	  if(aClassName[iPlr] == "Juggernaut" ) return 5;
	  if(aClassName[iPlr] == "Necris" ) return 6;
	  if(aClassName[iPlr] == "Nakthi" ) return 4;
	  if(aClassName[iPlr] == "Skaarj" ) return 3;
	  if(aClassName[iPlr] == "Liandri" ) return 4;
	  if(aClassName[iPlr] == "Metal Guard" ) return 6;
  }
  if(iData == CData_Agility)
  {
	  if(aClassName[iPlr] == "Marine" ) return 7;
	  if(aClassName[iPlr] == "Mercenary" ) return 9;
	  if(aClassName[iPlr] == "Juggernaut" ) return 6;
	  if(aClassName[iPlr] == "Necris" ) return 6;
	  if(aClassName[iPlr] == "Nakthi" ) return 10;
	  if(aClassName[iPlr] == "Skaarj" ) return 9;
	  if(aClassName[iPlr] == "Liandri" ) return 7;
	  if(aClassName[iPlr] == "Metal Guard" ) return 7;  }
  if(iData == CData_Vitality)
  {
	  if(aClassName[iPlr] == "Marine" ) return 8;
	  if(aClassName[iPlr] == "Mercenary" ) return 7;
	  if(aClassName[iPlr] == "Juggernaut" ) return 10;
	  if(aClassName[iPlr] == "Necris" ) return 9;
	  if(aClassName[iPlr] == "Nakthi" ) return 7;
	  if(aClassName[iPlr] == "Skaarj" ) return 9;
	  if(aClassName[iPlr] == "Liandri" ) return 10;
	  if(aClassName[iPlr] == "Metal Guard" ) return 8;  }
  if(iData == CData_Facet)		return 1;
  if(iData == CData_Condition)  return eval(Format( "PlrGetClassName(%d)",iPlr) );
  if(iData == CData_FinishSelection ) return true;
  return Subclass(iData);
}

private func Class10Info(int iData, int iPlr)
{
  if(iData == CData_Name)       return "Support";
  if(iData == CData_AIRole)		return ROLE_Support;
  if(iData == CData_Desc)       return "Pipipipipi||Stärken:";
  if(iData == CData_Accuracy)
  {
	  if(aClassName[iPlr] == "Marine" ) return 7;
	  if(aClassName[iPlr] == "Mercenary" ) return 6;
	  if(aClassName[iPlr] == "Juggernaut" ) return 6;
	  if(aClassName[iPlr] == "Necris" ) return 7;
	  if(aClassName[iPlr] == "Nakthi" ) return 6;
	  if(aClassName[iPlr] == "Skaarj" ) return 2;
	  if(aClassName[iPlr] == "Liandri" ) return 8;
	  if(aClassName[iPlr] == "Metal Guard" ) return 7;  }
  if(iData == CData_Agility)
  {
	  if(aClassName[iPlr] == "Marine" ) return 6;
	  if(aClassName[iPlr] == "Mercenary" ) return 9;
	  if(aClassName[iPlr] == "Juggernaut" ) return 5;
	  if(aClassName[iPlr] == "Necris" ) return 6;
	  if(aClassName[iPlr] == "Nakthi" ) return 9;
	  if(aClassName[iPlr] == "Skaarj" ) return 9;
	  if(aClassName[iPlr] == "Liandri" ) return 8;
	  if(aClassName[iPlr] == "Metal Guard" ) return 7;  }
  if(iData == CData_Vitality)
  {
	  if(aClassName[iPlr] == "Marine" ) return 8;
	  if(aClassName[iPlr] == "Mercenary" ) return 6;
	  if(aClassName[iPlr] == "Juggernaut" ) return 10;
	  if(aClassName[iPlr] == "Necris" ) return 8;
	  if(aClassName[iPlr] == "Nakthi" ) return 6;
	  if(aClassName[iPlr] == "Skaarj" ) return 10;
	  if(aClassName[iPlr] == "Liandri" ) return 5;
	  if(aClassName[iPlr] == "Metal Guard" ) return 7;  }
  if(iData == CData_Facet)		return 1;
  if(iData == CData_Condition)  return eval(Format( "PlrGetClassName(%d)",iPlr) );
  if(iData == CData_FinishSelection ) return true;
  return Subclass(iData);
}

private func Class11Info(int iData, int iPlr)
{
  if(iData == CData_Name)       return "Sniper";
  if(iData == CData_AIRole)		return ROLE_Sniper;
  if(iData == CData_Desc)       return "Pipipipipi||Stärken:";
  if(iData == CData_Accuracy)
  {
	  if(aClassName[iPlr] == "Marine" ) return 10;
	  if(aClassName[iPlr] == "Mercenary" ) return 10;
	  if(aClassName[iPlr] == "Juggernaut" ) return 10;
	  if(aClassName[iPlr] == "Necris" ) return 9;
	  if(aClassName[iPlr] == "Nakthi" ) return 10;
	  if(aClassName[iPlr] == "Skaarj" ) return 6;
	  if(aClassName[iPlr] == "Liandri" ) return 10;
	  if(aClassName[iPlr] == "Metal Guard" ) return 10;  }
  if(iData == CData_Agility)
  {
	  if(aClassName[iPlr] == "Marine" ) return 5;
	  if(aClassName[iPlr] == "Mercenary" ) return 6;
	  if(aClassName[iPlr] == "Juggernaut" ) return 3;
	  if(aClassName[iPlr] == "Necris" ) return 5;
	  if(aClassName[iPlr] == "Nakthi" ) return 8;
	  if(aClassName[iPlr] == "Skaarj" ) return 7;
	  if(aClassName[iPlr] == "Liandri" ) return 4;
	  if(aClassName[iPlr] == "Metal Guard" ) return 4;  }
  if(iData == CData_Vitality)
  {
	  if(aClassName[iPlr] == "Marine" ) return 6;
	  if(aClassName[iPlr] == "Mercenary" ) return 5;
	  if(aClassName[iPlr] == "Juggernaut" ) return 8;
	  if(aClassName[iPlr] == "Necris" ) return 7;
	  if(aClassName[iPlr] == "Nakthi" ) return 3;
	  if(aClassName[iPlr] == "Skaarj" ) return 8;
	  if(aClassName[iPlr] == "Liandri" ) return 7;
	  if(aClassName[iPlr] == "Metal Guard" ) return 7;  }
  if(iData == CData_Facet)		return 1;
  if(iData == CData_Condition)  return eval(Format( "PlrGetClassName(%d)",iPlr) );
  if(iData == CData_FinishSelection ) return true;
  return Subclass(iData);
}

private func Class12Info(int iData, int iPlr)
{
  if(iData == CData_Name)       return "Defender";
  if(iData == CData_AIRole)		return ROLE_Defend;
  if(iData == CData_Desc)       return "Pipipipipi||Stärken:";
  if(iData == CData_Accuracy)
  {
	  if(aClassName[iPlr] == "Marine" ) return 8;
	  if(aClassName[iPlr] == "Mercenary" ) return 7;
	  if(aClassName[iPlr] == "Juggernaut" ) return 7;
	  if(aClassName[iPlr] == "Necris" ) return 8;
	  if(aClassName[iPlr] == "Nakthi" ) return 7;
	  if(aClassName[iPlr] == "Skaarj" ) return 4;
	  if(aClassName[iPlr] == "Liandri" ) return 7;
	  if(aClassName[iPlr] == "Metal Guard" ) return 8;  }
  if(iData == CData_Agility)
  {
	  if(aClassName[iPlr] == "Marine" ) return 5;
	  if(aClassName[iPlr] == "Mercenary" ) return 7;
	  if(aClassName[iPlr] == "Juggernaut" ) return 4;
	  if(aClassName[iPlr] == "Necris" ) return 4;
	  if(aClassName[iPlr] == "Nakthi" ) return 7;
	  if(aClassName[iPlr] == "Skaarj" ) return 7;
	  if(aClassName[iPlr] == "Liandri" ) return 4;
	  if(aClassName[iPlr] == "Metal Guard" ) return 5;  }
  if(iData == CData_Vitality)
  {
	  if(aClassName[iPlr] == "Marine" ) return 8;
	  if(aClassName[iPlr] == "Mercenary" ) return 7;
	  if(aClassName[iPlr] == "Juggernaut" ) return 10;
	  if(aClassName[iPlr] == "Necris" ) return 9;
	  if(aClassName[iPlr] == "Nakthi" ) return 7;
	  if(aClassName[iPlr] == "Skaarj" ) return 10;
	  if(aClassName[iPlr] == "Liandri" ) return 10;
	  if(aClassName[iPlr] == "Metal Guard" ) return 8;
  }
  if(iData == CData_Facet)		return 1;
  if(iData == CData_Condition)  return eval(Format( "PlrGetClassName(%d)",iPlr) );
  if(iData == CData_FinishSelection ) return true;
  return Subclass(iData);
}
