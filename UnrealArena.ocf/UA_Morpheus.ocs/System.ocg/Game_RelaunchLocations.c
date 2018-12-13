/**
 Overloads/implements callbacks from the scenario script object.
 @author Marky
 @version 0.1
 */

#appendto Environment_ScenarioScript

protected func RelaunchLocations()
{
	return [
	   // no team
	   { x =  640, y = 820, team = 0},
	   { x = 1040, y = 885, team = 0},
	   { x =  745, y = 625, team = 0},
	   { x =  855, y = 625, team = 0},
	   { x =  745, y = 465, team = 0},
	   // left team
	   { x = 280, y = 755, team = 1},
	   { x = 400, y = 755, team = 1},
	   { x = 375, y = 675, team = 1},
	   { x = 350, y = 595, team = 1},
	   { x = 235, y = 515, team = 1},
	   { x = 350, y = 515, team = 1},
	   // right team
	   { x = 1410, y = 470, team = 2},
	   { x = 1205, y = 470, team = 2},
	   { x = 1315, y = 660, team = 2},
	   { x = 1205, y = 660, team = 2},
	   { x = 1400, y = 580, team = 2},
	   { x = 1205, y = 580, team = 2}
	   ];
}
