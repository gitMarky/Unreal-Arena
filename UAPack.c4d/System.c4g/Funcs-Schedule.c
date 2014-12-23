/*-- Neues Script --*/

#strict 2

global func FxIntScheduleTimer(object pObj,  int iEffect)
{
	if(!pObj) return -1; // mich wundert, dass das noch niemandem aufgefallen ist!
	if(g_bRoundRestarted) return -1; // Das gibt sonst so viele Fehler
	// dummerweise kommt ein gelöschtes Objekt im Call vor :/, bei Neustart der Runde
	  // Nur eine bestimmte Anzahl Ausführungen
	  var fDone = (--EffectVar(1, pObj, iEffect) <= 0);
	  // Ausführen
	  eval(EffectVar(0, pObj, iEffect));
	  return(-fDone);
}
