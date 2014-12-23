/*-- Objekte, die bei Rundenstart entfernt werden sollen --*/

#strict 2

// Hazard Objekte

#appendto FLGH
#appendto CLGH
#appendto BLGH
#appendto HSGN
#appendto FENC
#appendto ENGT
#appendto AWRP // Warps
#appendto LADR // Leitern
#appendto LFTP // Liftplatten
#appendto SPNP // Spawnpunkte
#appendto WAYP // Wegpunkte
#appendto FLA2 // Flaggen
#appendto GEAR // alle Extras

// Unreal Objekte

#appendto CLBD // Leichen
#appendto HCRH // Fadenkreuze
#appendto _HUD // Huds

// Effekte
#appendto _EB2
#appendto _EB0
#appendto _EB1
#appendto _ES1
#appendto _ES0

// Projektile
#appendto SHT_
#appendto SHT1
#appendto SHT2

public func RemoveOnRestart(){ return true;}
