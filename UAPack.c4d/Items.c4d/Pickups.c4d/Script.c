/*-- Ausrüstung --*/

#strict 2

/* Status */

public func IsPickup() { return true; }

/* Aufnahme */

public func OnCollection( object pCrew )
{
	if(PickupEffect( pCrew )) return true; // Aufsammeln
		//RemoveObject();
	return false; // liegen lassen
}

public func Entrance( object pContainer)
{
	if(pContainer->~IsSpawnPoint()) return;
	Exit(); SetPosition(); SetCategory(1); SetVisibility(VIS_None);
	Schedule("RemoveObject()",1);
	//RemoveObject();
}


/* Callbacks */

public func PickupEffect(object pClonk){ return false; }
