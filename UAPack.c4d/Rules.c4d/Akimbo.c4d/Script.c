/*-- Akimbo Guns --*/

#strict 2

global func MOD_Akimbo(){ return ObjectCount( ID_Rule_Akimbo ); }

protected func Activate(iPlr)
{
  MessageWindow(GetDesc(),iPlr);
}

// Kann mittels des Spielzielausw�hlers ausgew�hlt werden
public func IsChooseable() { return true; }
