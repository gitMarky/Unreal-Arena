/*-- Hazard-Regeln nicht wählbar machen --*/

#strict 2

#appendto NODR // Arena verbieten - das hier ist immer Arena ;)
#appendto NOCA // Camping ist explizit erlaubt - ich werde mal Camping-Areas einführen, wo man campen darf
#appendto WPCH // Keine Waffenwahl

// Kann mittels des Spielzielauswählers ausgewählt werden
public func IsChooseable() { return false; }
