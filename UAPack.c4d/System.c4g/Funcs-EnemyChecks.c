#strict 2

global func UTBotAIFindEnemies( object pObj, int iRadius, bool disregardLineOfSight )
{
	if(!pObj)
		pObj = this;

	var pT;
	var targets = [];
	
	var x = GetX(pObj);
	var y = GetY(pObj);

	var preTargets = FindObjects(	Find_Distance(iRadius, AbsX(x), AbsY(y)),
									Find_Exclude(pObj),
									Find_NoContainer(),
									Find_Or( Find_Func("IsBulletTarget",GetID(pObj),0,pObj),
											 Find_OCF(OCF_Alive)),
									Sort_Distance());
	
	for(pT in preTargets)
	{
		var ox = GetX(pT);
		var oy = GetY(pT);
	
		if(!CheckEnemy(pT,pObj, true))
		{
			continue;
		}
	
		// Pfad frei
		if(!disregardLineOfSight)
			if(!PathFree(x,y,ox,oy))
				continue;
		
		// unsichtbare Ziele
		if(!CheckVisibility(pT, pObj))
			continue;
		
		// dann rein in Ergebnismenge...
		targets[GetLength(targets)] = pT;
	}
	
	return targets;
}
