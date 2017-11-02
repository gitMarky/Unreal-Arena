#appendto Arena_JumpPad

func SetEffectColor(int color)
{
	SetLightRange(100, 75);
	SetLightColor(color);
	return inherited(color, ...);
}
