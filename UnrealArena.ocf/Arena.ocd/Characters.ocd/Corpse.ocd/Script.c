#include Library_ModularAppearance
#include Clonk_Animations

static const ANIM_SLOT_Min = 1;

func Initialize()
{
	_inherited(...);
	SetAction("Be");
	vertex_adjust = [0, 0];
}

local animation_main;
local animation_side;
local animation_slot;
local angle_stable = 0;
local angle_prohibited = [-80, 80];
local vertex_adjust;

/**
 Starts the corpse effects.

 @par animation_speed Linear value, where 1000 is the normal animation speed.
                      The speed is scaled by the gravity value additionally,
 @par on_ground If true, the corpse will play the normal death animation.
 */
func StartSplatter(int animation_speed, bool on_ground)
{
	var animation_length;

	if (on_ground)
	{
		animation_main = "Dead";
		animation_side = nil;
		animation_length = 20;
		CreateEffect(FxInterpolateVertices, 1, 1);
		angle_stable = 0; // TODO: interpolation again
	}
	else
	{
		var side = "L"; if (Random(2)) side = "R";
		animation_main = Format("JumpUp.%s", side);
		animation_side = "Stand";
		animation_length = 40;
		angle_stable = 90;
	}

	var death = false;

	//var animation_duration = animation_length * Sqrt(100 / Max(1, GetGravity()));
	var animation_duration = animation_length * Sqrt(20 / Max(1, GetGravity()));
	animation_duration = Max(1, BoundBy(animation_speed, 0, 1000) * animation_duration / 1000);

	// Bsp: 20
	// Gravity 100
	// duration = 20 * sqrt(1); ...
	// Gravity 50 => 20 * Sqrt (2) => 34
	// Gravity 200 => 20 * Sqrt (100/200) = ...
	// Gravity 200 => 20 * Sqrt(10000 / 200) = 20 * Sqrt (50) / 10

	// Overlay death animations except for the death slot
	for (var slot = 0; slot < CLONK_ANIM_SLOT_Death; ++slot)
	{
		var number = GetRootAnimation(slot); 
		if (number == nil) continue;

		death = true;
		OverlayDeathAnimation(slot, animation_duration, animation_main, animation_side);
	}

	//if (!death) // add the animation on the lowest slot, so that blending is not disturbed
	//{
		OverlayDeathAnimation(ANIM_SLOT_Min, animation_duration, animation_main, animation_side);
	//}

	// Update carried items
	UpdateAttach();
	// Set proper turn type
	SetTurnType(1);

	// Stabilize the corpse
	CreateEffect(FxStabilize, 1, 1);
	CreateEffect(FxDisintegrate, 1, 1);
}

// animation stuff

func OverlayDeathAnimation(int slot, int animation_duration, string animation1, string animation2)
{
	if (animation1 == nil)
	{
		FatalError("You have to pass at least one animation");
	}

	var weight = 500;
	if (animation2) weight = 300;

	// Play the animations

	PlayAnimation(animation1, slot, Anim_Linear(0, 0, GetAnimationLength(animation1), animation_duration, ANIM_Hold), Anim_Const(weight));

	if (animation2)
	{
		PlayAnimation(animation2, slot, Anim_Linear(0, 0, GetAnimationLength(animation2), animation_duration * 2, ANIM_Hold), Anim_Const(weight));
	}
}

func CopyAnimationPositionFrom(object target)
{
	// Copy all animations except for the death slot from the target
	for (var slot = 0; slot < CLONK_ANIM_SLOT_Death; ++slot)
	{
		var number = target->GetRootAnimation(slot); 
		if (number == nil) continue;

		var name = target->GetAnimationName(number);
		if (!name) continue; // for simplicity we skip combination nodes

		var position = target->GetAnimationPosition(number);

		var range = 30;
		PlayAnimation(name, slot, Anim_Linear(position, 0, GetAnimationLength(name), range, ANIM_Hold), Anim_Linear(0, 1000, range, ANIM_Remove));
	}
}

func Flinch(int duration)
{
	if (animation_main == "Dead") return;

	var stand = animation_side ?? animation_main ?? "Stand";
	PlayAnimation(stand, CLONK_ANIM_SLOT_Movement, Anim_Linear(0, 0, GetAnimationLength(stand), duration ?? 2, ANIM_Remove));
}

local ActMap = {
Be = {
	Prototype = Action,
	Name = "Be",
	Procedure = DFA_NONE,
	Directions = 2,
	FlipDir = 0,
	Length = 1,
	Delay = 0,
	X = 0,
	Y = 0,
	Wdt = 8,
	Hgt = 20,
},
};

// vertex configurations

func VertexSetupLegs()
{
	var permanent = 2;
	SetVertex(1, VTX_Y, 2, permanent);
	SetVertex(3, VTX_Y, 2, permanent);
	SetVertex(4, VTX_Y, 2, permanent);
	SetVertex(5, VTX_X,-2, permanent);
	SetVertex(6, VTX_X, 2, permanent);
	SetVertex(5, VTX_Y, 4, permanent);
	SetVertex(6, VTX_Y, 4, permanent);

	ApplyOffset(0, -8000);
}

func VertexSetupBody()
{
	var permanent = 2;
	SetVertex(1, VTX_Y, -4, permanent);
	SetVertex(2, VTX_Y, 2, permanent);
	SetVertex(5, VTX_X, 0, permanent);
	SetVertex(6, VTX_X, 0, permanent);
	SetVertex(7, VTX_Y, 0, permanent);
	SetVertex(8, VTX_Y, 0, permanent);

	ApplyOffset(0, -3000);
}

func VertexSetupHead()
{
	ChangeVertex(0, 0, -3, CNAT_Top);
	ChangeVertex(1, 0, +3, CNAT_Bottom);
	ChangeVertex(2, -3, 0, CNAT_Left);
	ChangeVertex(3, +3, 0, CNAT_Right);

	for (var i = 4; i < 9; ++i)
	{
		ChangeVertex(i, 0, 0, CNAT_Center);
	}

	ApplyOffset(0, 7000);

	CreateEffect(FxDismembered, 1, 1);
}


func ChangeVertex(int index, int x, int y, int cnat)
{
	SetVertex(index, VTX_X, x, VTX_SetPermanentUpd);
	SetVertex(index, VTX_Y, y, VTX_SetPermanentUpd);
	SetVertex(index, VTX_CNAT, cnat, VTX_SetPermanentUpd);
}

func VertexSetupArmL()
{
	var position = +1 * GetXDirection();
	VertexSetupArm(position);
}

func VertexSetupArmR()
{
	var position = -1 * GetXDirection();
	VertexSetupArm(position);
}

func VertexSetupArm(int position)
{
	var permanent = 2;

	for (var i = 0; i < 9; ++i) SetVertex(i, VTX_X, position, permanent);

	SetVertex(0, VTX_Y,  0, permanent);
	SetVertex(1, VTX_Y, -3, permanent);
	SetVertex(2, VTX_Y, +3, permanent);
	SetVertex(3, VTX_Y, -2, permanent);
	SetVertex(4, VTX_Y, +2, permanent);
	SetVertex(5, VTX_Y, -1, permanent);
	SetVertex(6, VTX_Y, +1, permanent);
	SetVertex(7, VTX_Y, -1, permanent);
	SetVertex(8, VTX_Y, +1, permanent);

	ApplyOffset(0, -5000);
}

func ApplyOffset(int x, int y)
{
	var precision = 1000;

	MovePosition(-x, -y, precision);
	vertex_adjust[VTX_X] = -x / precision;
	vertex_adjust[VTX_Y] = -y / precision;

	var fx = CreateEffect(FxApplyOffset, 1, 1);
	fx.x = x; fx.y = y;
	fx->Timer();
}

local FxApplyOffset = new Effect
{
	Timer = func ()
	{
		var r = Target->GetR();
		var s_x = Sin(r, this.x);
		var c_x = Cos(r, this.x);
		var s_y = Sin(r, this.y);
		var c_y = Cos(r, this.y);
		Target->SetObjDrawTransform(1000, 0, c_x - s_y, 0, 1000, s_x + c_y);
	},
};

func GetXDirection()
{
	return -1 + 2 * GetDir();
}


func IsDismembered()
{
	var fx = GetEffect("FxDismembered", this);
	if (fx)
	{
		return fx.Time;
	}
	else
	{
		return false;
	}
}

local FxDismembered = new Effect
{
	Timer = func (int time)
	{
		if (time < 50) this.Target->EffectBloodSpray(1);
	},
};


local FxDisintegrate = new Effect
{
	Timer = func (int time)
	{
		if (time > 200)
		{
			if (this.Target) this.Target->Disintegrate(20, 50, -5, true);

			return FX_Execute_Kill;
		}
	}
};


local FxStabilize = new Effect
{
	Timer = func ()
	{
		if (this.Target.is_dismembered) return FX_Execute_Kill;

		var rdir = this.Target->GetRDir();
		var angle = Normalize(this.Target->GetR(), -180);
		var angle_desired = this.Target.angle_stable;
		if (angle < 0) angle_desired *= -1;

		var diff = angle_desired - angle;

		if (this.Target->GetContact(-1))
		{
			var rdir_add = Sin(diff, 10); // TODO: find real value

			var factor = 1;

			if (Inside(angle, Normalize(angle_desired + this.Target.angle_prohibited[0], -180), Normalize(angle_desired + this.Target.angle_prohibited[1], -180)))
			{
				factor = 0;
			}

			this.Target->SetRDir(factor * rdir + rdir_add / 2); // TODO: check if this is correct
		}
	}
};


local FxInterpolateVertices = new Effect
{
	Start = func (bool temp)
	{
		if (temp) return;

		this.old_index = 0;
		this.new_index = 0;

		// DefCore vertices
		var x_00 = [0,  0,  0, -2,  2, -4,  4, -2,  2];
		var y_00 = [2, -7,  8, -3, -3,  2,  2,  6,  6];

		// Transitions
		var x_02 = [0,  0,  0, -2,  2, -4,  3, -2,  2];
		var x_03 = [0, -1,  0, -2,  2, -4,  2, -2,  2];
		var x_04 = [0, -2,  0, -2,  2, -4,  2, -2,  2];
		var x_05 = [0, -3,  0, -2,  1, -3,  2, -2,  2];
		var x_06 = [0, -4,  0, -3,  1, -2,  2, -1,  3];
		var x_07 = [0, -5,  1, -4,  1, -1,  2, -1,  3];
		var x_08 = [0, -5,  2, -4,  0, -1,  3,  0,  3];
		var x_09 = [0, -6,  3, -4,  0,  0,  4,  3,  4];
		var x_12 = [0, -7,  4, -4,  0,  0,  4,  3,  4];
		var x_13 = [0, -7,  5, -4,  0,  0,  4,  4,  5];
		var x_14 = [0, -8,  6, -4,  0,  0,  4,  5,  6];
		var x_15 = [0,  0,  6, -4, -8,  0,  4,  5,  6]; //
		var x_16 = [0,  0,  0, -4,  4, -8,  8, -4,  4];

        //          C   H   F   S   S   A   A   L   L

		var y_07 = [2, -6,  9, -3, -3,  2,  2,  6,  6];
		var y_08 = [2, -5,  9, -2, -2,  2,  2,  6,  6];
		var y_09 = [2, -4,  9,  2, -1,  2,  2,  6,  6];
		var y_10 = [2, -3,  9,  2, -3,  2,  2,  6,  6];
		var y_11 = [2, -2,  9,  2, -2,  2,  2,  6,  6];
		var y_12 = [2, -1,  9,  2, -1,  2,  2,  6,  6];
		var y_13 = [3,  0,  9,  3,  0,  3,  3,  6,  6];
		var y_14 = [4,  2,  9,  4,  0,  4,  4,  6,  6];
		var y_15 = [5,  0,  9,  5,  2,  5,  4,  6,  6]; //
		var y_16 = [5,  2,  9,  5,  5,  7,  7,  7,  7];
		var y_17 = [7,  4,  9,  6,  6,  7,  7,  8,  8];

		// Dummy death vertices
		var x_20 = [0,  0,  0, -4,  4, -8,  8, -4,  4];
		var y_20 = [7,  5,  9,  6,  6,  7,  7,  8,  8];

		var vertices_x = [x_00, x_00, x_02, x_03, x_04, x_05, x_06, x_07, x_08, x_09, x_09, x_09, x_12, x_13, x_14, x_15, x_16, x_20, x_20, x_20, x_20, x_20];
		var vertices_y = [y_00, y_00, y_00, y_00, y_00, y_00, y_00, y_07, y_08, y_09, y_10, y_11, y_12, y_13, y_14, y_15, y_16, y_17, y_20, y_20, y_20, y_20];

		// Save to effect
		this.target_vertices = [vertices_x, vertices_y];
		this.actual_vertices = [[0,  0,  0, -2,  2, -4,  4, -2,  2],
		                        [2, -7,  8, -3, -3,  2,  2,  6,  6]];
	},

	SetVertexPos = func(int v, int x, int y)
	{
		x *= this.Target->GetXDirection();
		this.Target->SetVertex(v, VTX_X, x, VTX_SetPermanentUpd);
		this.Target->SetVertex(v, VTX_Y, y, VTX_SetPermanentUpd);
		this.actual_vertices[VTX_X][v] = x;
		this.actual_vertices[VTX_Y][v] = y;
	},

	InterpolateVertex = func (int v, array target_x, array target_y, array actual_x, array actual_y)
	{
		var y_adjust = this.Target.vertex_adjust[VTX_Y];

		// Cycle through interpolated positions, beginning at the furthest away position
		// => Go back slowly until you are, at worst, at the previous position
		var max_dist = 5;
		for (var dist = max_dist; dist >= 0; --dist)
		{
			var x = dist * target_x[v] + (max_dist - dist) * actual_x[v];
			var y = dist * target_y[v] + (max_dist - dist) * actual_y[v];

			x /= max_dist; y /= max_dist;

			SetVertexPos(v, x, y + y_adjust);

			if (!this.Target->Stuck())
			{
				break;
			}
		}
	},

	Timer = func()
	{
		// Get data
		var number = this.Target->GetRootAnimation(ANIM_SLOT_Min);
		var pos = this.Target->GetAnimationPosition(number);
		var max = this.Target->GetAnimationLength(this.Target->GetAnimationName(number));

		// Update array indices for interpolation
		this.old_index = this.new_index;
		this.new_index = pos * Max(0, GetLength(this.target_vertices[VTX_X]) - 1) / max;

		var target_x = this.target_vertices[VTX_X][this.new_index];
		var target_y = this.target_vertices[VTX_Y][this.new_index];
		var actual_x = this.actual_vertices[VTX_X];
		var actual_y = this.actual_vertices[VTX_Y];

		var stuck_before = this.Target->Stuck();

		for (var v = 0; v < Target->GetVertexNum(); ++v)
		{
			// If you are stuck anyway just update the vertex position, cannot get so much worse
			if (stuck_before)
			{
				SetVertexPos(v, target_x[v], target_y[v]);
			}
			// Try going towards a vertex position where you are not stuck
			else
			{
				InterpolateVertex(v, target_x, target_y, actual_x, actual_y);
			}
		}
	}
};

// override functions that are expected by clonk animations

func IsWalking(){ return false;}
func StartSearchLadder(){}
func PlaySoundIdle(){}


// contact and physics
//
// TODO: The looks could be improved greatly if the CNAT are set correctly and
// - the RDir is updated if you contact bottom and have xdir; maybe calculate a vector and make it into angular speed?

local ContactCalls = true;

func ContactBottom()
{
	return BouncePhysics();
}

func ContactTop()
{
	return BouncePhysics();
}

func ContactLeft()
{
	return BouncePhysics();
}

func ContactRight()
{
	return BouncePhysics();
}

func BouncePhysics(bool allow_bounce)
{
	if (!GetXDir() && !GetYDir()) return;

	// Bounce not so often
	if (!GetEffect("IntNoBounce", this) && (IsDismembered() || allow_bounce))
	{
		SetRDir(-(3 * GetRDir()) / 2);
		AddEffect("IntNoBounce", this, 1, 20, this);

		if (IsDismembered() < 90)
		{
			EffectBloodSpray(12, BoundBy(30 * Distance(GetXDir(), GetYDir()) / 200, 5, 30));
		}
	}

	return true;
}

protected func Hit(int dx, int dy)
{
	if (dy > 1)
	{
		SetYDir(dy / -4, 100);
	}

	Flinch();
	BouncePhysics(true);
}

/**
 Can be hit by explosions only.
 */
func IsProjectileTarget(object projectile, object shooter)
{
	if (MOD_NoBlood()) return false;
	return projectile && projectile->GetID() == Projectile_Explosion;
}

func OnWeaponDamageShooter(object projectile, int damage_amount, int damage_type)
{
	if (IsDisintegrating()) return;
	EffectCastGore(10, 30 + damage_amount / 4, 0, 3);
	EffectBloodSpray(damage_amount * 6, 30);
	RemoveObject();
}
