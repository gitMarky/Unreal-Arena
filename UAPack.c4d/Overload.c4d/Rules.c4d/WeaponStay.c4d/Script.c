/*-- Weapon Stay --*/

#strict

protected func Activate(iPlr) {
  MessageWindow(GetDesc(),iPlr);
}

protected func Initialize() {
  for(var pSpawn in FindObjects(Find_ID(SPNP)))
    pSpawn->Initialized();
}

// Kann mittels des Spielzielausw�hlers ausgew�hlt werden
public func IsChooseable() { return(1); }
