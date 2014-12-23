/*-- Erweiterung des Wegpunkts --*/

// Wegpunkte haben jetzt Info-Sektionen, sodass die Bots auch nur bestimmte Wegpunkt anlaufen können

#strict 2
#appendto WAYP

static const WPInfo_None = 		0; // nix besonderes, einfach nur so drin
static const WPInfo_Defend = 	1; // der Wegpunkt soll von Defendern angelaufen werden
static const WPInfo_Sniper = 	2; // der Wegpunkt soll von Snipern angelaufen werden

// lokale Variablen - die sind nicht Pfadabhängig!!
local iWPInfo;
local iWPInfoTeam;

local aPathLength;
local iWPWeight;


public func SetInfo( int info, int team )
{
	iWPInfo = info;
	iWPInfoTeam = team;
}

public func CheckInfo( int info, int team )
{
	if( iWPInfoTeam && team && iWPInfoTeam != team) return false;

		if( iWPInfo & info ) return true;
	return false;
}

public func Serialize(array& extra)
{
	var cmd;
	for (var i=0;i<GetLength(aPath);++i)
	{
		if(GetPathFlag(i) == 0) cmd = "Path_MoveTo";
		else if(GetPathFlag(i) == 1) cmd = "Path_Jump";
		else if(GetPathFlag(i) == 2) cmd = "Path_Backflip";
		else if(GetPathFlag(i) == 3) cmd = "Path_Lift";
		if (GetPathJetpack(i))
		{
			extra[GetLength(extra)] = Format("AddPath(Object(%%d),%s,%d,%d,%d)", GetPathTarget(i), cmd, GetPathDir(i), GetPathJetpack(i), GetPathJetpackFlag(i));
		}
		else
		{
			extra[GetLength(extra)] = Format("AddPath(Object(%%d),%s,%d)", GetPathTarget(i), cmd, GetPathDir(i));
		}
		for (var j=0;cmd=GetArriveCommand(i,j,0);++j)
		{
			if (GetArriveCommand(i,j,4))
			{
				// TODO: any Data für AddCommand in der KI kann u.U. auch Objektzeiger beinhalten.. auch serialisieren. :D
				extra[GetLength(extra)]	= Format("SetArriveCommand(%d,%d,\"%s\",Object(%%d),%d,%d,Object(%%d),%v)", i, j, cmd, GetArriveCommand(i,j,1), GetArriveCommand(i,j,2), GetArriveCommand(i,j,3), GetArriveCommand(i,j,4), GetArriveCommand(i,j,5));
			}
			else
			{
				extra[GetLength(extra)]	= Format("SetArriveCommand(%d,%d,\"%s\",Object(%%d),%d,%d,0,%v)", i, j, cmd, GetArriveCommand(i,j,1), GetArriveCommand(i,j,2), GetArriveCommand(i,j,3), GetArriveCommand(i,j,5));
			}
		}
		if( iWPInfo )
		{
				extra[GetLength(extra)] = Format("SetWPInfo(%d,%d)",iWPInfo,iWPInfoTeam);
		}
	}
}



// Schreibt ein Wegpunktscript in den Enginelog
global func LogWaypoints()
{
	var aWaypoints = FindObjects(Find_ID(WAYP));

	var i = 0;
	for(var pWp in aWaypoints)
		Log("	var wp%d = CreateWP(%d,%d);", ++i, pWp->GetX(), pWp->GetY());

	i = 0;
	for(pWp in aWaypoints)
	{
		i++;
		for(var j = 0,eo ; j < pWp->WAYP::GetPathCount() ; j++)
		{
			eo = Waypoints_GetArrayPosition(aWaypoints, pWp->WAYP::GetPathTarget(j));
			if(i < 0)
			{
				Log("// Path error: no target!");
				continue; // Pfad kann man nicht auswerten
			}

			var flag = pWp->WAYP::GetPathFlag(j);
			var strFlag = Format("%d",flag);
			if(flag == 0) strFlag = "Path_MoveTo";
			if(flag == 1) strFlag = "Path_Jump";
			if(flag == 2) strFlag = "Path_Backflip";
			if(flag == 3) strFlag = "Path_Lift";

			if(pWp->WAYP::GetPathJetpack(j))
				Log("	wp%d -> AddPath(wp%d, %s, %d, %d, %d);", i,eo, strFlag, pWp->WAYP::GetPathDir(j), pWp->WAYP::GetPathJetpack(j), pWp->WAYP::GetPathJetpackFlag(j));
			else
				Log("	wp%d -> AddPath(wp%d, %s, %d);", i,eo, strFlag, pWp->WAYP::GetPathDir(j));

			var cmd;
			for(var z = 0; cmd = pWp->WAYP::GetArriveCommand(j,z,0,true);z++)
			{
				var obj = pWp->WAYP::GetArriveCommand(j,z,1,true);
				if(GetType(obj) == C4V_C4Object) obj = Format("Object(%d)",ObjectNumber(obj));
				Log("	wp%d -> SetArriveCommand(%d, %d, \"%s\", %v, %d, %d, %v, %v);",i, j,z,cmd, obj, pWp->WAYP::GetArriveCommand(j,z,2,true), pWp->WAYP::GetArriveCommand(j,z,3,true), pWp->WAYP::GetArriveCommand(j,z,4,true),pWp->WAYP::GetArriveCommand(j,z,5,true));
			}

		}
		if( LocalN("iWPInfo",pWp) )
		{
				Log("	wp%d -> SetInfo(%d,%d);",i,LocalN("iWPInfo",pWp),LocalN("iWPInfoTeam",pWp));
		}
		Log("	wp%d -> SetName(\"wp%d\");", i, i);
	}
}


public func SetPathTarget(int pId, object target)
{
	_inherited(pId, target);

	if(target)
	{
		aPathLength[pId] = ObjectDistance(target);
	}
}

// damit man z.B. Teleportern etc. einen geringeres Pfadgewicht geben kann
public func SetPathLength(int pId, int iLength)
{
	return aPathLength[pId] = iLength;
}

public func GetPathLengthWP(int pId)
{
	return aPathLength[pId];
}

public func GetPathID(object target)
{
	if (target == 0 || target == this) return -1;

	for(var i = 0; i < GetPathCount(); i++)
	{
		if (GetPathTarget(i) == target)
			return i;
	}

	return -1;
}

protected func Initialize()
{
	aPathLength = CreateArray();
	_inherited();
}


// Wurde erweitert, damit sich das Aufrufende Objekt selbst übergeben kann
/*public func GetArriveCommand(int pId, int iNo, int iElement, bool NoLog, object pCaller) {
  if(GetPathCount() <= pId)
  {
    if(NoLog)
      return();
    return(ErrorLog(WAYP_PathErr, pId));
  }
  if(iElement < 0 || iElement > 5) return;
  if(!EffectVar(iNo + 5, this(), aPath[pId])) return;

  var val = EffectVar(iNo + 5,this(),aPath[pId])[iElement];
  if(val == "self") val = pCaller;
  return val;
}*/


public func Test()
{
  // alle Pfade
  for(var i; i<GetLength(aPath); ++i)
  {
	var hzck = CreateObject(UBOT,0,10,-1);
	var jtpk = hzck->CreateContents(JTPK);
	hzck->DoAmmo(GSAM,500);
	jtpk->Activate(hzck);
	hzck->SetLocal(13,GetPathTarget(i));
	//hzck->SetMacroCommand(this(),"Wait",0,0,0,40);
	hzck->SetMacroCommand(this, "MoveTo", GetPathTarget(i));
  }
}
