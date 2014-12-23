/*-- No Blood --*/

#strict 2

global func MOD_HalfDamage(){ return ObjectCount( ID_Rule_HalfDamage ); }

protected func Activate(iPlr)
{
  MessageWindow(GetDesc(),iPlr);
}

// Kann mittels des Spielzielauswählers ausgewählt werden
public func IsChooseable() { return true; }
