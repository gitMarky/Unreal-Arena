#strict

local w, l, r, c, iTime, pDmgCallback, pAttach, pLight, iMaxDist, dx, dy, iPosi, fIgnoreLiving, pPointAt;
local shooter;
local speed;

protected func Initialize()
{
	SetObjectBlitMode(1);
	SetClrModulation(RGB(255,0,0)); // Standard ist Rot
}

/* Interface (public functions) */

public func SetUser( object pObj )
{
	shooter = pObj;
}

public func SetPosi(int iPos)
{
	iPosi = iPos;
}

public func SetAngle(int iAngle)
{
	r = iAngle-180;
}

public func SetWidth(int iWidth)
{
	w = 1000*iWidth/GetActMapVal("Facet","Laser",0,2);
}

public func SetVelocity(int iSpeed)
{
	speed = iSpeed;
}

public func SetMaxDistance(int iDist)
{
	iMaxDist = iDist;
}

public func SetContinuous(int iDelay)
{
	if(!iDelay) iDelay = 1;
	AddEffect("Continuous", this(), 1,iDelay, this());
}

public func Set(int iAngle, int iWidth, int iDist, int iT, pDmg, object pAtt, int iPos, colmod, fIgnore, object pTarget)
{
	if(!pDmg) Log("NoDamager!!!!");

	// optional parameters
	if(!iWidth) iWidth = 3;
	if(!iDist) iDist = 300;
	// iT = 0: no out-fading
	iTime = iT;
	fIgnoreLiving = fIgnore;
	pPointAt = pTarget;

	// reset fade-timer
	c=0;

	// new attach and damage callback objects (optional)

	pDmgCallback = pDmg;
	pAttach = pAtt;
	if(pAttach)
	{
		dx = GetX()-GetX(pAttach);
		dy = GetY()-GetY(pAttach);
	}

	// create light
	//pLight = CreateObject(LALI,0,0,-1);

	if(colmod) SetClrModulation( colmod );

	SetMaxDistance(iDist);
	SetAngle(iAngle);
	SetWidth(iWidth);
	SetPosi(iPos);
	HitCheck();
	DrawTransform();
}

public func GetAngle()	{ return(r+180); }
public func GetWidth()	{ return(w*GetActMapVal("Facet","Laser",0,2)/1000);	}
public func GetLength()
{ 
	if(GetAction() == "Idle")
		return(l*GetActMapVal("Facet","Laser",0,3)/1000);
	else
		return(l*GetActMapVal("Facet",GetAction(),0,3)/1000);
}

public func GetMaxDistance() { return(iMaxDist); }

public func LaserEnd(&x, &y)
{
	x = +Sin(GetAngle(),GetLength());
	y = -Cos(GetAngle(),GetLength());
}

/* intern */

private func HitCheck()
{

	var solid_hit;

	// calculate length
	var mx=-Sin(r,iMaxDist),
			my=+Cos(r,iMaxDist),
			ml=0;
	while(++ml)
	{
		var needs_break;

		if(GBackSolid(mx*ml/iMaxDist,my*ml/iMaxDist)) { solid_hit = 1; break;}

		if(ml>=iMaxDist) break;

		if( GetX()+mx*ml/iMaxDist < 0 ) needs_break = 1;
		if( GetX()+mx*ml/iMaxDist > LandscapeWidth() ) needs_break = 1;
		if( GetY()+my*ml/iMaxDist < 0 ) needs_break = 1;
		if( GetY()+my*ml/iMaxDist > LandscapeHeight() ) needs_break = 1;

		if( needs_break )
		{
			ml--;
			break;
		}
	}

	// graphic
	SetLaser(ml == iMaxDist);

	l = 1000*ml/GetActMapVal("Facet",GetAction(),0,3);
	LaserEnd(mx,my); 

	// no hit check
	if(!pDmgCallback)
	return();

	// search points on the line for clonks etc.
	// step for step
	if( !fIgnoreLiving )
	{
		if(!pPointAt)
		{

			/*for(var i = 0; i < ml; i=Min(ml,i+3))
			{
				var pObj;

				if( pObj = pDmgCallback->~FindEnemy( GetX()+NotZero(mx*i/ml), GetY()+my*i/ml, 1 ) )
				if(pDmgCallback ->~ LaserStrike(pObj))
				{
					// the laser doesn't get through objects, shorten it
					SetLaser(0);
					l = 1000*i/GetActMapVal("Facet",GetAction(),0,3);
					return();
				}
			}*/

			var pTargets;
			pTargets = FindObjects(	 Find_OnLine(0,0,mx,my),//Find_AtPoint( dx*i/dist,dy*i/dist),
									 Find_Exclude(this),
									 Find_Exclude(pDmgCallback),
									 //Find_Exclude(shooter),
									 Find_NoContainer(),
									 Find_Or(Find_Func("IsBulletTarget",GetID(),this,shooter), Find_OCF(OCF_Alive)),
									 Find_Func("CheckEnemy",this),
									 Find_Not(Find_Func("HitExclude")),
									 Sort_Distance(0,0));

			//Log("Found Targets %v",pTargets);

			var x = GetX(pDmgCallback),y = GetY(pDmgCallback);
			var xdir = GetXDir(pDmgCallback),ydir = GetYDir(pDmgCallback);
			pDmgCallback->SetSpeed(Cos(r,speed),-Sin(r,speed));

			for(pObj in pTargets)
			{
				var i = ObjectDistance(pObj);//Distance(GetX(),GetY(),GetX(pObj),GetY(pObj));
				//lx = dx*i/dist;
				//ly = dy*i/dist;
				pDmgCallback->SetPosition( GetX() + mx*i/ml, GetY() + my*i/ml);
				//pDmgCallback->SetPosition(GetX(pObj),GetY(pObj));
				var result = pDmgCallback ->~ LaserStrike(pObj);
				if(result)
				{
									// the laser doesn't get through objects, shorten it
									SetLaser(0);
									l = 1000*ObjectDistance(pObj)/GetActMapVal("Facet",GetAction(),0,3);
									return;
				}
			}
			pDmgCallback->SetPosition(x,y);
			pDmgCallback->SetSpeed(xdir,ydir);
		}
	}
	else
	{
		var i = ml;
	}

	if( pPointAt )
	{
		var i = ObjectDistance( pAttach, pPointAt );

		if( i > ml )
		{
			pDmgCallback ->~ LaserPointLost( pPointAt, this() );
		}
		else
		{
			// the laser doesn't get through objects, shorten it
			SetLaser(0);
			l = 1000*i/GetActMapVal("Facet",GetAction(),0,3);
		}

		return();
	}

	// Nix getroffen, aber auf Material aufgeschlagen:

	if( solid_hit )
	{
		pDmgCallback->~LaserMaterial( GetX()+NotZero(mx*i/ml), GetY()+my*i/ml );
	}
	else
	{
		pDmgCallback->~LaserVoid( GetX()+NotZero(mx*i/ml), GetY()+my*i/ml );
	}
}

private func NotZero(int a)
{
	if(!a) return(1);
	else return(a);
}

private func SetLaser(bool end)
{
	if(end)
	{
		if(GetAction() ne "Laser")
		{
			SetAction("Laser");
		}

	}
	else
	{
		if(GetAction() ne "Laser")
		{
			SetAction("Laser");
		}
	}
}

protected func Laser()
{		// fade out

	// attach to obj
	if(pAttach) SetPosition(GetX(pAttach)+dx,GetY(pAttach)+dy);

	// animation & effect
	SetPhase(Random(4));
	if(GetAction() eq "Laser")
	{
		var x,y;
		LaserEnd(x,y);
		var color = GetClrModulation();
		//LaserLight(20+GetWidth()*2,color,x,y);
		/*var a,r,g,b;
		SplitRGBaValue(color,r,g,b,a);
		a = BoundBy(a+64,180,255);
		CreateParticle("PSpark",x,y,0,0,300+GetWidth()*30,RGBa(r,g,b,a));*/
	}

	if(!iTime) return();

	// fade out (only if time != 0)
	var a = 200*c/iTime;
	var rgba = SetRGBaValue(GetClrModulation(),a,0);
	SetClrModulation(rgba);
	++c;

	if(c >= iTime) RemoveObject();
}

protected func Destruction()
{
	if(pLight) RemoveObject(pLight);
	if(pDmgCallback) pDmgCallback->~LaserRemoved();
}


private func DrawTransform()
{	// draw line

	var cl = BoundBy( l-iPosi, 0, l);
	cl = l*GetActMapVal("Facet","Laser",0,3)/GetActMapVal("Facet",GetAction(),0,3);

	var fsin = -Sin(r, 1000), fcos = Cos(r, 1000);

	var xoff = -(GetActMapVal("Facet",GetAction(),0,2)*w/1000)/2;
	var yoff = 0;

	var width = +fcos*w/1000, height = +fcos*cl/1000;
	var xskew = +fsin*cl/1000, yskew = -fsin*w/1000;

	var xadjust = +fcos*xoff + fsin*yoff +fsin*iPosi;
	var yadjust = -fsin*xoff + fcos*yoff +fcos*iPosi;

	// set matrix values
	SetObjDrawTransform ( width, xskew, xadjust, yskew, height, yadjust );
	
	// light
	if(pLight)
	{
		var alphamod, sizemod;
		CalcLight(alphamod, sizemod);

		var light_width = w*(5+sizemod/100);
		var light_height = cl/2;
		xoff = -(GetActMapVal("Facet","Laser",LALI,2)*light_width/1000)/2;
		xadjust = +fcos*xoff + fsin*yoff;
		yadjust = -fsin*xoff + fcos*yoff;
		width =	+fcos*light_width/1000;
		height = +fcos*light_height/1000;
		xskew =	+fsin*light_height/1000;
		yskew =	-fsin*light_width/1000;
		
		var a,r_,g,b;
		SplitRGBaValue(GetClrModulation(),r_,g,b,a);
		SetClrModulation(RGBa(r_,g,b,Min(a+160+alphamod,180)),pLight);
		SetPosition(GetX(),GetY(),pLight);
		
		SetObjDrawTransform ( width, xskew, xadjust, yskew, height, yadjust, pLight );
	}
}

private func FxContinuousTimer(object target, int effect, int time)
{
	// ein bischen Hax: wenn der Laser bleibt, wird auch der Winkel
	// angepasst, falls der Target AimAngle hat (Clonk hat das)
	var angle;
	if(pAttach)
	{
		if( pPointAt ) pAttach->~Aim2Angle( Angle( GetX( pAttach ), GetY( pAttach ), GetX( pPointAt ), GetY( pPointAt ) ) );

		if(angle = (pAttach->~AimAngle(1)))
			SetAngle(angle);

		if((GetDir(pAttach) == DIR_Right) && (dx < 0)) dx *= -1;
		if((GetDir(pAttach) == DIR_Left) && (dx > 1))	dx *= -1;
	}
	else
	{
		return(-1);
	}

	HitCheck();

	DrawTransform();
}
