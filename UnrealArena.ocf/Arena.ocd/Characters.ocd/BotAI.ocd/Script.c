#include AI


func AddAI(object clonk)
{
	var fx = AI->AddAI(clonk);
	if (fx)
	{
		clonk.ExecuteAI = BotAI.Execute;
		fx.ai = BotAI;
		fx.ignore_allies = true;
	}
	return fx;
}


func Execute(proplist fx, int time)
{
	// Do nothing!
	return;
}
