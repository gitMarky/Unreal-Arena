
#appendto Goal_CaptureTheFlagEx

public func DoSetup(int round)
{
	SetFlagBase(1, +347, 590);
	SetFlagBase(2, -347 + LandscapeWidth(), 590);
	
	var flag_team1 = FindObject(Find_ID(GetFlagID()), Find_Func("FindTeam", 1));
	var flag_team2 = FindObject(Find_ID(GetFlagID()), Find_Func("FindTeam", 2));

	//flag_team1->SetClrModulation(team_color[1]);
	//flag_team2->SetClrModulation(team_color[2]);
//	flag_team1->SetClrModulation(GetTeamColor(1));
//	flag_team2->SetClrModulation(GetTeamColor(2));
	flag_team1->SetColor(GetTeamColor(1));
	flag_team2->SetColor(GetTeamColor(2));
	
//	DebugLog("Setup round %d", round);

	_inherited(round);
}

public func DoCleanup(int round)
{
	RemoveAll(FindObjects(Find_ID(GetFlagBaseID())));
	RemoveAll(FindObject(Find_ID(GetFlagID())));
}
