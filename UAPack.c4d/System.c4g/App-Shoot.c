/*-- Schuss --*/

// Erweiterung des Hazard-Schusses
// wird eigentlich nicht mehr gebraucht, wenn ich mich richtig erinnere

#strict 2
#appendto SHT1

public func Launch(int iAngle, int iSpeed, int iDist, int iSize, int iTrail, int iDmg, int iDmgPrec, int iGlowSize, int iAPrec, int iReflections)
{
	if(MOD_FastBullets()) iSpeed*=2;

	_inherited( iAngle, iSpeed, iDist, iSize, iTrail, iDmg, iDmgPrec, iGlowSize, iAPrec, iReflections );
}

/*
public func LaunchInstant(int iAngle, int iSpeed, int iDist, int iSize, int iTrail, int iDmg, int iDmgPrec, int iGlowSize, int iAPrec, int iReflections)
{
	// Standardwerte setzen
	if(!iSize)		iSize = 8;
	//if(!iGlowSize)
	//	iGlowSize = iSize;
	if(!iTrail)	 iTrail = 300;
	if(!iDmg)		 iDamage = 3;
	else iDamage = iDmg;

	// und zuweisen
	iSize = Min(iSize+2,GetDefWidth());
	iPrec = iDmgPrec;

	// Positionieren
	SetPosition(GetX(),GetY()+GetDefWidth()/2);

	DoCon(100*iSize/GetDefWidth()-100);

	lx = GetX();
	ly = GetY();

	iTime = 10*iDist/iSpeed;

	if(!iTime)
		return(RemoveObject());

	var self = this;
	SetAction("Travel");
	if(!self) return;	 // Kleiner Sicherheitscheck, ob die Kugel nicht sofort verschwindet

	SetXDir(+Sin(iAngle,iSpeed, iAPrec));
	SetYDir(-Cos(iAngle,iSpeed, iAPrec));
	SetR(+iAngle);


	// Werte für Reflektionen speichern
	iRefl = iReflections;
	//if(iRefl)
	//{
		iTrailSize = iSize;
		iTrailLength = iTrail;
		iXDir = GetXDir(0, 100);
		iYDir = GetYDir(0, 100);
	//}

	HitCheck(iAngle,iDist);
}
*/
public func FxHitCheckTimer(object target, int effect, int time)
{
	var obj;
	var oldx = EffectVar(0, target, effect);
	var oldy = EffectVar(1, target, effect);
	var newx = GetX(target);
	var newy = GetY(target);
	EffectVar(0, target, effect) = GetX(target);
	EffectVar(1, target, effect) = GetY(target);

	//Schuss schon Scharf?
	var exclude = EffectVar(2, target, effect);
	//Nicht selber treffen
	if(EffectVar(4, target, effect)) exclude = target;

	//DrawParticleLine("NoGravSpark",newx-oldx, newy-oldy,0,0,1,25,RGB(255,0,0),RGB(0,0,255));
	//Wir suchen nach Objekten entlang der Linie die wir uns seit dem letzten Check
	//bewegt haben. Und sortieren sie nach Distanz (nähere zuerst)
	for(obj in FindObjects(Find_OnLine(oldx,oldy,newx,newy),
				Find_NoContainer(),
				Sort_Distance(oldx, oldy)))
	{
		//Excludes
		if(obj == target) continue;
		if(obj == exclude) continue;

		//CheckEnemy
		if(!CheckEnemy(obj,target)) continue;

		// IsBulletTarget oder Alive
		if(obj->~IsBulletTarget(GetID(target),target,EffectVar(2, target, effect),oldx,oldy) || GetOCF(obj) & OCF_Alive)
		{
			DebugLog("%s IsBulletTarget: %i, %s, %s","HitCheck",GetName(obj),GetID(target),GetName(target),GetName(EffectVar(2, target, effect)));
			return(target-> ~HitObject(obj));
		}
	}

	EffectVar(0, target, effect) = GetX(target);
	EffectVar(1, target, effect) = GetY(target);

	//Der Schuss wird erst "scharf gemacht", d.h. kann den Schützen selbst treffen, wenn
	//der Schuss einmal die Shape des Schützen verlassen hat.

	if(!EffectVar(5,target,effect))
	{
		if(!EffectVar(4, target, effect))
		{
			//Ready gibt an, ob wir schon "scharf" sind. True = Scharf
			var ready = true;
			//Wir suchen alle Objekte mit der ID unseres Schützens an unserer momentanen Stelle
			for(var foo in FindObjects(Find_AtPoint(GetX(target),GetY(target)),Find_ID(EffectVar(3, target, effect))))
			//Und schauen, ob es der Schütze ist.
			if(foo == EffectVar(2, target, effect))
				//Wir haben den Schützen gefunden -> Er ist noch an unserer Position
				ready = false;
			//Wir haben den Schützen nicht gefunden
			if(ready)
				//Wir treffen ihn ab jetzt
				EffectVar(4, target, effect) = true;
		}
	}
}


/*
private func HitCheck(int r, int d)
{
	// Endposition bestimmen
	var dx, dy, dist, ox, oy;
	dx = Sin(r,d);
	dy = -Cos(r,d);
	dist = 0;

	ox = GetX(); oy = GetY();
	// Landschaftskollision prüfen
	while(++dist)
	{
		if(GBackSolid(dx*dist/d,dy*dist/d))
			break;
		if(dist>=d)
			break;
	}

	// neue Endposition
	dx = +Sin(r,dist);
	dy = -Cos(r,dist);

	for(var i = 0; i < dist; i++)
	{
		var pObj, pTargets;

		SetPosition(ox,oy);

		pTargets = FindObjects(	Find_AtPoint( dx*i/dist,dy*i/dist),
								 Find_Exclude(this),
								 Find_Exclude(shooter),
								 Find_NoContainer(),
								 Find_Or(Find_Func("IsBulletTarget",GetID(),this,shooter), Find_OCF(OCF_Alive)),
								 Find_Func("CheckEnemy",this),
								 Find_Not(Find_Func("HitExclude")));

		for(pObj in pTargets)
		{
			lx = dx*i/dist;
			ly = dy*i/dist;
			SetPosition( ox + dx*i/dist, oy + dy*i/dist);

			if(HitObject(pObj))
			{
				 // Trail erzeugen
				 CreateTrailEx(iTrailSize, iTrailLength);

				//dst += i;
				return i;
			}
		}
	}

	lx = dx;
	ly = dy;

	SetPosition(ox+dx,oy+dy);

	CreateTrailEx(iTrailSize,iTrailLength);

	if(dist < d)//Nicht in der Luft. :O
		HitObject(); //HitLandscape(mx,my);

	//dst += ml;
	//return ml;
	RemoveObject();

	 // Trail erzeugen


}

// Extern für Überladung
private func CreateTrailEx(int iSize, int iTrail) {
	pTrail = CreateObject(TRAI,0,0,-1);
	if(pTrail)
	{
		pTrail->Set(iSize-2,iTrail,shooter);
		SetObjectBlitMode(GetObjectBlitMode(),pTrail);
	}
}

*/
private func HitObject(object pObject)
{
	var ret = BulletStrike(pObject);

	if(!ret)
	{
		//Sparks(70*GetCon()/100,Color(GetActTime()));
		Sound("ProjectileHit*");
		CastParticles("ColSpark",10,20,0,0,20,40,RGBa(255,100,0,80),RGBa(255,200,0,130));
	}

	Remove();

	return ret;
}

public func BulletStrike(object pObj)
{
	if(pObj)
	{
		DoDmg(iDamage,DMG_Projectile,pObj,iPrec);
		return true;
	}
}


private func Traveling()
{
	if(pTrail) HandleTrail();

	_inherited();
}

public func HandleTrail()
{
	 //SetPosition(GetX(), GetY(), pTrail);
	 if(pTrail) pTrail->~Update(this);

	 if(GetAction() == "TravelBallistic")
		  SetR(Angle(0,0,GetXDir(),GetYDir()));
}

public func SetBallistic()
{
	SetAction("TravelBallistic");
}
