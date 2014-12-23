/*-- More Gore --*/

#strict 2

global func MOD_MoreGore(){ return ObjectCount( ID_Rule_MoreGore ); }

protected func Activate(iPlr)
{
  MessageWindow(GetDesc(),iPlr);
}

// Kann mittels des Spielzielauswählers ausgewählt werden
public func IsChooseable() { return true; }

public func RuleForbid(){ return [ ID_Rule_NoBlood ]; }

/* Zeugs, das herumliegt, soll verschwinden */
