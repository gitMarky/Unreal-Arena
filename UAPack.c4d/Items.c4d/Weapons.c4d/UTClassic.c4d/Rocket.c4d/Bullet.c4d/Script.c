#strict 2
#include SHT_


public func KillMessage(){ return DefinitionCall(W_U7,"KillMessage"); }

public func ExplosionRadius(){ return 32;}
public func ExplosionDamage(){ return 140;} // war 46

local bGrenade;
local bDetonated;

public func CreateTrail(){}

public func SetGrenade()
{
	bGrenade = true;
	SetGraphics("Grenade");

	SetR(Random(360));
	SetRDir(Random(30));

	ScheduleCall(this,"Detonate",70);

}

private func TravelingEx()
{
	if(!bGrenade) HandleTrail();

	// außerhalb der Landschaft: löschen
	if(GetY()<0) return(Remove());
}


public func HandleTrail()
{
/*	var dist = Distance(0,0,GetXDir(),GetYDir());
	var maxx = +Sin(GetR(),dist/10);
	var maxy = -Cos(GetR(),dist/10);
	var ptrdist = 50;

	for(var i=0; i<dist; i+=ptrdist)
	{

		var x = -maxx*i/dist;
		var y = -maxy*i/dist;


		var rand = RandomX(-5,5);
		var xdir = +Sin(GetR()+rand,20);
		var ydir = -Cos(GetR()+rand,20);

		CreateParticle("Thrust",x,y,GetXDir()/2,GetYDir()/2,RandomX(40,60),RGBa(255,200,200,60),0,0);
		CreateParticle("Smoke2",x,y,xdir,ydir,RandomX(60,70),RGBa(220,200,180,0),0,0);
	}
	*/
	  var dist = Distance(0,0,GetXDir(),GetYDir());
	  var maxx = +Sin(GetR(),dist/10);
	  var maxy = -Cos(GetR(),dist/10);
	  var ptrdist = 50;

	  for(var i=0; i<dist; i+=ptrdist) {

	    var x = -maxx*i/dist;
	    var y = -maxy*i/dist;


	    var rand = RandomX(-5,5);
	    var xdir = +Sin(GetR()+rand,20);
	    var ydir = -Cos(GetR()+rand,20);

	    //CreateParticle("Thrust",x,y,GetXDir()/2,GetYDir()/2,RandomX(40,60),RGBa(255,200,200,60),0,0);
	    ////CreateParticle("Smoke2",x,y,xdir,ydir,RandomX(100,120),RGBa(220,200,180,0),0,0);
		CreateParticle("Thrust",x,y,GetXDir()/2,GetYDir()/2,RandomX(40,60),RGBa(255,200,200,60),0,0);
		CreateParticle("Smoke2",x,y,xdir,ydir,RandomX(60,70),RGBa(220,200,180,0),0,0);
	  }

}

public func HitObject(object pObject)
{
	if(!bGrenade || pObject) Detonate();
	return true;
}

public func Detonate()
{
	if(bDetonated) return;
	bDetonated = true;
	Explosion( this, ExplosionDamage(), ExplosionRadius(), 1);

	Schedule("RemoveObject()",1,0,this);//RemoveObject();
}

public func ContactBottom()
{
	if( bGrenade )
	{
		Sound("Hit*");
		if( GBackSolid(5,0) || GBackSolid(-5,0) ) SetXDir(GetXDir()/-2);
		if( GBackSolid(0,5) || GBackSolid(0,-5) ) SetYDir(GetYDir()/-2);
 	}
	else
	{
		Detonate();
	}
}
