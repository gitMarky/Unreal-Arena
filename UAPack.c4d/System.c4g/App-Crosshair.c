/*-- Fadenkreuz --*/

#strict 2
#appendto HCRH

static const CH_Distance = 90;

public func GetAngle() {
	  var user = GetActionTarget();
	if(user)
	{
		var a = user->GetAction();

		if(a == "KneelDown" || a == "KneelUp" || a == "Swim")
			return -90 + (user->GetDir())*180;

	}
  return(Normalize(my_r,-180));
}

