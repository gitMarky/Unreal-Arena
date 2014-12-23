#strict 2
#include SHT_

local fExplodes, iHits;

public func KillMessage(){ return DefinitionCall(W_U4,"KillMessage"); }

public func SetSecondary()
{
	fExplodes = true;
}

public func Traveling()
{
	SetRDir( Angle( 0,0, GetXDir(), GetYDir() ) );
	if(GetActTime() > 14 && GetAction() != "TravelBallistic") SetAction("TravelBallistic");

	_inherited();
}

public func Hit()
{
	if( fExplodes )
	{
		Detonate();
	}
	else
	{
		CastParticles("ColSpark",10,20,0,0,20,40,RGBa(255,50,0,80),RGBa(255,100,0,130));

		//if(GetAction() == "Travel" )
		//{
			SetAction( "Idle" );
			SetAction( "Travel" );

			Sound("RipperHit");

			SetXDir(-iXDir ,0,100); //GetXDir());
			SetYDir(-iYDir+150-Random(310),0,100); //GetYDir()+15-Random(31));

			iRefl = 1;

			iHits++;

			_inherited();
		//}
		//else
		if(iHits > 3)
		{
			RemoveObject();
		}
	}
}

public func HitObject( pObj )
{
	if( fExplodes )
		return Detonate();
	else
		return _inherited( pObj );
}

public func Detonate()
{
	if( fExplodes > 1 ) return;
	fExplodes = 2;
		Explosion( this, 24, 10, 0 );

		RemoveObject();
}


private func Color(int iATime)
{
  return RGBa(255,255,255,0);
}

public func TrailID(){ return TRAJ; }

public func TrailColor(int iATime)
{
  return(RGBa(255,255,255,140));
}

public func GlowColor(int iATime) {
  return(RGBa(255,190,0,50));
}
