#strict 2

local team, color, tgt1,tgt2,tgt3;

public func ActivateEntrance( object pObj )
{
	//if(( pObj->~GetTeam()) != team ) return SetEntrance();
	SetEntrance(1);

	//var flag = FindObject(GetID(), 0,0,0,0,0, 0,pObj);
	//if(flag) flag->~AttachTargetLost();

	var tele = false;
	if( tgt1 && !tgt2 && !tgt3) {ScheduleCall(this,"Teleport",2,0,pObj,tgt1); tele = true;}//Teleport(pObj,tgt1);
	if(!tgt1 &&  tgt2 && !tgt3) {ScheduleCall(this,"Teleport",2,0,pObj,tgt2); tele = true;}//Teleport(pObj,tgt2);
	if(!tgt1 && !tgt2 &&  tgt3) {ScheduleCall(this,"Teleport",2,0,pObj,tgt3); tele = true;}//Teleport(pObj,tgt3);

	if(!tele)
		ScheduleCall(this,"Teleport",15,0,pObj,this);

	Schedule("SetEntrance()",2);
	return 1;
}

public func Serialize(array& extra)
{
	extra[GetLength(extra)] = Format("SetTo(%d,%d,%%d,%%d,%%d)", team, color,tgt1,tgt2,tgt3);
}

public func ContainedUp( object obj )
{
	Teleport( obj, tgt3);
	return 1;
}

public func ContainedRight( object obj )
{
	Teleport( obj, tgt2);
	return(1);
}


public func ContainedLeft( object obj )
{
	Teleport( obj, tgt1);
}

public func Teleport( object obj, object target)
{
	if(!obj) return;
	//if(( obj->~GetTeam()) != team ) return;
	if(target == this)
	{
		if(!Contained(obj)) return;
		if(Contained(obj)!=this) return;
	}
	Exit(obj);
	if(!target) return;
	SetPosition(GetX(target),GetY(target),obj);
	SetComDir(COMD_Stop,obj);
}

public func SetTo( int t, int c, object t1, object t2, object t3, bool bCancel)
{
	team = t;
	color = c;

	tgt1 = t1;
	tgt2 = t2;
	tgt3 = t3;

	AddEffect("Glow",this(),110,5,this(),0,color);

	if(bCancel) return;

	if(tgt1) tgt1 ->~ SetTo( t,c, this,0,0,true);
	if(tgt2) tgt2 ->~ SetTo( t,c, this,0,0,true);
	if(tgt3) tgt3 ->~ SetTo( t,c, this,0,0,true);
}


public func FxGlowStart(object pTarget, int iEffectNumber,int  iTemp, int dwcolor )
{
	EffectVar(0, pTarget, iEffectNumber) = dwcolor;
}

public func FxGlowTimer (object pTarget, int iEffectNumber, int iEffectTime)
{
	var r,g,b;

	r = GetRGBaValue(EffectVar(0, pTarget, iEffectNumber), 1);
	g = GetRGBaValue(EffectVar(0, pTarget, iEffectNumber), 2);
	b = GetRGBaValue(EffectVar(0, pTarget, iEffectNumber), 3);

	CreateParticle("ColSparkNGSlow", -8 + Random(17), -7 + Random(15), 0, -2 -Random(5), 30+Random(10), RGBa(r,g,b,20));
	CreateParticle("ColSparkNGSlow", -5 + Random(11), -5 + Random(11), 0, -2 -Random(5), 30+Random(10), RGBa(r,g,b,20));
	CreateParticle("ColSparkNGSlow", -5 + Random(11), -5 + Random(11), 0, -2 -Random(5), 30+Random(10), RGBa(r,g,b,20));
}
/*
public func TeleportToTarget( pObj, pTarget )
{
	// Schaut welcher Punkt näher am Ziel ist und teleportiert an diesen Ausgang

	var obj,tele;
	while( obj=FindObject(DE6J,GetX(pTarget)-GetX(),GetY(pTarget)-GetY(),-1,-1,0,0,0,0,obj))
	{
		if( obj==tgt1 || obj==tgt2 || obj==tgt3  )
		{
			tele=obj;
			break;
		}
	}

	if(!tele)
	{
		Exit(pObj);
	}
	else
	{
		Exit(pObj);
		SetPosition(GetX(tele),GetY(tele),pObj);

		SetComDir(COMD_Stop(),pObj);	
	}
}
*/
