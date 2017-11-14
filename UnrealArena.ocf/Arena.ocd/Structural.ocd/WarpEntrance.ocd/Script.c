
/* -- Properties -- */

local team, color;
local target_left, target_right, target_up, target_down, target_single;

local Name = "$Name$";
local Description = "$Description$";


public func ActivateEntrance( object pObj )
{
/*
	SetEntrance(1);


	var tele = false;
	if( tgt1 && !tgt2 && !tgt3) {ScheduleCall(this,"Teleport",2,0,pObj,tgt1); tele = true;}//Teleport(pObj,tgt1);
	if(!tgt1 &&  tgt2 && !tgt3) {ScheduleCall(this,"Teleport",2,0,pObj,tgt2); tele = true;}//Teleport(pObj,tgt2);
	if(!tgt1 && !tgt2 &&  tgt3) {ScheduleCall(this,"Teleport",2,0,pObj,tgt3); tele = true;}//Teleport(pObj,tgt3);

	if(!tele)
	{
		ScheduleCall(this,"Teleport",15,0,pObj,this);
	}

	Schedule("SetEntrance()",2);
	return 1;
*/
}
/*

public func ContainedUp( object obj )
{
	Teleport( obj, tgt3);
	return 1;
}

public func ContainedRight( object obj )
{
	Teleport( obj, tgt2);
	return(1);
}


public func ContainedLeft( object obj )
{
	Teleport( obj, tgt1);
}
*/

public func Teleport(object bot, object target)
{
	if (!bot) return;
	if (target == this && (bot->Contained() != this))
	{
		return;
	}

	bot->Exit();
	if (target)
	{
		bot->SetPosition(target->GetX(), target->GetY());
		bot->SetComDir(COMD_Stop);
	}
}


public func SetTeam(int team)
{
	return this;
}

public func SetGlowColor(value)
{
	if (GetType(value) == C4V_PropList)
	{
		color = value;
	}
	else
	{
		color = SplitRGBaValue(value);
	}
	return this;
}

public func SetTargetSingle(object target)
{
	target_single = target;
	return this;
}

public func SetTargetLeft(object target)
{
	target_left = target;
	return this;
}

public func SetTargetRight(object target)
{
	target_right = target;
	return this;
}

public func SetTargetUp(object target)
{
	target_up = target;
	return this;
}


public func SetTargetDown(object target)
{
	target_down = target;
	return this;
}


public func Create(bool reconnect)
{
    if (target_single && reconnect)
    {
                      target_single->SetTeam(team)->SetGlowColor(color)->SetTargetSingle(this)->Create();
    }
	if (target_left)  target_left ->SetTeam(team)->SetGlowColor(color)->SetTargetSingle(this)->Create();
	if (target_right) target_right->SetTeam(team)->SetGlowColor(color)->SetTargetSingle(this)->Create();
	if (target_up)    target_up   ->SetTeam(team)->SetGlowColor(color)->SetTargetSingle(this)->Create();
	if (target_down)  target_down ->SetTeam(team)->SetGlowColor(color)->SetTargetSingle(this)->Create();
	
	var fx = CreateEffect(FxGlow, 110, 1);
	fx.R = color.R;
	fx.G = color.G;
	fx.B = color.B;
	fx.Alpha = color.Alpha;
	
	SetLightColor(RGB(color.R ?? 0, color.G ?? 255, color.B ?? 0));
	SetLightRange(100, 70);
}


local FxGlow = new Effect
{
	Timer = func (int time)
	{
		var particles = 
		{
			Prototype = Particles_Magic(),
			Size = PV_Random(2, 3),
			R = this.R ?? 0,
			G = this.G ?? 255,
			B = this.B ?? 0,
			Alpha = PV_Linear(this.Alpha ?? 128, 0),
		};
		if (time % 5 == 0)
		{
			this.Target->CreateParticle("Magic", PV_Random(-8, +8), PV_Random(+10, -5), 0, PV_Random(-7, -2), PV_Random(25, 40), particles);
			this.Target->CreateParticle("Magic", PV_Random(-5, +5), PV_Random( +7, -2), 0, PV_Random(-7, -2), PV_Random(25, 40), particles, 2);
		}
		this.Target->CreateParticle("Magic", PV_Random(-1, +1), 7, 0, -4, 60, {Prototype = particles, Size = PV_Random(15, 25), Alpha = PV_Linear(3, 0)});
	},
};
