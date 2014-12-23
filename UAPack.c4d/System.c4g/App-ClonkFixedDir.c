/*
 * Clonks haben eine feste Richtung, in die sie schauen. 
 */

#strict 2

static const g_FX_FixedDir_Direction = 0;
static const g_FX_FixedDir_Timer = 1;


global func GetDir(object obj, bool realDir)
{
	if(!obj && this) obj = this;
	
	var effect = GetEffect("UTFixedDir", obj);
	if (!effect
	 || realDir
	 || OverrideFixedDir(obj))
		return _inherited(obj);

	if (EffectVar(g_FX_FixedDir_Timer, obj, effect) <= 0)
		return _inherited(obj);

	return EffectVar(g_FX_FixedDir_Direction, obj, effect);
}

global func SetDir(int dir, object obj)
{
	if(!obj && this) obj = this;

	if (obj == this)
	{
		this->~SetFixedDir(dir);
	}
	_inherited(dir, obj);
}

global func OverrideFixedDir(object obj)
{
	return !(obj->~IsAiming());
}

global func FxUTFixedDirStart(object target,int nr, int temp)
{
		EffectVar(g_FX_FixedDir_Direction,target,nr) = GetDir(target, true);
}

global func FxUTFixedDirTimer(object target,int nr, int time)
{
	var current = GetDir(target, true);
	var dir = EffectVar(g_FX_FixedDir_Direction,target,nr);
	
	if (EffectVar(g_FX_FixedDir_Timer, target, nr) > 0)
		EffectVar(g_FX_FixedDir_Timer, target, nr)--;

	if (current == dir || OverrideFixedDir(target) || !EffectVar(g_FX_FixedDir_Timer, target, nr))
		target->SetObjDrawTransform(1000, 0, 0, 0, 1000);
	else
		target->SetObjDrawTransform(-1000, 0, 0, 0, 1000);
}
