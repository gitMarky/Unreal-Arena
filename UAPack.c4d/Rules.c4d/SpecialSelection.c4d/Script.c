/*-- No Blood --*/

#strict 2

global func MOD_NoBlood(){ return ObjectCount( ID_Rule_NoBlood ); }

protected func Activate(iPlr)
{
  MessageWindow(GetDesc(),iPlr);
}

// Kann mittels des Spielzielausw�hlers ausgew�hlt werden
//public func IsChooseable() { return true; } // hat noch keine Funktion
