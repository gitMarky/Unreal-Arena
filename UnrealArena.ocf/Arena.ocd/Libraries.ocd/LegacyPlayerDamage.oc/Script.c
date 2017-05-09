local bDeathHeadshot, bNoCorpse, LastDmgWeapon;

// TODO - start 
static const ID_Gore_Chunk = nil; 

func MOD_MoreGore()
{
	return 0;
}

func MOD_NoBlood()
{
	return false;
}

func MOD_FastBullets()
{
	return false;
}

func CrewGetBlood()
{
	return 0;
}

func CrewGetVoice()
{
}

func DeathThrowWeapon()
{
}

static const CLBD = nil;

static const ID_Gore_BloodStream = nil;

static const WC6I = nil;

// TODO - end

// Waffe des Killers speichern!
func LastDamageWeapon(id idWeap, bool bOverride)
{
	if (idWeap || bOverride)
		LastDmgWeapon = idWeap;
	
	return LastDmgWeapon;
}


global func BloodFXColor(string szType)
{
	if (szType == "Machine")
		return [RGBa(90, 0, 200, 40), RGBa(90, 0, 200, 160)];
	if (szType == "Alien")
		return [RGBa(0, 220, 0, 40), RGBa(0, 220, 0, 160)];
	return [RGBa(255, 0, 0, 40), RGBa(200, 0, 0, 160)];
}

func Hurt()
{
	Sound(Format("%s_hit*", CrewGetVoice(this)));
}


func FxDeathCamStart(object pTarget, proplist iEffectNumber, int iTemp, int iOwner)
{
	iEffectNumber.var0 = iOwner;
}

func FxDeathCamTimer(object pTarget, proplist iEffectNumber, int iEffectTime)
{
	if (iEffectTime > 90)
	{
		DebugLog("DeathCamTimout");
		return -1;
	}
	if (!pTarget)
	{
		DebugLog("DeathCamTarget");
		return -1;
	}
	var g_iPlrViewRange = 700;
	var i = (g_iPlrViewRange * Cos(iEffectTime, 100)) / 100;
	SetPlrViewRange(i, pTarget);
	SetPlrView(iEffectNumber.var0, pTarget);
}

func FxDeathCamStop(object pTarget, proplist iEffectNumber, int iReason)
{
	SetPlrView(iEffectNumber.var0, GetHiRank(iEffectNumber.var0));
}

