/*-- schnelle Projektile --*/

#strict 2
	//Kann mittels des Spielzielausw�hlers ausgew�hlt werden
public func IsChooseable()	{return true;}
public func RuleForbid(){ return [ IGIB ];}


protected func Activate(iPlr)
{
  MessageWindow(GetDesc(),iPlr);
}

global func MOD_FastBullets()
{
	return ObjectCount(ID_Rule_FastBullets);
}
