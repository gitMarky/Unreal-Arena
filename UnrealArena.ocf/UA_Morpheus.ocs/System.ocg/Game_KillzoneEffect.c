/**
 The effect kills the crew if they fall to deep.
 @author Marky
 @version 0.1
 */

global func FxGameKillzoneTimer(object target, proplist effect, int time)
{
	if (target->GetY() > 970)
	{
		target->Kill(target, true);
		return FX_Execute_Kill;
	}
}