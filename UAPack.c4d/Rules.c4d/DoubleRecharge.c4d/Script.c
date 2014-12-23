/*-- No Blood --*/

#strict 2

global func MOD_DoubleRecharge(){ return ObjectCount( ID_Rule_DoubleRecharge ); }

protected func Activate(iPlr)
{
  MessageWindow(GetDesc(),iPlr);
}

// Kann mittels des Spielzielauswählers ausgewählt werden
public func IsChooseable() { return true; }
