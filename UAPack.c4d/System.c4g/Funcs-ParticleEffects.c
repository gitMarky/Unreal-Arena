/* Partikeleffekte */

#strict 2

/* Disintegration */

global func Disintegrate(object target, int iLen, int iMult, int dy)
{
	if(!target) target = this;
	if(!target) return;
	if(Contained(target)) return;
	if(GetEffect("ArenaDisint",target)) return;
	
	if(!GetRDir(target)) SetRDir(RandomX(-7,7),target);

	AddEffect("ArenaDisint", target, 1, 1, 0, 0, iLen, iMult,dy);

}

global func CancelDisintegrate()
{

}

global func IsDisintegrating(object target)
{
	if(GetEffect("ArenaDisint",target)) return true;
	return false;
}

global func FxArenaDisintStart (object pTarget, int iEffectNumber, int iTemp, int iLen,int iMult, int dy)
{
	if(!iLen) iLen = 50;
	if(!iMult) iMult = 255;
	EffectVar(0,pTarget,iEffectNumber) = GetColorDw(pTarget);
	EffectVar(1,pTarget,iEffectNumber) = iLen;
	EffectVar(2,pTarget,iEffectNumber) = iMult;
	EffectVar(3,pTarget,iEffectNumber) = dy;
	EffectVar(4,pTarget,iEffectNumber) = GetY(pTarget);
}
/*
global func FxArenaDisintTimer(object target, int number, int time)
{
	var len = EffectVar(1,target,number);
	var dt = EffectVar(1,target,number);
	if(time <= len)
	{
		var r,g,b, dw;

		dw = EffectVar(0,target,number);
		r = GetRGBaValue(dw,1);
		g = GetRGBaValue(dw,2);
		b = GetRGBaValue(dw,3);

		r = BoundBy( ((len-time)*r + time*255)/len,0,255);
		g = BoundBy( ((len-time)*g + time*255)/len,0,255);
		b = BoundBy( ((len-time)*b + time*255)/len,0,255);

		dw = RGB(r,g,b);

		r = BoundBy(255-255*time/len,0,255);
		target->SetClrModulation(RGBa(r,255,r,BoundBy(60*time/len,0,255)));
		target->SetColorDw(dw);
	}
	else
	{
		var diff = time - len;
		diff = 255*diff/dt;
		SetObjectBlitMode(1,target);
		SetClrModulation(RGBa(0,255,0, diff ),target);

		for(var i=1; i<20; i++)
		{
			if(GetUnusedOverlayID(i,target)==i) continue;
			SetClrModulation(RGBa(0,255,0, diff ),target,i);
			SetObjectBlitMode(1,target,i);
		}

		target->SetColorDw(RGB(255,255,255));
		if(diff >= 255) RemoveObject(target);
	}
		if(!Random(10))
		{
			var r = target->GetR();
			var x1, x2, y1,y2;
			var tID = GetID(target);
			x1 = GetDefOffset(tID,0);
			x2 = GetDefOffset(tID,0) + GetDefWidth(tID);
			y1 = GetDefOffset(tID,1);
			y2 = GetDefOffset(tID,1) + GetDefHeight(tID);

			var rx1,rx2,ry;

			rx1 = Cos(r,x1) - Sin(r,y2);
			rx2 = Cos(r,x2) - Sin(r,y1);

			ry = Cos(r,GetDefHeight(tID)/2 ) + Sin(r,GetDefWidth(tID)/2 );

			target->CreateParticle("NoGravSpark", RandomX(rx1,rx2), RandomX(ry/2,ry), 0, -5, 25, RGBa(0, 210, 0, diff ));
		}

}
*/
global func FxArenaDisintTimer(object target, int number, int time)
{
	// aufgesammelte Objekte brechen ab
	// allerdings kann der Clonk keine disintegrierenden Objekte aufsammeln, also passt alles
	if(Contained(target))
	{
		return -1;
	}
	var len = EffectVar(1,target,number);
	var dt = EffectVar(2,target,number);
	var dy = time*EffectVar(3,target,number)/(len+dt);
	var clr_mod, clr_dw;
	if(time <= len)
	{
		var r,g,b, dw;

		dw = EffectVar(0,target,number);
		r = GetRGBaValue(dw,1);
		g = GetRGBaValue(dw,2);
		b = GetRGBaValue(dw,3);

		r = BoundBy( ((len-time)*r + time*255)/len,0,255);
		g = BoundBy( ((len-time)*g + time*255)/len,0,255);
		b = BoundBy( ((len-time)*b + time*255)/len,0,255);

		dw = RGB(r,g,b);

		r = BoundBy(255-255*time/len,0,255);
		clr_mod = RGBa(r,255,r,BoundBy(60*time/len,0,255));
		clr_dw = dw;
	}
	else
	{
		var diff = time - len;
		diff = 255*diff/dt;
		SetObjectBlitMode(1,target);
		clr_mod = RGBa(0,255,0, diff );
		clr_dw = RGB(255,255,255);

		if(diff >= 255)
		{
			RemoveObject(target);
			return -1;
		}
	}

	for(var i=1; i<20; i++)
	{
		if(GetUnusedOverlayID(i,target)==i) continue;
		SetClrModulation(clr_mod,target,i);
		SetObjectBlitMode(1,target,i);
	}

	SetClrModulation(clr_mod,target);
	SetColorDw(clr_dw,target);

	var tID = GetID(target);
	var ran = BoundBy( 20 - 2*Distance(GetDefWidth(tID),GetDefHeight(tID))/3, 2, 20 );

		if(!Random(ran))
		{
			var r = target->GetR();
			var x1, x2, y1,y2;
			x1 = GetDefOffset(tID,0);
			x2 = GetDefOffset(tID,0) + GetDefWidth(tID);
			y1 = GetDefOffset(tID,1);
			y2 = GetDefOffset(tID,1) + GetDefHeight(tID);

			var rx1,rx2,ry;

			rx1 = Cos(r,x1) - Sin(r,y2);
			rx2 = Cos(r,x2) - Sin(r,y1);

			ry = Cos(r,GetDefHeight(tID)/2 ) + Sin(r,GetDefWidth(tID)/2 );

			target->CreateParticle("NoGravSpark", RandomX(rx1,rx2), RandomX(ry/2,ry), 0, EffectVar(3,target,number)/2, 25, RGBa(0, 210, 0, diff ));
		}

	SetPosition(GetX(target),EffectVar(4,target,number)+dy,target);
	SetYDir(0,target);

}
