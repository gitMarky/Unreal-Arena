#strict 2

global func CreateSmokeTrail(int iStrength, int iAngle, int iX, int iY, object pObj, bool bNew) {
//  Log("I...");
    iX += GetX(pObj);
    iY += GetY(pObj);
	if(!bNew)
		AddEffect("SmokeTrail", 0, 300, 1, 0, 0, iStrength, iAngle, iX, iY);
	else
		AddEffect("SmokeTrail2", 0, 300, 1, 0, 0, iStrength, iAngle, iX, iY);
}

// Variablen:
// 0 - Stärke
// 1 - momentane Stärke
// 2 - X-Position
// 3 - Y-Position
// 4 - Anfangs-X-Geschwindigkeit
// 5 - Anfangs-Y-Geschwindigkeit
global func FxSmokeTrail2Start(object pTarget, int iEffectNumber, int iTemp, iStrength, iAngle, iX, iY) {

  if(iTemp)
    return;

  if(iAngle%90 == 1) iAngle += 1;
  iStrength = Max(iStrength,5);

  EffectVar(0, pTarget, iEffectNumber) = iStrength;
  EffectVar(1, pTarget, iEffectNumber) = iStrength;
  EffectVar(2, pTarget, iEffectNumber) = iX;
  EffectVar(3, pTarget, iEffectNumber) = iY;
  EffectVar(4, pTarget, iEffectNumber) = +Sin(iAngle,iStrength*30);
  EffectVar(5, pTarget, iEffectNumber) = -Cos(iAngle,iStrength*30);

}

global func FxSmokeTrail2Timer(object pTarget, int iEffectNumber, int iEffectTime) {
  var iStrength = EffectVar(0, pTarget, iEffectNumber);
  var iAStr = EffectVar(1, pTarget, iEffectNumber);
  var iX = EffectVar(2, pTarget, iEffectNumber);
  var iY = EffectVar(3, pTarget, iEffectNumber);
  var iXDir = EffectVar(4, pTarget, iEffectNumber);
  var iYDir = EffectVar(5, pTarget, iEffectNumber);

  iAStr = Max(1,iAStr-iAStr/10+Random(2));
  iAStr--;
  iYDir += GetGravity()/4;

  var xdir = iXDir*iAStr/iStrength;
  var ydir = iYDir*iAStr/iStrength;

  // Neu: Random
  //iX += RandomX(-3,3);
  //iY += RandomX(-3,3);

  var rgb1 = EffectVar(6, pTarget, iEffectNumber)=150-Random(30);
  var rgb2 = EffectVar(7, pTarget, iEffectNumber)=50-Random(30);

  var rgb = (rgb1*iAStr + rgb2*(iStrength-iAStr))/iStrength;
  var alpha = (60*iAStr + 160*(iStrength-iAStr))/iStrength;

  // zuerst zeichnen
  CreateParticle("Smoke3",iX,iY,RandomX(-2,2),RandomX(-4,2),20+iAStr*4,RGBa(75,65,55,55+35*iAStr/iStrength));
  //CreateParticle("Smoke3",iX,iY,0,0,20+iAStr*8,RGBa(rgb,rgb,rgb,alpha));
  if(!Random(2)) CreateParticle("Blast",iX,iY,0,0,10+iAStr*8,RGBa(250,100+Random(100),100,220));

  // dann nächste position berechnen
  iX += xdir/100;
  iY += ydir/100;
  
  if(GBackSemiSolid(iX,iY))
    return(-1);
  if(iAStr <= 1)
    return(-1);

  EffectVar(1, pTarget, iEffectNumber) = iAStr;
  EffectVar(2, pTarget, iEffectNumber) = iX;
  EffectVar(3, pTarget, iEffectNumber) = iY;
  EffectVar(5, pTarget, iEffectNumber) = iYDir;
}
