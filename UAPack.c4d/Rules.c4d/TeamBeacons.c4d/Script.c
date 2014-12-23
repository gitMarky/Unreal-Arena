#strict 2

global func MOD_TeamBeacon(){ return ObjectCount( ID_Rule_TeamBeacon ); }

protected func Activate(iPlr)
{
  MessageWindow(GetDesc(),iPlr);
}

// Kann mittels des Spielzielauswählers ausgewählt werden
public func IsChooseable() { return true; }
//public func RuleRequired() { return [ ID_Rule_ClassSelection ]; }
