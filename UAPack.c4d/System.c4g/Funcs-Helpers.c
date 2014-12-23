/*-- Neues Script --*/

#strict 2

global func RGBaRandom( int iCol1, int iCol2)
{
	var r1,g1,b1,a1,r2,g2,b2,a2;
	//
	r1 = GetRGBaValue(iCol1,1);
	g1 = GetRGBaValue(iCol1,2);
	b1 = GetRGBaValue(iCol1,3);
	a1 = GetRGBaValue(iCol1,0);
	//
	r2 = GetRGBaValue(iCol2,1);
	g2 = GetRGBaValue(iCol2,2);
	b2 = GetRGBaValue(iCol2,3);
	a2 = GetRGBaValue(iCol2,0);
	//
	var r,g,b,a,x;
	x = 1+Random(100);

	r = BoundBy((x*r1 + (100-x)*r2)/100,0,255);
	g = BoundBy((x*g1 + (100-x)*g2)/100,0,255);
	b = BoundBy((x*b1 + (100-x)*b2)/100,0,255);
	a = BoundBy((x*a1 + (100-x)*a2)/100,0,255);

	return RGBa(r,g,b,a);
}


global func Fraction( int iVal1, int iVal2, int iVal3)
{
  /* gibt iVal1/iVal2 von iVal3 zurück
     Beispiel: iVal1=2 iVal2=3 iVal3=6 gibt dann 2/3 von 6
     zurück, also 4 */
	return((iVal3*iVal1)/iVal2);
}


global func SmokeX(int _x, int _y, int con, int move_style, int time, int dwColor1, int dwColor2,int effect_style)
{
	return (CreateObject(ID_Effect_Smoke,0,0,-1)->~Activate(GetX()+_x,GetY()+_y,con/2,move_style,time,dwColor1,dwColor2,effect_style));
}


global func Explosion( object pObj, int iDamage, int iRadius, bool fSmoke, string szSound, bool fNoSprites, object shooter )
{
	var obj,helper,id,i;
	if(!shooter) shooter = this;

	var targets = FindObjects(  Find_Distance(iRadius,GetX(pObj)-GetX(),GetY(pObj)-GetY()),
								Find_Exclude(pObj),
								Find_NoContainer(),
								Find_Or(Find_Func("IsBulletTarget",XPLO,pObj,shooter), Find_OCF(OCF_Alive)),
								Find_Not(Find_Func("HitExclude")) );

	for( obj in targets)
	{
		//var rad = iRadius + Distance(0,0,GetObjWidth(obj),GetObjHeight(obj));
		//if(ObjectDistance(pObj,obj) > rad) continue;


		var help = CreateObject(SHT_,0,0,-1);
		if(help) help->SetPosition( GetX(obj)-5+Random(11), GetY(obj)-10+Random(21));

		var iAngle, iSpeed, iDist;
		var explo = GetExplosionSpeed(pObj,obj,iRadius);
		SetSpeed( explo[0], explo[1], obj );


		iAngle = Angle(0,0,explo[0],explo[1]);
		iSpeed = Distance(0,0,explo[0],explo[1]);

		var percent = Sin( BoundBy(90*(iRadius-ObjectDistance(help,obj))/iRadius,0,90) ,100);

		if(help) help->Launch(iAngle, iSpeed, 1000, 1, 1, iDamage*percent/100,DMG_Explosion);

		LocalN("bNoRemove",help) = true;
		if(help) help->BulletStrike(obj);
		//Log("Hit Object %d, %d, %d",ObjectNumber(obj),BoundBy(iDamage-ObjectDistance( help, obj ),0,iDamage), iRadius);
		//Schedule(Format("RemoveObject(Object(%d))",ObjectNumber(help)),1);
		Schedule("RemoveObject()",1,0,help);
	}

	// -------------------------------------------------------------------------
	// Raucheffekte
	// -------------------------------------------------------------------------

	if( fSmoke )
	{
		// Rauchstrahlen
		//for( var ang = 0; ang <= 360; ang+=60 )
		//for ( i=0; i < 6; i++)
		//{
		//	CreateSmokeTrail(iRadius, Random(360)/*ang+RandomX(-40,40)*/, 0, 0, pObj,true);
		//}

		// Rauchstrahlen
/*
		CastObjects(ID_Effect_SmokeStack,6,100);

		//while(obj=FindObject(EG6K,-2,-2,4,4,0,0,0,0,obj))
		for(obj in FindObjects( Find_ID(ID_Effect_SmokeStack), Find_Distance(2) ))
			obj->Launch(25*iRadius/10);
*/
		CastObjectsCall( [ID_Effect_SmokeStack,6,100], "Launch", 25*iRadius/10 );

		// Toller Rauch

		for(i=0; i<3; i++)
		SmokeX(
			(-iRadius+Random(iRadius*2))/2,
			(-iRadius+Random(iRadius*2))/2,
			iRadius*5,2,15*iRadius/10,RGB(60,60,60),RGB(20,20,20),1);


		// Flammen wegschießen

		for( i=0; i < 3; i++)
		{
			obj = CreateObject(ID_Gore_Flame,0,0,-1);

			var rot, dist;

			rot = Random(360);
			dist = iRadius + Random(30);

			obj->~SetUpSpark( iDamage + Random(20), +Sin(rot,dist), -Cos(rot,dist));
		}
	}

	// -------------------------------------------------------------------------
	// Beleuchtungseffekte
	// -------------------------------------------------------------------------

	if(!fNoSprites)
	{

		var con, rad, size, x,rot,rrd,shock,shockr;

		con = 100*iRadius/32;

		rad = 16*con/100;
		size = 64*con/20;

		shock = 48*con/100;
		shockr = 72*con/100;

		var col_center, col_amb1, col_amb2;

		if(!(col_center = pObj->~ExplosionColCenter() )) col_center = RGBa(255,50+Random(50),0,80+Random(50));
		if(!(col_amb1 = pObj->~ExplosionColAmb1() )) col_amb1 = RGBa(255,50,0,80);
		if(!(col_amb2 = pObj->~ExplosionColAmb2() )) col_amb2 = RGBa(255,100,0,130);

		for( x=0; x<5; x++ )
		{
			rot = Random(360);
			rrd = Random(rad);

			CreateParticle("ColSparkNGSlow", Sin(rot,rrd),-Cos(rot,rrd),Sin(rot,5),-Cos(rot,5), size, col_center );
		}

		// Der da war bei allen Explosionen gleich bis jetzt, lassen wirs mal so ^^
		CreateParticle("ColSparkNoGrav", 0,0,0,0, size, RGBa(220-Random(30),255,0,80+Random(50)));

		CastParticles("ColSpark",shock,shockr,0,0,40,60, col_amb1, col_amb2);
	}

	// -------------------------------------------------------------------------
	// Sonstiges
	// -------------------------------------------------------------------------

	var helper = CreateObject(TIM2,0,0,-1);
	SetPosition(GetX(pObj),GetY(pObj),helper);
	ShakeViewPort( iRadius, helper);

	if( szSound )
	{
		Sound( szSound );
	}
	else
	{
		Sound("Blast*");
	}


}

global func GetExplosionSpeed( pCauser, pObj, iRadius)
{
	var x,y;

	if( GetX( pCauser ) < GetX( pObj )) x = GetX( pCauser )-GetX( pObj )+iRadius*2;
	if( GetX( pCauser ) > GetX( pObj )) x = GetX( pCauser )-GetX( pObj )-iRadius*2;
	if( GetY( pCauser ) < GetY( pObj )) y = GetY( pCauser )-GetY( pObj )+iRadius*2;
	if( GetY( pCauser ) > GetY( pObj )) y = GetY( pCauser )-GetY( pObj )-iRadius*2;

	//SetSpeed( x, y, pTarget );

	//ObjectCall( pTarget,"SetMaster",LocalN("owner",pCauser));
	return [x,y];
}


global func DoRGBaValue(val, chng, sel)
{
	return (val + (chng<<((3-sel)*8)));
}

global func CastObjectsCall( aCastObjects, szCall, Par2, Par3, Par4, Par5, Par6, Par7, Par8, Par9 )
{
	// bereits vorhandene Objekte ausschließen
	var objects = FindObjects( Find_ID(aCastObjects[0]), Find_Distance( aCastObjects[2], aCastObjects[3], aCastObjects[4]));

	CastObjects( aCastObjects[0],
				 aCastObjects[1],
				 aCastObjects[2],
				 aCastObjects[3],
				 aCastObjects[4]
	);

	var newobj = FindObjects( Find_ID(aCastObjects[0]), Find_Distance( aCastObjects[2], aCastObjects[3], aCastObjects[4]));

	var pcall;
	for( pcall in newobj )
	{
		if(GetLength(objects))
			if(GetArrayItemPosition(pcall, objects) >= 0) continue; // die alten Objekte übergehen
		ObjectCall( pcall, szCall, Par2, Par3, Par4, Par5, Par6, Par7, Par8, Par9 );
	}
}
