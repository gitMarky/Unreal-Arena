/*---- Unreal Bot ----*/

#strict 2

//#include HZCK
#include CLNK
#include L_DR	// can have drone (with context menu)
#include L_GE	// can use gear (with context menu)
#include L_LA	// can climb on ladders
#include L_AG	// is agil
#include L_JP	// can use jetpack
#include L_CA	// can aim

local iTumbleDir;
local iAimPhase;

static const AIM_Max = 179;

protected func Initialize()
{
	//dieser Effekt ist für die Waffen in den Händen zuständig
	wpneffect = AddEffect("ShowWeapon",this,1,1,this,HZCK);
//	//Energie fixen (kA wo das gechanged wird...) UH H4X
//	DoEnergy( -GetPhysical("Energy") / 1000 + 1); //Noch 1 Energie
//	SetPhysical("Energy", 150000, 2); //Physical ändern
//	DoEnergy( GetPhysical("Energy") / 100 - 1); // Neu dazugeben
//  // Munitionsgürtel
//  AmmoStoring();
//  GetHUD(this());
  // Schadenseffekt
	AddEffect("DmgCheck", this, 1, 0);

	return _inherited();
}

private func CheckArmed()
{
	var a = GetAction();
	if (a == "Walk" || a == "Jump")
	{
		UpdateAiming();
		//SetAimPhase(GetAimPhase());
	}
//	if(!GetEffect("UTFixedDir",this))
//		AddEffect("UTFixedDir",this,50,1,this);
}

private func Departure( pContainer )
{
	if(GetID(pContainer) == TIM2 )
	{
		SetGraphics("Quake");
		if(MOD_TeamBeacon() && GetTeamCount())
		{
			// einmal eine Taschenlampe
			var iPlr = GetOwner();
			var team = GetPlayerTeam(iPlr);
			var clr = GetTeamColor(team);
			clr = SetRGBaValue(clr, 90, 0);

			var flash = CreateObject(FLSH,0,0,-1);
			flash->~Activate(this);
			flash->~GearEffect(this,clr);

			  // dann noch ein Lichtschein
			  var tmp = AddLightAmbience(30,this);
			  tmp->ChangeColor(SetRGBaValue(GetTeamColor(team),30,0));
			  SetVisibility(VIS_All,tmp);

		}

		if(GetPlayerType(GetOwner())==C4PT_Script)
			if(!GetEffect("UTBotAIActivity"))
				AddEffect("UTBotAIActivity",this,50,1,this);
	}

	CheckArmed();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

public func CheckTumble()
{
	//SetDir(dir);
	if(!GetDir())
	{
		if(GetXDir()<0) { SetAction("TumbleForw"); iTumbleDir=1; return; }
		if(GetXDir()>0) { SetAction("TumbleBack"); iTumbleDir=2; return; }
	}
	else
	{
		if(GetXDir()>0) { SetAction("TumbleForw"); iTumbleDir=1; return; }
		if(GetXDir()<0) { SetAction("TumbleBack"); iTumbleDir=2; return; }
	}

	SetAction("TumbleNorm");
}

public func CheckFlatUp()
{
	if(iTumbleDir==1) { SetAction("FlatUpForw"); iTumbleDir=0; }
	if(iTumbleDir==2) { SetAction("FlatUpBack"); iTumbleDir=0; }
}

public func IsAiming()
{
	var a = GetAction();
	if( a == "Tumble" || a == "TumbleNorm" || a == "Push" || a == "Dead" || a == "FlatUp" || a == "FlatUpForw" || a == "FlatUpBack" || a == "BackFlip" || a == "ForwardFlip" || a == "Scale" || a == "ScaleDown" || a == "ScaleLadder" )
		return false;

	return true;
}


protected func ControlCommand(szCommand, pTarget, iTx, iTy, pTarget2, Data)
{
	ClearMacroCommands();
	SetAggroLevel(0,0,0,0,"ControlCommand");

	// Context -> Keine Sonderbehandlung
	if(szCommand == "Context") return 0;

	// Aus UA alt
	if(szCommand != "MoveTo")
	if(szCommand != "Dig")
	if(szCommand != "Chop")
		return 0;

	if (szCommand == "MoveTo" || szCommand == "Dig")
	{
		SetCommand(this,"None"); // Auch aus UA alt

		if (Control2Grab("ControlCommand")) return 0;

		if (ControlAim("ControlCommand",Par(1),Par(2),Par(3))) return 1;
		// Klettert an einer Leiter
		if (ControlLadder("ControlCommand")) return 1;
  }
  // Sonst nicht abfangen
  return(_inherited(szCommand, pTarget, iTx, iTy, pTarget2, Data) );
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/* Waffe/Feuern */

public func ReadyToFire()
{
	// der hier feuert eigentlich aus allen Aktionen :D

   var a = GetAction();

   if(IsAiming()) return true;

   if(a == "Swim" || a == "KneelDown" || a == "KneelUp")
		return true;

   // JetpackFlight fehlt noch, sowie Aim*... das steht in den Funktionalitäten
   return _inherited();
}

public func WeaponAt(&x, &y, &r) {

	if(Contained()) return 0;

	var a = GetAction();
	var p = GetPhase();

	// x < 0: Richtung hinter den Clonk
	// x > 0: Richtung vor den Clonk
	x=0; y=0; r=0;

	// hält Waffe in der Hand
	/*if(a == "Walk")
	{
		x=0;
		y=1000;
		return 1;
	}
	else if(a == "Jump")
	{
		x=500-125*p;
		y=1000;
		return 1;
	}
	else*/ if(a == "Swim")
	{
		x=4000;
		return 1;
	}
	//Jetpack
	else if(a == "JetpackFlight")
	{

		x=4000;
		r=35;
		return 1;
	}
	else if(a == "KneelDown")
	{
		x=0;
		y=1000;
		return 1;
	}
	else if(a == "KneelUp")
	{
		x=0;
		y=1000;
		return 1;
	}
	/*else if(a == "TumbleForw")
	{
		x=0;
		y=1000;
		return 1;
	}
	else if(a == "TumbleBack")
	{
		x=0;
		y=1000;
		return 1;
	}

*/
	// zielt mit der Waffe
	else if(IsAiming())
	{
		if(!crosshair)
			InitCrosshair();
		//if(p>4) r=(p-4)*11;
		//else		r=p*23-90;
		r = (Abs(crosshair->GetAngle())-90);
		x=-Sin(65+iAimPhase*20,4000)-1500;
		y=-Sin(55+iAimPhase*10,5000);

		if(WildcardMatch(a,"Walk*") || WildcardMatch(a,"Jump*") || a == "TumbleForw" || a == "TumbleBack")
			a = "AimLow";

		if( a == "Aim")
		{
			x = 0;
			y += 2000;
		}


		if(a == "AimLow")
		{
			x=0;//-Sin(65+iAimPhase*20,4000)-1500;
			y += 4000;
		}
		if(a == "AimSquat")
		{
			x += 5000;
			y += 4500;
		}
		if(a == "AimSquatLow")
		{
			x += 5000;
			y += 6000;
		}
		return 1;
	}

	// hat Waffe auf dem Rücken
	else
	{
		r=90;
		x=-3000;
		y=-2000;

		if(a == "Scale" || a == "ScaleDown" || a=="ScaleLadder")
		{
			x-=1000-Cos(p*45,250);
			return 1;
		}
		else if(a == "Push")
		{
			r=100;
			x+=1000-Cos(p*90,350);
			y-=2000;
			return 1;
		}
		else return 0;
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/* Control from the clonk */
private func ControlAgility(string strControl)
{
	// return 0 if the rest of the clonks script should be executed

	// left
	if(strControl == "ControlLeft")
	{
		if(this()->IsJumping())
		{
			if(GetDir() == DIR_Left && GetAction() != "JetpackFlight") SetXDir(-20);
			SetDir(DIR_Left);
			SetComDir(COMD_Left);
		}

		var iEff = AddEffect("ControlStack", this, 110, 5, this);
		EffectVar(0, this, iEff) = COMD_Left;
		//ScheduleCall(this,"UpdateAiming",1);
		return 0;
	}

	// right
	if(strControl == "ControlRight")
	{
		if(this()->IsJumping())
		{
			if(GetDir() == DIR_Right && GetAction() != "JetpackFlight") SetXDir(+20);
			SetDir(DIR_Right);
			SetComDir(COMD_Right);
		}

		var iEff = AddEffect("ControlStack", this, 110, 5, this);
		EffectVar(0, this, iEff) = COMD_Right;
		//ScheduleCall(this,"UpdateAiming",1);
		return 0;
	}
/*
	// left double
	if(strControl == "ControlLeftDouble")
	{
		if(MayFlip())
		{
			if(GetDir() == DIR_Left)
				ForwardFlip(-30);
			else
				BackFlip(-30);
				return 1;
		}
	}
	// right double
	if(strControl == "ControlRightDouble")
	{
		if(MayFlip())
		{
			if(GetDir() == DIR_Right)
				ForwardFlip(-30);
			else
				BackFlip(-30);
				return 1;
		}
	}
*/

	// up
	if(strControl == "ControlUp")
	{
		if((this()->IsJumping() || GetAction() == "JetpackFlight") && !GetEffect("ExtraJump", this))
			//if(GetYDir() <= 5)
			if(Inside(GetActTime(),12,20))
				return ExtraJump();
			if(WildcardMatch(GetAction(), "Scale*") && GetActTime() < 5)
				EffectVar(2, this, AddEffect("ControlStack", this, 110, 5, this)) = true;
			if(GetEffect("ControlStack", this))
				EffectVar(1, this, GetEffect("ControlStack", this)) = COMD_Up;
			return 0;
	}

}

private func MayFlip(){ return false; }

/* Jump functions */

protected func ExtraJump()
{
	SetYDir(GetYDir()-(GetPhysical("Jump")/1000));
	//var jump = GetPhysical("Jump", 0) * 100 / GetPhysical("Jump", 0,0, GetID());
	//SetYDir(-40 * jump / 100);
	//SetXDir(GetXDir() / 2);
	AddEffect("ExtraJump", this, 111, 5, this);
}

public func JumpStart(bool bBackflip)
{
	// Clonk auf festem Boden?
	if (!GetEffect("ExtraJump", this)) {
		var iEff, iComd, iStr = 0, iYDir = 0;
		if(iEff = GetEffect("ControlStack", this))
		{
			if(EffectVar(1, this, iEff) != COMD_Up && !bBackflip) return;
			if(EffectVar(2, this, iEff)) { iYDir = -60; iStr = -40; }
			iComd = EffectVar(0, this, iEff);

			RemoveEffect(0, this, iEff);
			/*if(iComd == COMD_Left)
			{
				if(GetDir() == DIR_Left)
					ForwardFlip(iStr, 0, iYDir);
				else
					BackFlip(iStr, 0, iYDir);
			}
			if(iComd == COMD_Right)
			{
				if(GetDir() == DIR_Left)
					BackFlip(iStr, 0, iYDir);
				else
					ForwardFlip(iStr, 0, iYDir);
			}*/
		}
	}
	// für ladder
	_inherited(bBackflip);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


// Muss Aktion "Aim" haben (und "AimLow")

protected func ControlAim(string command) {

	if(!IsAiming()) return false;

	if(command == "ControlCommand")
	{
		if(!crosshair)
			InitCrosshair();

		return DoMouseAiming(Par(2),Par(3));
	}
/*
	else if(command == "ControlContents")
	{

		var pTarget = FindContents(Par(1));
		ChangeWeapon(pTarget);
		return false;

	}
*/
	return false;
	//return true;
}


public func UpdateAiming()
{
	if(!IsAiming() || !crosshair)
		return;


	var a = crosshair->GetAngle();

	//Position des Attachvertex so verschieben, dass das Kreuz von der Waffe aus ist
	UpdateVertices();

	if((GetDir() == DIR_Left && a > 0) ||
	   (GetDir() == DIR_Right && a < 0))
	crosshair->ChangeDir();

	//if(!GetEffect("UTFixedDir",this))
	//	AddEffect("UTFixedDir",this,50,1,this);
	//SetFixedDir(GetDir());

	SetPlrViewRange( GetSight() );

	SetAimPhase(AimAngle2Phase(a));
}

private func MayAimPhase()
{
	var a = GetAction();
	if( WildcardMatch(a, "Walk*") || WildcardMatch(a,"Jump*") )
		return false;

	return true;
}

private func MayAimPhase2()
{
	var a = GetAction();
	if( WildcardMatch(a, "Kneel*") )
		return false;

	return true;
}

private func SetAimPhase( int iPhase)
{
	iAimPhase = iPhase;

	if( MayAimPhase())
	{
		if(MayAimPhase2()) SetPhase(iPhase);
	}
	else
	{
		var a = GetAction();
		var p = GetPhase();

		if(WildcardMatch(a,"Walk*")) a="Walk";
		if(WildcardMatch(a,"Jump*")) a="Jump";

		a = Format("%s%d",a,iPhase);
		SetAction(a);
		SetPhase(p);
	}
}

private func AimAngle2Phase(int angle) {
  angle = Normalize(angle,-180);
  var phases = 7;//GetActMapVal("Length",GetAction());
  var newphase = Min( phases-1, phases*Abs(angle)/AimMax() );
  return (newphase);
}

private func GetAimPhase()
{
	var a = GetAction();

	if(a == "KneelDown" || a == "KneelUp" || a == "Swim") return 3;

	return iAimPhase;
}

// Hier kommt die eine Spezialtaste rein
protected func CrewSelection( bool fDeselect, bool fCursorOnly )
{
	if( fCursorOnly )
	{
		if ( Control2Contents("ControlSpecial3") ) return 1;
	}
}

protected func ControlDig()//Single()
{
	local bHackDigStanding;
	if(!GetXDir()) bHackDigStanding = true;
	ClearLastPlrCom(GetController());
  ClearMacroCommands();
  SetAggroLevel(0,0,0,0,"ControlDig");
  if (Control2Grab("ControlDig")) return 0;
  if (Control2Contents("ControlDig")) return 1;
  if (ControlLadder("ControlDig") ) return 1;
  return 1;//return _inherited();
}

protected func HackDig()
{
	if(bHackDigStanding)
	{
		SetComDir(COMD_Stop);
		bHackDigStanding = false;
	}
}



public func GetTarget(int iAngle, int iMaxAngle, int iRange)
{
	return 0; // der hier hat kein Autoaim!!
}

static const g_iPlrViewRange = 450;

protected func GetSight()
{
	var i = g_iPlrViewRange;

	if(Contents()) i+=Contents()->~GetFMData(FM_SightBonus);

	return i;
}


public func SetFixedDir(dir)
{
	var effect = GetEffect("UTFixedDir", this);

	if (!effect)
	{
		effect = AddEffect("UTFixedDir", this, 50, 1, this);
	}

	if (effect)
	{
		EffectVar(g_FX_FixedDir_Direction, this, effect) = dir;
		EffectVar(g_FX_FixedDir_Timer, this, effect) = 120;
	}
}
