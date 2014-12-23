/*--- Flagge! ---*/

#strict

local base, team, cteam, doreturn, lost;
local iToRotate, iMyRotate, iBobRotate, iBobDir, iLastDir;


/* TimerCall */

private func Wee()
{
	if(WildcardMatch(GetAction(), "*Fly*")) return(Wind2Fly());
	if(WildcardMatch(GetAction(), "*Attach*")) return(Clonk2Rotate());
}


private func Wind2Fly()
{
	SetDir(BoundBy(GetWind() / 15 + 3, 2, 4));
}


private func Clonk2Rotate()
{
	if(!GetAlive(GetActionTarget())) return(AttachTargetLost());

	SetDir(BoundBy(-GetXDir(GetActionTarget()) / 10 + 3, 0, 6));
	//SetDir(BoundBy(7 - GetXDir(GetActionTarget()) / 2, 0, 13));

	if(GetDir(GetActionTarget())!= iLastDir ) iMyRotate *= -1;

	var iRan = Abs(GetXDir(GetActionTarget()))/3;
	if(GetDir(GetActionTarget()) == DIR_Left())
	{
		//if(Inside(GetR(), 15, 75)) SetR(GetR()+RandomX(-2,2));
		//else SetR(BoundBy(GetR(), 15, 75));
	    //SetR(15-GetXDir(GetActionTarget())+RandomX(-3,3));
		iToRotate = 15-GetXDir(GetActionTarget());
		iLastDir = DIR_Left();

	}
	if(GetDir(GetActionTarget()) == DIR_Right())
	{
		//if(Inside(GetR(), -75, -15)) SetR(GetR()+RandomX(-2,2));
		//else SetR(BoundBy(GetR(), -75, -15));
		//SetR(-15-GetXDir(GetActionTarget())+RandomX(-3,3));
		iToRotate = -15-GetXDir(GetActionTarget());
		iLastDir = DIR_Right();
	}

	var iMod = BoundBy(iToRotate-iMyRotate,-1,1)*1;

	if(iToRotate >= 0) iMyRotate = BoundBy( iMyRotate + iMod, 0, iToRotate );
	else iMyRotate = BoundBy( iMyRotate + iMod, iToRotate, 0 );

	if(GetProcedure(GetActionTarget()) == "WALK"  ) iBobRotate += -2 + 4*iBobDir;

	if((iBobRotate <= -iRan || iBobRotate >= iRan) && iRan)
	{
		iBobDir = 1-iBobDir;
	}
	else if(!iRan) iBobRotate = 0;

	SetR(iMyRotate+iBobRotate);
}


protected func Activate(pBase, iTeam, rgb)
{
	base = pBase;
	SetAction("Fly", base);
	Wind2Fly();

	team = iTeam;
	SetColorDw(rgb);

	// bei Dunkelheit besser zu sehen
	var tmp = AddLightAmbience(30,this());
	var r,g,b,a;
	SplitRGBaValue(rgb,r,g,b,a);
	tmp->ChangeColor(RGBa(r,g,b,a+30));
	SetVisibility(VIS_All,tmp);

	AddEffect("Collect", this(), 101, 5, this());
}

/* Einsamml0rn!!11 */

protected func Collected(pClonk)
{
	Sound("Flag");

	//Log("$HasTheFlag$", GetTaggedPlayerName(GetOwner(pClonk)), GetTaggedTeamName(team));
	HUDMessage(Format("$HasTheFlag$", GetTaggedPlayerName(GetOwner(pClonk)), GetTaggedTeamName(team)),-1);
	SetAction("Attach", pClonk);

	cteam = GetPlayerTeam(GetOwner(pClonk));

	// game call: FlagCaptured(flagTeam, captureTeam, clonk)
	// flagTeam: The team to which the flag belongs to
	// captureTeam: The team that captured the flag
	// clonk: the clonk who did it
	GameCallEx("FlagCaptured",team, GetPlayerTeam(GetOwner(pClonk)), pClonk);
	return(1);
}

protected func CheckCollect(pClonk)
{
	if(WildcardMatch(GetAction(), "*Attach*")) return();
	if(GetPlayerTeam(GetOwner(pClonk)) == team)
	{
		if(GetAction() ne "Lost")
			CheckFlag(pClonk);
		else
			Return2Base(pClonk);
		return();
	}
	if(FindObject(GetID(), 0,0,0,0,0, 0, pClonk)) return();	// Mehr als 2 Teams ftw!

	return(Collected(pClonk));
}

protected func AttachTargetLost()
{
	//Log("FlagLost!!");
	SetAction("Lost");
	SetActionTargets();
	SetDir();
	if(GetR() > 0) SetDir(1);
	// falls sie festsitzt, wird sie sofort zurückgebracht
	if(GBackSolid())
	{
		RemoveObject();
	return();
	}
	HUDMessage(Format("$FlagLost$", GetTaggedTeamName(team)),-1);
	//Log("$FlagLost$", GetTaggedTeamName(team));
	GameCallEx("FlagLost",team);
}

protected func LostCounter()
{
	lost++;
	if(lost > 30)
	{
		//HUDMessage(Format("$FlagReturned$", GetTaggedPlayerName(GetOwner(pClonk)) ),-1);
		//Return2Base(0,true);
		// game call: FlagReturned(flagTeam)
		// flagTeam: The team to which the flag belongs to
		GameCallEx("FlagReturned",team);
		//Log("$FlagReturned$", GetTaggedTeamName(team));
		HUDMessage(Format("$FlagReturned$", GetTaggedTeamName(team) ),-1);
	}
}
/* Zurückfliegen und Punkte vergeben */

public func IsAtHome() {
	if(GetActionTarget() == base)
		return(true);
}

public func GetCarrier() {
	if(WildcardMatch(GetAction(), "*Attach*")) {
			return(GetActionTarget());
	}
}

protected func Return2Base(pClonk, nolog)
{
	lost=0;
	doreturn = false;
	//Log("Return2Base");
	SetR();
	SetRDir();
	SetAction("Fly", base);
	if(!nolog && pClonk)
		HUDMessage(Format("$ReturnedTheFlag$", GetTaggedPlayerName(GetOwner(pClonk)) ),-1);
		//Log("$ReturnedTheFlag$", GetTaggedPlayerName(GetOwner(pClonk)));
	// game call: FlagReturned(flagTeam, clonk)
	// flagTeam: The team to which the flag belongs to
	// clonk: the clonk who did it
	GameCallEx("FlagReturned",team, GetPlayerTeam(GetOwner(pClonk)));
}
/*
protected func CheckFlag(pClonk)
{
	var flag = FindObject(GetID(), 0,0,0,0,0, 0,pClonk);
	if(!flag) return();

	HUDMessage(Format("$CapturedTheFlag$", GetTaggedPlayerName(GetOwner(pClonk)) ),-1);
	//Log("$CapturedTheFlag$", GetTaggedPlayerName(GetOwner(pClonk)));
	DoWealth(GetOwner(pClonk), 50); // Geld!
	DoTeamScore(GetPlayerTeam(GetOwner(pClonk)), 1);
	flag->~Return2Base(0,1);
	// game call: FlagScored(flagTeam, scoreTeam, clonk)
	// flagTeam: The team to which the flag belongs to
	// scoreTeam: the team that just scored
	// clonk: the clonk who did it
	GameCallEx("FlagScored",team, GetPlayerTeam(GetOwner(pClonk)), pClonk);
}
*/

protected func CheckFlag(pClonk)
{
  var flag = FindObject(GetID(), 0,0,0,0,0, 0,pClonk);
  if(!flag) return;

  if(pClonk) HUDMessage(Format("$CapturedTheFlag$", GetTaggedPlayerName(GetOwner(pClonk))),-1);
  DoWealth(GetOwner(pClonk), 50); // Geld!
  DoTeamScore(GetPlayerTeam(GetOwner(pClonk)), 1);
  // game call: FlagScored(flagTeam, scoreTeam, clonk)
  // flagTeam: The team to which the flag belongs to
  // scoreTeam: the team that just scored
  // clonk: the clonk who did it
  GameCallEx("FlagScored",team, GetPlayerTeam(GetOwner(pClonk)), pClonk);
  // Flagge danach zurückkehren lassen, damit der sound nicht kommt
  if(flag) flag->~Return2Base(0,1);
}


// verbessertes Einsammeln
public func FxCollectTimer(target, no)
{
	if(WildcardMatch(GetAction(target),"*Attach*")) return();
	var clonk;
	while(clonk = FindObject(0, -20, -20, 40, 40, OCF_CrewMember(), 0,0, NoContainer(), clonk))
		if(clonk->GetOCF() & OCF_Alive())
			if(CheckCollect(clonk))
				return();
}

/* Kaputt! */

public func Destruction()
{
	if(g_bRoundRestarted) return; // dann soll sie auch weg!
	// Flaggen dürfen nicht einfach weg sein
	var nFlag = CreateObject(FLA2, 0,0, GetOwner());
	SetPosition(GetX(),GetY(),nFlag);
	nFlag->Activate(base, team, GetColorDw());
	if(!doreturn)
	{
		nFlag->LocalN("doreturn")=true;

		if(GetR() < 0)
			nFlag->SetR(-30);
		else
			nFlag->SetR(30);

		nFlag->~AttachTargetLost();
	}
	else
	{
		// game call: FlagReturned(flagTeam)
		// flagTeam: The team to which the flag belongs to
		GameCallEx("FlagReturned",team);
		//Log("$FlagReturned$", GetTaggedTeamName(team));
		HUDMessage(Format("$FlagReturned$", GetTaggedTeamName(team) ),-1);
	}
}

public func NoWarp() { return(1); }

public func GetBase() { return(base); }

public func GetTeam() { return (team); }
