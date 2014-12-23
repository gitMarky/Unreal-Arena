#strict
#include SHT_

local bSecondary;

public func KillMessage(){ return DefinitionCall(W_U6,"KillMessage"); }

private func HitObject(object pObject)
{
	if(bSecondary) return Detonate();
	return _inherited(pObject);
}


private func Hit()
{
	if(bSecondary) return Detonate();

	// Reflektion an Wänden (Eintrittswinkel = Austrittwinkel)
	if(iRefl)
	{
		Sound("Hit*");

		if(iRefl > 4) CastParticles( "ColSpark" ,10,20,0,0,20,40,RGBa(255,50,0,80),RGBa(255,100,0,130));

		SetXDir(GetXDir()/-2);
		SetYDir(GetYDir()/-2);

		// Trail wird entfernt!
		if(pTrail)
		{
			pTrail->SetPosition(GetX(), GetY());
			pTrail->SetSpeed(0,0);
			pTrail->Remove();
		}
		// Und neu erzeugt
		CreateTrail(iTrailSize, iTrailLength);

		// Eine Reflektion weniger
		iRefl--;
		// Und erstmal nicht auftreffen
		return;
	}

	// umliegende Objekte beschädigen.
	var objs = FindObjects(	Find_AtPoint(),
							Find_NoContainer(),
							Find_Or(
								Find_Func("IsBulletTarget",GetID(),this(),shooter),
								Find_OCF(OCF_Alive)
							),
							Find_Func("CheckEnemy",this())
							/*Find_Not(Find_Func("HitExclude"))*/);

	for(var pTarget in objs)
	{
		BulletStrike(pTarget);
	}

	HitObject();
}

public func ContactBottom()
{
	Hit();
}

public func SetSecondary()
{
	bSecondary = true;
	SetGraphics("Grenade");
}

public func Detonate()
{
		//Explosion( this,  32, 24, false );
		Explosion( this,  64, 24, false );

		  for(var i=0; i<9; i++)
		  {
			var ammo;
		    ammo = CreateObject(S_U6, 0, 0, GetOwner());
		    ammo -> Launch(Random(360), 30, 9999, 1, 1, 11 );
		    ammo -> SetBallistic();
		    LocalN("iRefl",ammo) = 5;
		    CreateParticle("ColSparkAttPerm",GetX(ammo)-GetX(),GetY(ammo)-GetY(),0,0,70,RGBa(255,100,0,150),ammo);

		  }

		RemoveObject();
		return true;
}
