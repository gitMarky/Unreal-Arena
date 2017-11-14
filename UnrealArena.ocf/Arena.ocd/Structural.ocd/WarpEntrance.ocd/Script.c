
/* -- Properties -- */

local team, color;
local target_left, target_right, target_up, target_down, target_single;

local Name = "$Name$";
local Description = "$Description$";

/* -- Interaction -- */

// Players can interact
func IsInteractable(object clonk)
{
	return target_left || target_right || target_up || target_down || target_single;
}




// Adapt appearance in the interaction bar.
public func GetInteractionMetaInfo(object clonk)
{
	var text;
	return { Description = "Teleport", IconName = nil, IconID = Hammer };
}


// Called on player interaction.
public func Interact(object bot)
{
	if (target_single)
	{
		Teleport(bot, target_single);
	}
	else
	{
		var fx = GetEffect("FxTeleportControl", bot);
		if (!fx)
		{
			fx = bot->CreateEffect(FxTeleportControl, 1, 1);
		}
		fx.teleport = this;
	}
	return true;
}


public func Teleport(object bot, object target)
{
	if (!bot) return;
	if (target == this)
	{
		return;
	}

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

local FxTeleportControl = new Effect
{
	Control = func (int ctrl, int x, int y, int strength, bool repeat, int status)
	{
		if (status == CONS_Up)
		{
			this.kill = true;
			if (ctrl == CON_Up)
			{
				this.teleport->Teleport(this.Target, this.teleport.target_up);
				return true;
			}
			if (ctrl == CON_Down)
			{
				this.teleport->Teleport(this.Target, this.teleport.target_down);
				return true;
			}
			if (ctrl == CON_Left)
			{
				this.teleport->Teleport(this.Target, this.teleport.target_left);
				return true;
			}
			if (ctrl == CON_Right)
			{
				this.teleport->Teleport(this.Target, this.teleport.target_right);
				return true;
			}
		}
		return true;
	},
	
	Timer = func ()
	{
		if (this.kill || !this.teleport)
		{
			return FX_Execute_Kill;
		}
		return FX_OK;
	},
};
