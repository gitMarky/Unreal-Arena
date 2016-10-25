/* Schadenssystem */

// TODO - start

func MOD_HalfDamage()
{
}

// TODO - end


// Schaden wird durch Mods angepasst! Sollte hier passieren, damit es nicht in jeder Waffe stehen muss
global func DoDmg(int iDmg, int iType, object pTarget, int iPrecision, int dmgplayer)
{
	var mult = 1, div = 1;
	
	// Mods
	if (MOD_HalfDamage())
		div *= 2;
	//if(MOD_HardcoreDamage()) { mult*=2; div*=3; }
	
	// vom Schadensverursacher modifizieren?
	if (this)
		this->~DoDmgMod(mult, div);
	
	// anpassen
	iDmg = mult * iDmg / div;
	return _inherited(iDmg, iType, pTarget, iPrecision, dmgplayer);
}
