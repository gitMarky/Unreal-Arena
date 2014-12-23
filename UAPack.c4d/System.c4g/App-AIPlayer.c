#strict 2
#appendto __AI

// das hat er dem falschen Spieler zugeordnet oO
public func InitializePlayer(int iPlr, int iX, int iY, object pBase, int iTeam, id idExtra)
{
	//Log("AI Initialized for %d",iPlr);
	SetCategory(C4D_StaticBack);
	if(FindObjectOwner(GetID(), iPlr)) return RemoveObject();

}



public func Activity()
{
  // Kein Besitzer?
  if(GetOwner() == NO_OWNER) return;
  // Spielziel ermitteln (mehrere Spielziele gehen nicht!)
  var goal = GetGoal();
  // Operate according mission objectives!
  for(var i=0, pCrew ; pCrew = GetCrew(GetOwner(), i) ; i++)
  {
    if(pCrew->Contained())
    {
      // Clonk steckt in TIM2?
      if(pCrew->Contained()->GetID() == TIM2) pCrew->Contained()->ContainedUp(); // von ContainedLeft() geändert - er soll wenn möglich seine Klasse neu wählen, um sich besser der Teamstrategie anzupassen
      // Container ist verschwunden, weil es TIM2 war?
      if(pCrew->Contained())
      // Clonk steckt in TIM1?
      if(pCrew->Contained()->GetID() == TIM1)
        // Waffenwahl aktiviert? (Achtung: Hardcode!)
        if(FindObject(WPCH))
        {
          var rule = FindObject(WPCH);
          // Warenangebot abfragen
          var aWaresAll = rule->GetWares("IsWeapon");
          var aWares = CreateArray();
          for(var ware in aWaresAll)
            if(GetWealth(GetOwner()) >= GetValue(0, ware))
              aWares[GetLength(aWares)] = ware;
          // Irgendwas rauspicken
          var rand = BoundBy(Random(4), 0, GetLength(aWares));
          var toBuy = CreateArray();
          var pseudoSet = CreateObject(L_ST, 0,0, -1);
          for(var i=0 ; i < rand ; i++)
          {
            var weapon = aWares[Random(GetLength(aWares))];
            while(pseudoSet->ContainsItem(weapon))
              weapon = aWares[Random(GetLength(aWares))];
            pseudoSet->AddItem(weapon);
          }
          // Kaufen
          for(i=0 ; i < pseudoSet->GetItemCount() ; i++)
            PrivateCall(rule, "BuyItem", pseudoSet->GetItem(i), pCrew);
          // Menü weg
          rule->Finish(0, pCrew);
        }
    }
  }
  // spezielle Taktik...
  if(goal)
    if(!(goal->~AITactic(this()))) {
      for(var i=0, pCrew=0 ; pCrew = GetCrew(GetOwner(), i) ; i++) {
        // Fallback
        TacticDefault(pCrew);
      }
  }
}



/* Helferlein */

// Zufälligen Wegpunkt suchen
//private func GetRandomWp()
//{
//	var wps = FindObjects(Find_ID(WAYP));
//	return(wps[Random(GetLength(wps))]);
//}

// Sniper
public func Sniper(pCrew)
{
	var owner = GetOwner(pCrew);
	/*  if(pCrew->GetAggroLevel() != Aggro_Guard)
	  {
		pCrew->SetMacroCommand(this(), "None", 0,0,0,0,Aggro_Guard);
		pCrew->DebugMessage(Format("@%s", text), "tactics");
	  }
*/
	  var wps = FindObjects(Find_ID(WAYP), Find_Func("CheckInfo",WPInfo_Sniper,GetPlayerTeam(owner)));
	  	var wp, snipe;

	 	DebugLog("Found sniping points %v","tactics",wps);
	 	for(wp in wps)
	 	{
			if(ObjectDistance(pCrew,wp)<5)
			{
				snipe = true;
				Log("Guarding Sniper Point %d",ObjectNumber(pCrew));
				//pAIPlayer->Guard(pCrew,"Guarding our sniper point!"); // passt :)
				break;
			}
		}
		if(!snipe)
		{
				wp = wps[Random(GetLength(wps))];
				Message("!!!",wp);

				if(wp) pCrew->SetMacroCommand(this(), "MoveTo",	wp,0,0,0,Aggro_Shoot);
				DebugMessage("@Going to a sniper point", "tactics");
		}
		else
		{

			Message("",pCrew);
			pCrew->SetMacroCommand(0, "Wait",	0,0,0,0,35);

			var target;
			if(!pCrew->GetAggroTarget()) target = pCrew->~UTBotAIFindEnemy(pCrew,1500);
			if(target)
			{
				pCrew->SetAggroLevel(Aggro_Shoot);
				pCrew->SetAggroTarget(target);
				Message("@Sniping,...",pCrew);
				Message("@Sn: %d",target,ObjectNumber(pCrew));
			}
			else
			{
				pCrew->SetAggroLevel(Aggro_Nothing);
				Message("@Waiting,...",pCrew);
			}

			//if(!PathFree(GetX(),GetY(),GetX(target),GetY(target))) return(0);

			//var weapon = FindBestWeapon( target );

			//UseWeaponOn( target, weapon );

		}
}
