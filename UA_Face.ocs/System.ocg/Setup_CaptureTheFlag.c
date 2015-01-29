
#appendto Goal_CaptureTheFlagEx

public func DoSetup(int round)
{
	var team_color = [nil, RGB(0, 0, 255), RGB(255, 0, 0)];

	SetFlagBase(1, +347, 590);
	SetFlagBase(2, -347 + LandscapeWidth(), 590);
	
	var flag_team1 = FindObject(Find_ID(GetFlagID()), Find_Func("FindTeam", 1));
	var flag_team2 = FindObject(Find_ID(GetFlagID()), Find_Func("FindTeam", 2));

	flag_team1->SetClrModulation(team_color[1]);
	flag_team2->SetClrModulation(team_color[2]);
}

public func DoCleanup(int round)
{
	RemoveAll(FindObjects(Find_ID(GetFlagBaseID())));
	RemoveAll(FindObject(Find_ID(GetFlagID())));
}