/*-- Killstatistics --*/

#strict 2
#appendto HHKS

static const KillStat_ColorWeapon = 9610924;//RGB(146,166,172);
static const STAT_Spree = 5; // für je eine Stufe Killing Spree
static g_bFirstBlood; // schon jemand gestorben diese Runde?


/* Killmeldungen */

//Kill mit Team - Spieler von Team A hat Spieler von Team B getötet!
//Die Funktion ist so klug auch zu erkennen, ob es überhaupt Teams gibt
//und ob es ein Teamkill war
public func KTMsg(int plr1, int plr2, int type, id idWeapon)
{
	//haben wir Parameter? D:
	if(!plr1 && !plr2)
		return; //welcher Doofie hat denn den Schwachsinn übergeben >:(

	if(!GetPlayerName(plr1) || !GetPlayerName(plr2)) //weirde Spieler?
		return;

	//haben wir Teamplay?
	var tp;
	for(var goal in FindObjects(Find_Category(C4D_Goal)))
		if(goal->~IsTeamGoal())
			tp = true;
	//nein -> normale killmessage
	if(!tp)
//		return(KMsg(plr1, plr2, type));
		// Kein Hazardspielziel, da wollen wir das eher nicht haben
		return;
	//doch -> teamkiller?
	else if(GetPlrTeamName(plr1) == GetPlrTeamName(plr2))
		return(TKMsg(plr1,plr2));


	var msg;
	if( ! idWeapon )
	{
		if(Random(3) && type > 1) {
			if(type == DMG_Projectile)
				msg = $MsgKillProjectile$;
			else if(type == DMG_Fire)
				msg = $MsgKillFire$;
			else if(type == DMG_Explosion)
				msg = $MsgKillExplosion$;
			else if(type == DMG_Energy)
				msg = $MsgKillEnergy$;
			else if(type == DMG_Bio)
				msg = $MsgKillBio$;
			else
				msg = $MsgKill$;
		}
		else
			msg = $MsgKill$;
	}
	else
		msg = DefinitionCall(idWeapon,"KillMessage");

	msg = msg[Random(GetLength(msg))];
	if( idWeapon )
	{
			msg = ReplaceAll(msg,"pKiller", Format("%s",GetTaggedPlayerName(plr1))   );
			msg = ReplaceAll(msg,"pVictim", Format("%s",GetTaggedPlayerName(plr2))   );
			//msg = ReplaceAll(msg,"sWeapon", Format("<c %x>%s</c>",RGB(83,90,92),GetName(0,idWeapon)));
			msg = ReplaceAll(msg,"sWeapon", Format("<c %x>%s</c>",KillStat_ColorWeapon,GetName(0,idWeapon)));
			msg = Format("{{GTDM}} %s",msg);
	}
	else
		msg = Format(msg, GetTaggedPlayerName(plr1),
		                  GetTaggedPlayerName(plr2) );

	HUDMessage(msg,-1);
	//Log(msg,Format("%s (%s)",GetTaggedPlayerName(plr1),GetPlrTeamName(plr1))
	//							,Format("%s (%s)",GetTaggedPlayerName(plr2),GetPlrTeamName(plr2)));
}

//Kill - Spieler A hat Spieler B getötet!
public func KMsg(int plr1, int plr2, int type, id idWeapon)
{
	var msg;
	/*if(Random(3) && type > 1)
	{
		if(type & DMG_Projectile)
			msg = $MsgKillProjectile$;
		else if(type & DMG_Fire)
			msg = $MsgKillFire$;
		else if(type & DMG_Explosion)
			msg = $MsgKillExplosion$;
		else if(type & DMG_Energy)
			msg = $MsgKillEnergy$;
		else if(type & DMG_Bio)
			msg = $MsgKillBio$;
		else
			msg = $MsgKill$;
	}
	else
		msg = $MsgKill$;

	msg = msg[Random(GetLength(msg))];
	Log(msg, GetTaggedPlayerName(plr1),GetTaggedPlayerName(plr2));*/

	if( ! idWeapon )
	{
		if(Random(3) && type > 1) {
			if(type == DMG_Projectile)
				msg = $MsgKillProjectile$;
			else if(type == DMG_Fire)
				msg = $MsgKillFire$;
			else if(type == DMG_Explosion)
				msg = $MsgKillExplosion$;
			else if(type == DMG_Energy)
				msg = $MsgKillEnergy$;
			else if(type == DMG_Bio)
				msg = $MsgKillBio$;
			else
				msg = $MsgKill$;
		}
		else
			msg = $MsgKill$;
	}
	else
		msg = DefinitionCall(idWeapon,"KillMessage");

	msg = msg[Random(GetLength(msg))];
	if( idWeapon )
	{
			msg = ReplaceAll(msg,"pKiller", Format("%s",GetTaggedPlayerName(plr1))   );
			msg = ReplaceAll(msg,"pVictim", Format("%s",GetTaggedPlayerName(plr2))   );
			msg = ReplaceAll(msg,"sWeapon", Format("<c %x>%s</c>",KillStat_ColorWeapon,GetName(0,idWeapon)));
			msg = Format("{{GTDM}} %s",msg);
	}
	else
		msg = Format(msg, GetTaggedPlayerName(plr1),
		                  GetTaggedPlayerName(plr2) );

	HUDMessage(msg,-1);
}

//Selfkill - Spieler A hat sich selbst umgebracht! D:
public func SKMsg(int plr)
{
	var msg = $MsgSelfKill$;
	msg = msg[Random(GetLength(msg))];
	HUDMessage(Format("{{SKUL}} %s", Format(msg,GetTaggedPlayerName(plr))),-1);
}

//Teamkill - Spieler von Team A hat Spieler des selben Teams getötet! :O
public func TKMsg(int plr, int plr2) {
	HUDMessage(Format("$MsgTeamKill$",GetTaggedPlayerName(plr),GetPlrTeamName(plr),GetTaggedPlayerName(plr2)),-1-GetPlayerTeam(plr));
}

//Score - Spieler von Team A hat gepunktet!
public func SMsg(int plr) {
	HUDMessage(Format("$MsgScore$",GetTaggedPlayerName(plr),GetPlrTeamName(plr)),-1);
}






/* Killstatistiken --- um den Ansager modifiziert */


public func KillStat(object pClonk, int killedplr)
{
  //nein, die Engine darf keine Kills machen. >:(
	if(!pClonk)
		if(!(pClonk = this()))
			return;
	if(killedplr == NO_OWNER) return;
	//Nein, Teamkills bringen keine Punkte. :(
	if(GetPlayerTeam(killedplr) == GetPlayerTeam(GetController(pClonk)))
		return;
	AddEffect("KillStats",pClonk,23,10,this(),HHKS);

	// kein First Blood für Selbstmörder ^^
	if(!g_bFirstBlood && (GetOwner(pClonk) != killedplr))
	{
		UA_Announcer("an_aw_firstblood",pClonk);
		g_bFirstBlood = true;
	}

	// Taunt
	if(!Random(3))
		Sound(Format("%s_%staunt*",CrewGetVoice(pClonk),PlrGetGender(killedplr)),0,pClonk);
	else
		Sound(Format("%s_btaunt*",CrewGetVoice(pClonk)),0,pClonk);

}


//Schaun ob die Killzeit schon abgelaufen ist
func FxKillStatsTimer(object pTarget, int iEffectNumber, int iEffectTime) {
	//Haben wir gerade Killingtime?
	if(!EffectVar(3, pTarget, iEffectNumber))
		return;
	//Schon lange genug gewartet? ----- 5 Sek
	if(iEffectTime - EffectVar(1, pTarget, iEffectNumber) > 180)//126)
	{ //Die dreifache Wahrheit!
		EffectVar(2, pTarget, iEffectNumber) = 0;
		EffectVar(3, pTarget, iEffectNumber) = 0;
	}
}

func FxKillStatsAdd(object pTarget, int iEffectNumber, string szNewEffectName, int iNewEffectTimer) {
	//Neue Daten speichern
	var total = ++EffectVar(0, pTarget, iEffectNumber);
	var running = ++EffectVar(2, pTarget, iEffectNumber);
	EffectVar(1, pTarget, iEffectNumber) = GetEffect("KillStats", pTarget, 0, 6);
	EffectVar(3, pTarget, iEffectNumber) = 1;

	//Auf Killfolgen prüfen
	var msg = $MsgKillingSpree$;
	var t = GetLength(msg);
	var lvl = total/STAT_Spree;
	if(!(total%STAT_Spree) && lvl <= t)
	{
		//Log(Format("%s (%s)", msg[Min(total/STAT_Spree-1,t-1)],GetName(pTarget)));
		if( lvl == 1 ) UA_Announcer("an_ser_killing_spree",pTarget);
		if( lvl == 2 ) UA_Announcer("an_ser_rampage",pTarget);
		if( lvl == 3 ) UA_Announcer("an_ser_dominating",pTarget);
		if( lvl == 4 ) UA_Announcer("an_ser_unstoppable",pTarget);
		if( lvl == 5 ) UA_Announcer("an_ser_godlike",pTarget);
		if( lvl == 6 ) UA_Announcer("an_ser_whicked_sick",pTarget);
	}

	//auf Multikills prüfen
	if(running >= 2)
	{
		// Double Kill (2), Multi kill (3), Ultra kill (4), Megakill (5), MONSTERKILL
		msg = ["an_frag_doublekill","an_frag_multikill","an_frag_ultrakill","an_frag_megakill","an_frag_monsterkill"];
		UA_Announcer(msg[Min(running-2,GetLength(msg)-1)],pTarget);
		msg = $MsgMultikill$;
		//PlayerMessage(GetOwner(pTarget),"<c ff0000>%s</c>",pTarget, msg[Min(running-2,GetLength(msg)-1)]);

	}
}

global func GetPlrTeamName(int plr)
{
  return(Format("<c %x>[%s]</c>", GetTeamColor(GetPlayerTeam(plr)),GetTeamName(GetPlayerTeam(plr))));
}

global func GetTaggedTeamName(int team)
{
  return(Format("<c %x>[%s]</c>", GetTeamColor(team),GetTeamName(team)));
}



