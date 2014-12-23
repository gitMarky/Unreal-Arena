/* Ammosystem */

#strict 2

global func DoAmmo(id ammoid, int change, object target)
{ 
	if(!target) target = this;
	if( target->~IsWeapon() && target == this && target->~GetUser() && target->~FeedUserAmmo() )
		target = this->~GetUser();
	//else
	//	return _inherited( ammoid, change, target);


	// gar keine Munition
	if(!(ammoid->~IsAmmo())) return 0;

	// Kann 0 sein bei Objektlokalen Aufrufen.
	if(!target) target=this;

	// Entsprechendes Munitionslagerobjekt suchen
	var obj = target ->~ AmmoStoring();
	if(!obj) obj = target;
	// no ammo rule
	if(ObjectCount(NOAM))
		if(obj ->~ IsAmmoStorage())
			return 0;
	var ammo=FindContents(ammoid, obj);
	// Gibts schon ein Objekt der Munitionsart? OMGOMG, nein!
	if(!ammo)
		// Schnell eines erstellen
		ammo=CreateContents(ammoid, obj);
	//if(!ammo) Log("Panik - Munitionsobjekt wird nicht erstellt");
	// Alten Munitionsstand speichern
	var oldammoamount = obj->GetAmmo(ammoid);
	var truechange;
	var maxamount = ammoid->~MaxAmmo();
	if(!maxamount) maxamount = 999;
	// Der neue Wert wird ausgerechnet, darf aber nicht größer als eventuelles MaxAmount() und nicht kleiner als 0 sein
	//if(maxamount == 0)
	//	truechange= Max(oldammoamount+change, 0);
	//else
		truechange= Max(Min(maxamount, oldammoamount+change), 0);

		//Log("Changing Ammo %d to %d/%d",truechange-oldammoamount, truechange, maxamount );

	// Neuer Wert dem Objekt geben, in Lokale ammoamount
	ammo->Local(0)=truechange;
	// Wenn Muni alle, Objekt entfernen
	if(truechange==0) RemoveObject(ammo);
	// Differenz zurückgeben: Tatsächliche Änderung.
	return truechange-oldammoamount ;
}

global func SetAmmo(id ammoid, int change, object target)
{
	// gar keine Munition
	if(!(ammoid->~IsAmmo())) return 0;

	// Kann 0 sein bei Objektlokalen Aufrufen.
	if(!target) target=this;

	// Entsprechendes Munitionslagerobjekt suchen
	var obj = target ->~ AmmoStoring();
	if(!obj) obj = target;
	// no ammo rule
	if(ObjectCount(NOAM))
		if(obj ->~ IsAmmoStorage())
			return 0;
	var ammo=FindContents(ammoid, obj);
	// Gibts schon ein Objekt der Munitionsart? OMGOMG, nein!
	if(!ammo)
		// Schnell eines erstellen
		ammo=CreateContents(ammoid, obj);

	var truechange;
	var maxamount = ammoid->~MaxAmmo();
	if(!maxamount) maxamount = 999;

		truechange= Max(Min(maxamount, change), 0);

	// Neuer Wert dem Objekt geben, in Lokale ammoamount
	ammo->Local(0)=truechange;
	// Wenn Muni alle, Objekt entfernen
	if(truechange==0) RemoveObject(ammo);
	// Differenz zurückgeben: Tatsächliche Änderung.
	return truechange;
}
