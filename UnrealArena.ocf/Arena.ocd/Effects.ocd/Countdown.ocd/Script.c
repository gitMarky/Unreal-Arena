/**
 Round countdown. Counts down the time until the round starts. 
 @author Marky
 @version 0.1.0
*/

public func Initialize()
{
	RoundManager()->RegisterRoundStartBlocker(this);
	AddEffect("CountdownRound", this, 1, 36, this);
}

private func FxCountdownRoundStart(object target, proplist effect, int temporary)
{
	if (temporary) return;

	effect.counter = 10;
}

public func FxCountdownRoundTimer(object target, proplist effect, int time)
{
	for (var i=0; i<GetPlayerCount(C4PT_User); ++i)
	{
		var player = GetPlayerByIndex(i, C4PT_User);
		//var crew = GetHiRank(player);

		if (effect.counter)
		{
			//UA_Announcer(Format("an_count_%d",g_iCountDown),crew,true);
			CustomMessage(Format("@%d...", effect.counter), nil, player, 0, 0, RGB(200, 0, 0));
		}
		else
		{
			CustomMessage("", nil, player);
		}
	}

	// remove?
	if (effect.counter <= 0)
	{
		return FX_Execute_Kill;
	}

	--effect.counter;
	return FX_OK;
}

private func FxCountdownRoundStop(object target, proplist effect, int reason, bool temporary)
{
	if (temporary) return;

	RoundManager()->RemoveRoundStartBlocker(target);
	target->RemoveObject();
}
