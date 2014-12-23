/*-- LowGravity --*/

#strict 2

global func MOD_LowGravity(){ return ObjectCount( ID_Rule_LowGravity ); }

protected func Activate(iPlr)
{
  MessageWindow(GetDesc(),iPlr);
}

// Kann mittels des Spielzielauswählers ausgewählt werden
public func IsChooseable() { return true; }


static g_iGravity;

public func Initialize()
{
	if(!g_iGravity) g_iGravity = GetGravity();

	SetGravity(g_iGravity/2);
	SetPosition();
}

public func Destruction()
{
	SetGravity( g_iGravity );
}
