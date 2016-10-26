
/**
 Ejects the currently selected weapon, removes the rest of the inventory.
 
 @par projectile The projectile that hit the user.
 */
func DeathThrowWeapon(object projectile)
{
	var weapon = Contents();
	if (weapon)
	{
		weapon->Exit();
		var xdir = -Random(10) + GetXDir();
		var ydir = -Random(10) + GetYDir();
		var rdir = RandomX(-5, 5);
		if (projectile)
		{
		 	xdir += projectile->GetXDir() / 3;
		 	ydir += projectile->GetYDir() / 3;
		 	rdir += (projectile->GetXDir() + projectile->GetYDir()) / 10;
		}
		xdir /= 2;
		ydir /= 2;

		weapon->SetSpeed(xdir, ydir);
		weapon->SetRDir(rdir);
		weapon->SetCategory(C4D_Vehicle); // the weapon should not hurt objects
		//TODO: this should not be necessary Schedule("Disintegrate(0,20,50,-10)", 100, 0, weapon);
	}

	var contents = FindObjects(Find_Container(this));
	for (var item in contents) item->RemoveObject();
}


private func OnDeathExtended(int iDmg, int iType, object pProjectile, bool headshot )
{
	//if(headshot) UA_Announcer( "an_aw_headshot", GetKiller() );
	//DeathAnnounce(GetOwner(),this,GetKiller());
	//NoDeathAnnounce(); // nicht nochmal aufrufen

	//if(GetEffect("NoCorpse",this)) return;
	//AddEffect("NoCorpse",this,50,0,this);
	if(bNoCorpse) return;
	bNoCorpse = true;

	DeathThrowWeapon( pProjectile );

	if(headshot) pProjectile->DoEnergy(-1000,this);

	var divisor = 1+MOD_FastBullets();

	var fBlastWeapon = false;
	if( iType & DMG_Explosion ) {fBlastWeapon = true;Log("BlastAttack");}

	var bodyshot, feetshot;

	//is_dead=1;


	// In einem Fahrzeug? Rausschmeissen!
	if(Contained()->~IsVehicle()) Contained()->~ExitPassenger(this);

	// Regen an?
	//if(MOD_Regeneration()) if(ObjectCall( pProjectile ,"BulletOwner")) DoEnergy(10,ObjectCall( pProjectile ,"BulletOwner"));


	if( fBlastWeapon )
	{
		if(/*Inside(GetX( pProjectile )-GetX(),-7,7) &&*/ Inside(GetY( pProjectile )-GetY(),-6,1)) bodyshot=1;
		if(/*Inside(GetX( pProjectile )-GetX(),-7,7) &&*/ Inside(GetY( pProjectile )-GetY(),1,10)) feetshot=1;
	}

	// Projektil sehr schnell: zurückschleudern;
	if(Abs(GetXDir( pProjectile ))>140)
	{
		SetSpeed(GetXDir()+GetXDir( pProjectile )/(4*divisor),GetYDir()+GetYDir( pProjectile )/(4*divisor));
	}
	else
	{

		SetSpeed(GetXDir()+GetXDir( pProjectile )/(8*divisor),GetYDir()+GetYDir( pProjectile )/(8*divisor));
	}


	// Bloodsplats aktiviert?
	/*if(MOD_BloodSplats())
	{
		CreateBloodsplat(ObjectCall( pProjectile ,"BulletOwner"));
		if(!Random(3)) CreateBloodsplat(ObjectCall( pProjectile ,"BulletOwner"));
		if(!Random(5)) CreateBloodsplat(ObjectCall( pProjectile ,"BulletOwner"));

		if(MOD_MoreGore())
		{
			CreateBloodsplat(ObjectCall( pProjectile ,"BulletOwner"));
			if(!Random(3)) CreateBloodsplat(ObjectCall( pProjectile ,"BulletOwner"));
		}

		if(MOD_BloodSplats()>=2)
		{
			CreateBloodsplat(ObjectCall( pProjectile ,"BulletOwner"));
			if(!Random(3)) CreateBloodsplat(ObjectCall( pProjectile ,"BulletOwner"));
		}
		if(MOD_BloodSplats()>=3)
		{
			CreateBloodsplat(ObjectCall( pProjectile ,"BulletOwner"));
			if(!Random(3)) CreateBloodsplat(ObjectCall( pProjectile ,"BulletOwner"));
		}
		if(MOD_BloodSplats()>=4)
		{
			CreateBloodsplat(ObjectCall( pProjectile ,"BulletOwner"));
			if(!Random(3)) CreateBloodsplat(ObjectCall( pProjectile ,"BulletOwner"));
		}
		if(MOD_BloodSplats()>=5)
		{
			CreateBloodsplat(ObjectCall( pProjectile ,"BulletOwner"));
			if(!Random(3)) CreateBloodsplat(ObjectCall( pProjectile ,"BulletOwner"));
		}
	}
*/
	// MoreGore aktiviert?
	if(!MOD_NoBlood())
	{
     		CastGore(ID_Gore_BloodStream,MOD_MoreGore()*2,40+Abs( iDmg )-Random(Abs( iDmg )));
     		CastGore(ID_Gore_Chunk,MOD_MoreGore()/3,60+Abs( iDmg )-Random(Abs( iDmg )));

		if( headshot )
		{
			if(MOD_MoreGore()>18) FlingGore(ID_Gore_Chunk,-10+Random(21)+GetXDir()/5,GetYDir()/5-10-Random(25));
			if(MOD_MoreGore()>16) FlingGore(ID_Gore_Chunk,-10+Random(21)+GetXDir()/5,GetYDir()/5-10-Random(25));
			if(MOD_MoreGore()>12) FlingGore(ID_Gore_Chunk,-10+Random(21)+GetXDir()/5,GetYDir()/5-10-Random(25));
			if(MOD_MoreGore()>8) FlingGore(ID_Gore_Chunk,-10+Random(21)+GetXDir()/5,GetYDir()/5-10-Random(25));
			if(MOD_MoreGore()>6) FlingGore(ID_Gore_Chunk,-10+Random(21)+GetXDir()/5,GetYDir()/5-10-Random(25));
			if(MOD_MoreGore()>4) FlingGore(ID_Gore_Chunk,-10+Random(21)+GetXDir()/5,GetYDir()/5-10-Random(25));
			if(MOD_MoreGore()>2) FlingGore(ID_Gore_Chunk,-10+Random(21)+GetXDir()/5,GetYDir()/5-10-Random(25));
			if(MOD_MoreGore()>0) FlingGore(ID_Gore_Chunk,-10+Random(21)+GetXDir()/5,GetYDir()/5-10-Random(25));
		}
	}

  /* Todesanimation bestimmen */
	var iPhase = 0;

	if(!GetDir())
	{
		if(GetX( pProjectile )>GetX())
			iPhase = 1;
		else if(GetX( pProjectile )<GetX())
			iPhase = 2;
	}
	else
	{
		if(GetX( pProjectile )<GetX())
			iPhase = 1;
		else if(GetX( pProjectile )>GetX())
			iPhase = 2;
	}


	var cl_body, cl_head, cl_legs;

	cl_head = CreateObject(CLBD,0,0,-1);
	cl_legs = CreateObject(CLBD,0,0,-1);
	cl_body = CreateObject(CLBD,0,0,-1);

	SetPosition( GetX(), GetY(), cl_legs );
	SetPosition( GetX(), GetY(), cl_body );
	SetPosition( GetX(), GetY(), cl_head );

	cl_legs ->~ CreateCorps( this, 0, iPhase );
	cl_body ->~ CreateCorps( cl_legs, "Body", iPhase );
	cl_head ->~ CreateCorps( cl_body, "Head", iPhase );
	cl_legs ->~ SetMaster();
	cl_body -> SetDir(GetDir());
	cl_head -> SetDir(GetDir());
	cl_legs -> SetDir(GetDir());

	cl_legs->SetSpeed(GetXDir(),GetYDir());
	SetSpeed();
	SetVisibility(VIS_None);

	// Flammen-Effekte auf die Leiche übertragen
	var flame, flames = FindObjects( Find_ID(ID_Gore_Flame),Find_Action("Hover"),Find_ActionTarget(this));
	for( flame in flames)
	{
		flame->~SetMaster(cl_legs);
	}

	if( fBlastWeapon )
	{
		cl_legs->SetSpeed(GetXDir( pProjectile )/divisor-5+Random(11),GetYDir( pProjectile )/divisor-5+Random(11));
		var amount = GetXDir(cl_legs) + GetYDir(cl_legs);
		cl_legs->SetRDir(amount);
		cl_head->SetRDir(amount/2);
		cl_body->SetRDir(amount/2);
	}
	else
	{
		cl_legs->SetRDir((GetXDir( pProjectile )+GetYDir( pProjectile ))/(30*divisor));
		cl_head->SetRDir((GetXDir( pProjectile )+GetYDir( pProjectile ))/(50*divisor));
		cl_body->SetRDir((GetXDir( pProjectile )+GetYDir( pProjectile ))/(50*divisor));
	}

	/* Teile anpassen */
	var deathcam_obj = cl_legs;

	if( headshot)
	{

		SetPosition( GetX(), GetY()-5, cl_head );
		SetSpeed(GetXDir()+GetXDir( pProjectile )/(3*divisor),-Random(10)+GetYDir()+GetYDir( pProjectile )/(3*divisor),cl_head);
		SetRDir((GetXDir( pProjectile )+GetYDir( pProjectile ))/(10*divisor),cl_head);
		cl_head->~SetMaster();
	}
	if(bodyshot) //if(!Random(3))
	{
		//cl_head->~SetMaster();
		cl_body->~SetMaster();
		deathcam_obj = cl_body;
		SetSpeed(GetXDir()+GetXDir( pProjectile )/(3*divisor),-Random(10)+GetYDir()+GetYDir( pProjectile )/(3*divisor),cl_head);
		SetSpeed(GetXDir()+GetXDir( pProjectile )/(3*divisor),-Random(10)+GetYDir()+GetYDir( pProjectile )/(3*divisor),cl_body);
		if(!MOD_NoBlood())
		{
			FlingGore(ID_Gore_Chunk,GetXDir()+GetXDir( pProjectile )/(3*divisor),-Random(10)+GetYDir()+GetYDir( pProjectile )/(3*divisor),-3+Random(7),-3+Random(7));
			FlingGore(ID_Gore_Chunk,GetXDir()+GetXDir( pProjectile )/(3*divisor),-Random(10)+GetYDir()+GetYDir( pProjectile )/(3*divisor),-3+Random(7),-3+Random(7));
			FlingGore(ID_Gore_Chunk,GetXDir()+GetXDir( pProjectile )/(3*divisor),-Random(10)+GetYDir()+GetYDir( pProjectile )/(3*divisor),-3+Random(7),-3+Random(7));
		}
		SetRDir((GetXDir( pProjectile )+GetYDir( pProjectile ))/(10*divisor),cl_body);
		SetRDir((GetXDir( pProjectile )+GetYDir( pProjectile ))/(10*divisor),cl_head);
	}
	if(feetshot) //if(!Random(3))
	{
		cl_body->~SetMaster();
		deathcam_obj = cl_body;
		SetSpeed(GetXDir()+GetXDir( pProjectile )/(3*divisor),-Random(10)+GetYDir()+GetYDir( pProjectile )/(3*divisor),cl_legs);
		SetRDir((GetXDir( pProjectile )+GetYDir( pProjectile ))/(10*divisor),cl_legs);

		if(!MOD_NoBlood())
		{
			FlingGore(ID_Gore_Chunk,GetXDir()+GetXDir( pProjectile )/(3*divisor),-Random(10)+GetYDir()+GetYDir( pProjectile )/(3*divisor),-3+Random(7),-3+Random(7));
			FlingGore(ID_Gore_Chunk,GetXDir()+GetXDir( pProjectile )/(3*divisor),-Random(10)+GetYDir()+GetYDir( pProjectile )/(3*divisor),-3+Random(7),-3+Random(7));
			FlingGore(ID_Gore_Chunk,GetXDir()+GetXDir( pProjectile )/(3*divisor),-Random(10)+GetYDir()+GetYDir( pProjectile )/(3*divisor),-3+Random(7),-3+Random(7));
		}
	}

	if(bodyshot) if(!Random(3)) DeathSound(Format("%s_medic",CrewGetVoice(this) ));
	if(feetshot) if(!Random(3)) DeathSound(Format("%s_cant_feel_my_legs",CrewGetVoice(this) ));
	if(!headshot) DeathSound();

	// "Death-Cam" an
	SetPlrView(GetOwner(),cl_body);
	if( GetPlayerType(GetOwner())==C4PT_User) AddEffect("DeathCam",deathcam_obj,200,1,0,UBOT,GetOwner());

	//DeathComment( iDmg , pCounter , idWeapon, pProjectile, fBlastWeapon, fHeadshotWeapon, headshot );*/
}

