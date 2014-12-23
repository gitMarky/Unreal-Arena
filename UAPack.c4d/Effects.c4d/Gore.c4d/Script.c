/*--- Gore ---*/

#strict 2

local szType;

public func Hit()
{
	if(szType == "Machine")
		Sound("Hit*");
	else
		Sound("gibP*");
}

public func Launch( string type )
{
	szType = type;
	SetGraphics(type);
}

public func Initialize()
{
  	SetAction("Flying");
	SetDir(Random(5));
}

public func Splatter()
{
	if(MOD_NoBlood())  return RemoveObject();
	//if(GetActTime()>50) if(FindObject(MH6I)) return(RemoveObject());
	if(GetActTime()>200)
	{
		if(!IsDisintegrating()) Disintegrate(this,20,50,-10);
		return;
		//return RemoveObject();
	}

	CreateParticle("Blood",0,0,0,0,10+Random(30),RGBaRandom(BloodFXColor(szType)[0],BloodFXColor(szType)[1]));
}

public func ContactBottom()
{
 	if(!Random(3)) SetXDir(GetXDir()/2);
 	SetYDir(GetYDir()/-2);
 	return(PhysicalJumping());
}
public func ContactTop()
{
 	SetYDir(GetYDir()/-2);
 	return(PhysicalJumping());
}
public func ContactLeft()
{
	SetXDir(GetXDir()/-2);
	return(PhysicalJumping());
}
public func ContactRight()
{
	SetXDir(GetXDir()/-2);
	return(PhysicalJumping());
}

public func PhysicalJumping()
{
	SetRDir(GetRDir()*-2);
  	if(Or(GBackSolid(5,0),GBackSolid(-5,0))) SetXDir(GetXDir()/-2);
  	if(Or(GBackSolid(0,5),GBackSolid(0,-5))) SetYDir(GetYDir()/-2);
  	if(Or(GBackSolid(10,0),GBackSolid(-10,0))) SetXDir(GetXDir()/-2);
  	if(Or(GBackSolid(0,10),GBackSolid(0,-10))) SetYDir(GetYDir()/-2);
  	return(1);
}
