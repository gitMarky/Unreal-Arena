#strict 2
#include SHT_

local bLaser;


public func KillMessage(){ return DefinitionCall(W_U2,"KillMessage"); }

public func ExplosionRadius( iMode ){ /*var ret = [12,32];*/ return  12 + 20*iMode; }
public func ExplosionDamage( iMode ){ /*var ret = [32,64];*/ return  50 + 50*iMode; }//return( 32 + 32*iMode); }

public func ExplosionColCenter(){ return( RGBa(80,0,255,80+Random(50)) ); }
public func ExplosionColAmb1(){ return( RGBa(80,0,200,80) ); }
public func ExplosionColAmb2(){ return( RGBa(80,0,255,130) ); }

public func GlowColor() {	return RGBa(80,0,255,0); }

public func CreateTrail()
{
	if(!Random(2)) CreateParticle("ColSparkAttach",-2+Random(5),-2+Random(5),0,0,60+Random(20),RGBa(80,0,255,40+Random(50)),this());
}


public func LaunchLaser( object pObj, int iAngle, int iDmg, int iDmgType )
{
	SetClrModulation(RGBa(80,0,255,255));

		SetCategory(1);
		//SetAction("Exist");

		iDamage = iDmg;
		DMG_Type = iDmgType;

		var me = ObjectNumber(this);

		var beam = CreateObject( _LSR,0,0,GetOwner() );

		SetPosition( GetX( pObj ),GetY( pObj ), beam );

		SetVisibility(VIS_None);
		bLaser = true;

		iXDir =+Sin(iAngle,250);
		iYDir =-Cos(iAngle,250);

		beam->~Set( iAngle, 6, 2000, 20, this, 0, 8, RGB(80,0,255));
		beam->~SetUser( pObj );
		beam->~SetSpeed( 200 );

}


public func LaserStrike( pObj )
{
	bNoRemove = true;
	return HitObject( pObj );

	//return true;
}

public func LaserRemoved()
{
	RemoveObject();
}


private func HitObject(object pObject)
{
	bNoRemove = true;
	//Log("GetAction %s",GetAction());
	//if(GetAction() == "Travel")
	if(!bLaser)
	{
		return Detonate();
	}
	else
	{

		return _inherited( pObject );
	}
}


public func Detonate( size)
{
	if(!bLaser)
	{
		bLaser = true;
		//if( GetAction() != "Travel" ) return;
		Explosion( this, ExplosionDamage( size ), ExplosionRadius( size ), 0, "PulseExp*", 0, shooter );
		//Schedule("RemoveObject(this)",1,0,this);//RemoveObject();
		ScheduleCall(this,"LaserRemoved",1);
	}

	return true;
}


public func IsBulletTarget( id idProj, object pProj, object pShooter)
{
	//if((pProj->~GetDamageType()) & DMG_Energy )
	if(!bLaser)
		return true;
	else
		return false;
}

public func HitExclude(){ return false;}
public func CheckEnemy(){ return true;}

public func OnHit(int iDmg, int iType, object pFrom)
{
	//if(!bLaser)
	//{
		Detonate(1);
		//bLaser = true; // h4x
		pFrom->~Detonate();
/*
		Log("OnHit, %d", ObjectNumber(this));
		Explosion( this, ExplosionDamage( 1 ), ExplosionRadius( 1 ), 0, "PulseExp*", 0, shooter );
		//Schedule("RemoveObject(this)",1,0,this);//RemoveObject();
		ScheduleCall(this,"LaserRemoved",1);*/
	//}

}
