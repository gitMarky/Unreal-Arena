#strict 2
#include SHT_

public func LaunchLaser( object pObj, int iAngle, int iDmg, int iDmgType )
{
	//SetClrModulation(RGBa(80,0,255,255));

		SetCategory(1);
		//SetAction("Exist");

		iDamage = iDmg;
		DMG_Type = iDmgType;

		var me = ObjectNumber(this);

		var beam = CreateObject( _LSR,0,0,GetOwner() );

		SetPosition( GetX( pObj ),GetY( pObj ), beam );

		SetVisibility(VIS_None);

		//iXDir =+Sin(iAngle,200);
		//iYDir =-Cos(iAngle,200);
		iXDir =+Sin(iAngle,150);
		iYDir =-Cos(iAngle,150);

		beam->~Set( iAngle, 1, 2000, 10, this, 0, 10, RGBa(255,200,0));
		beam->~SetUser( pObj );

}


public func LaserStrike( pObj )
{
	return HitObject( pObj );
}

public func LaserRemoved()
{
	RemoveObject();
}


private func HitObject(object pObject)
{
	bNoRemove = true;
	SetXDir(iXDir);
	SetYDir(iYDir);
	return _inherited( pObject );
}
