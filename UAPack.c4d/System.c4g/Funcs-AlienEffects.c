/*-- Alienpartikeleffekte --*/

#strict 2

/* Alienpartikel 2 */

global func AddAlienFlareEffect(object target, int size, int iColor) {
	if(!target) target = this;
	if(!target) return;
	
	if(!iColor)
		iColor = RGBa(160,255,0,0);

	AddEffect("AlienFlareEffect", target, 1, 1, 0, 0,size,iColor);
	
}

global func FxAlienFlareEffectStart(object target, int effect, int temp, size, color) {
	EffectVar(0,target,effect) = size;
	EffectVar(1,target,effect) = color;
}

global func FxAlienFlareEffectTimer(object target, int effect, int time)
{
  var xdir = GetXDir(target);
  var ydir = GetYDir(target);
  var size = EffectVar(0,target,effect)+RandomX(-100,100);
 
  if(!Contained(target))
  	CreateParticle("FrSprk", GetX(target), GetY(target), xdir/3+RandomX(-2, 2), ydir/3+RandomX(-2, 2),
                 size, EffectVar(1,target,effect));
}


/* Alienpartikel 2 */

global func AddAlienStinkEffect(object target, int size, int iColor) {
	if(!target) target = this;
	if(!target) return;

	if(!iColor)
		iColor = RGBa(160,255,0,0);

	AddEffect("AlienStinkEffect", target, 1, 10, 0, 0,size, iColor);

}

global func FxAlienStinkEffectStart(object target, int effect, int temp, size, color) {
	EffectVar(0,target,effect) = size;
	EffectVar(1,target,effect) = color;
}

global func FxAlienStinkEffectTimer(object target, int effect, int time)
{
  var xdir = GetXDir(target);
  var ydir = GetYDir(target);
  var size = EffectVar(0,target,effect)+RandomX(-100,100);

  if(!Contained(target))
  	CreateParticle("FrSprk", GetX(target), GetY(target), xdir/3+RandomX(-2, 2), ydir/3+RandomX(-8, -2),
                 size, EffectVar(1,target,effect));
}

