/*-- Hazard-Regeln nicht w�hlbar machen --*/

#strict 2

#appendto NODR // Arena verbieten - das hier ist immer Arena ;)
#appendto NOCA // Camping ist explizit erlaubt - ich werde mal Camping-Areas einf�hren, wo man campen darf
#appendto WPCH // Keine Waffenwahl

// Kann mittels des Spielzielausw�hlers ausgew�hlt werden
public func IsChooseable() { return false; }
