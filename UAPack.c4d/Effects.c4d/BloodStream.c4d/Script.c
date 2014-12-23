#strict 2

public func ContactBottom()
{
	RemoveObject();
}

public func Splatter()
{
	if(MOD_NoBlood()) return RemoveObject();
	//if(FindObject(MH6I)) if(GetActTime()>2) return(RemoveObject());
	CreateParticle("Blood",0,0,0,0,10+Random(30),RGBaRandom(BloodFXColor(szType)[0], BloodFXColor(szType)[1] ));
}

public func Initialize(){ SetAction("Sparkle");}

local szType;

public func Launch( string type )
{
	szType = type;
}
