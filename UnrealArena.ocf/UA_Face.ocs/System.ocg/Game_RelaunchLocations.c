#appendto Environment_ScenarioScript

func RelaunchLocations()
{
	return [{x = 125, y = 575, team = 1},
	        {x = 120, y = 475, team = 2},
	    	{x = LandscapeWidth() - 125, y = 575, team = 2},
	    	{x = LandscapeWidth() - 120, y = 475, team = 2}];
}
