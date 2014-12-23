/* Schadenssystem */

#strict 2

//Schadens-Konstanten
static const DMG_Melee			= 1;
static const DMG_Projectile 	= 2;
static const DMG_Fire			= 4;
static const DMG_Explosion		= 8;
static const DMG_Energy			= 16;
static const DMG_Bio			= 32;
static const DMG_Headshot 		= 64;

// Schaden wird durch Mods angepasst! Sollte hier passieren, damit es nicht in jeder Waffe stehen muss
global func DoDmg(int iDmg, int iType, object pTarget, int iPrecision, int dmgplayer)
{
	var mult = 1, div = 1;

	// Mods
	if(MOD_HalfDamage()) div*=2;
	//if(MOD_HardcoreDamage()) { mult*=2; div*=3; }

	// vom Schadensverursacher modifizieren?
	if( this ) this->~DoDmgMod( mult, div );

	// anpassen
	iDmg = mult*iDmg/div;
	return _inherited( iDmg, iType, pTarget, iPrecision, dmgplayer);
}

/*
global func DoDmg(int iDmg, int iType, object pTarget, int iPrecision, int dmgplayer)
{
	if(!pTarget)
		if(!(pTarget = this))
			return;
	if(!iPrecision)
		iPrecision = 10;

	var dmg;
	var dmgdealer = dmgplayer-1;
	if(dmgdealer < 0)
		dmgdealer = GetController(pTarget);

	var red = pTarget->~OnDmg(iDmg, iType); //reduction
	// reduction
	dmg = iDmg*(100-red)*iPrecision;

	// Killer setzen
	if(this() && pTarget->GetOCF() & OCF_CrewMember || dmgplayer)
	{
		DebugLog("DoDmg sets killer: %d, owner of %s", "damage", this()->GetOwner(), this()->GetName());
		SetKiller(dmgdealer, pTarget);
	}

	//Schaden machen
	var pFrom;
	//if(pTarget != this())
		pFrom = this();
	pTarget->~LastDamageType(iType);
	pTarget->~OnHit(dmg/1000, iType, pFrom);

	// testweise: Schadensdings
	//var rgb = RGB(255,0,0);
	//if(dmg/1000 != 0){
	//	if(dmg < 0) rgb = RGB(0,255,0);
	//	CreateObject(CMBT,AbsX(GetX(pTarget)),AbsY(GetY(pTarget)),-1) -> Set(0, Format("%d",dmg/1000), rgb);
	//}

	if(GetCategory(pTarget) & C4D_Living)
		return(DoEnergy(-dmg,pTarget, true));
	return(DoDamage(dmg/1000,pTarget));
}
*/
