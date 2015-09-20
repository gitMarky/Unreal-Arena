/**
 Adds an effect to the crew when they relaunch.
 The effect kills the crew if they fall to deep.
 @author Marky
 @version 0.1
 */
 
 #appendto RelaunchContainerEx

public func OnRelaunchCrew(object crew)
{
	AddEffect("GameKillzone", crew, 1, 1, nil);
	_inherited(crew);
}
