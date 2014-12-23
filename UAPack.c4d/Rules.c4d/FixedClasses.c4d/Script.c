/*-- No Blood --*/

#strict 2

global func MOD_FixedEquipment(){ return ObjectCount( ID_Rule_FixedEquipment ); }

protected func Activate(iPlr)
{
  MessageWindow(GetDesc(),iPlr);
}

// Kann mittels des Spielzielauswählers ausgewählt werden
//public func IsChooseable() { return true; } // hat noch keine Funktion
public func RuleRequired() { return [ ID_Rule_ClassSelection ]; }
