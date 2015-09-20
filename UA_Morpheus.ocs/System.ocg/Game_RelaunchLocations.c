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
	   { x =  640, y = 820, team = -1},
	   { x = 1040, y = 885, team = -1},
	   { x =  745, y = 625, team = -1},
	   { x =  855, y = 625, team = -1},
	   { x =  745, y = 465, team = -1},
	   // left team
	   { x = 280, y = 755, team = 0},
	   { x = 400, y = 755, team = 0},
	   { x = 375, y = 675, team = 0},
	   { x = 350, y = 595, team = 0},
	   { x = 235, y = 515, team = 0},
	   { x = 350, y = 515, team = 0},
	   // right team
	   { x = 1410, y = 470, team = 1},
	   { x = 1205, y = 470, team = 1},
	   { x = 1315, y = 600, team = 1},
	   { x = 1205, y = 600, team = 1},
	   { x = 1480, y = 580, team = 1},
	   { x = 1205, y = 580, team = 1},
	   { x = 1205, y = 470, team = 1}
	   ];
}
