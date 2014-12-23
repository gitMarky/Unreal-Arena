/*-- No Blood --*/

#strict 2

global func MOD_NoBlood(){ return ObjectCount( ID_Rule_NoBlood ); }

protected func Activate(iPlr)
{
  MessageWindow(GetDesc(),iPlr);
}

// Kann mittels des Spielzielauswählers ausgewählt werden
public func IsChooseable() { return true; }
public func RuleForbid(){ return [ ID_Rule_MoreGore ];}
