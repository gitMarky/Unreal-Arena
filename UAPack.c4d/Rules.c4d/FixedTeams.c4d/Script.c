#strict 2

global func MOD_FixedTeams(){ return ObjectCount( ID_Rule_FixedTeams ); }

protected func Activate(iPlr)
{
  MessageWindow(GetDesc(),iPlr);
}

// Kann mittels des Spielzielauswählers ausgewählt werden
public func IsChooseable() { return true; }
//public func RuleRequired() { return [ ID_Rule_ClassSelection ]; }
