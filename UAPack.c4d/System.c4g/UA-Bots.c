/*-- Neues Script --*/

#strict 2

// Fügt einen neuen Clonk zum angegeben Team hinzu
global func AddBot(int iTeam)
{
  // Spieler suchen
  for(var i=0, found = -1 ; i < GetPlayerCount(C4PT_Script) ; i++)
    if(GetPlayerTeam(GetPlayerByIndex(i, C4PT_Script)) == iTeam)
    {
      found = GetPlayerByIndex(i, C4PT_Script);
      break;
    }
  // Spieler vorhanden?
  if(found == -1) return(AddAI(iTeam));
  // Clonk erstellen
  var bot = CreateObject(ID_Player, 0,0, found);
  MakeCrewMember(bot, found);
  GameCall("RelaunchPlayer", found, bot, -1);
  return(bot);
}
