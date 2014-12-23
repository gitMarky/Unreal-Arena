#strict 2
#appendto STAP

public func MayTransfer(object pObj)
{
	if(!pObj) return(false);
	var MaxAmmo = AmmoID()->~MaxAmmo();

	if(!MaxAmmo) MaxAmmo = 999;

	//if(MaxAmmo)
		if(GetAmmo(AmmoID(),pObj) + AmmoCount() > MaxAmmo)
			return(false);
	return(true);
}
