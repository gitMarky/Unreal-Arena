/*-- Groﬂe Schwebeplatte --*/


/* Lokale Variablen */

//local Mode; // 0: vertikal 1: horizontal 2: beides
//local UpperY, LowerY, WaitComDir, WaitY;
//local rLight, lLight;
//local Limited;
//
//// Begrenzen
//public func SetLimits(int iUY, int iLY)
//{
//	if(iUY) UpperY = iUY;
//	if(iLY) LowerY = iLY;
//	Limited = true;
//}
//
//public func SetAutoLimits() {
//	Limited = true;
//	UpdateTransferZone();
//}
//
///* Steuerung */
//
//public func ControlCommand (string command, object target, int x, int y) {
//  // Bewegungskommando vertikal
//  if (command eq "MoveTo") {
//    if (Vertical())
//    if (Inside(x - GetX(),-16,+16)) {
//      if (y < GetY())
//        return(SetComDir(COMD_Up()));
//      if (y > GetY())
//        return(SetComDir(COMD_Down()));
//    }
//  // Bewegungskommando horizontal
//    if (Horizontal())
//    if (Inside(x - GetY(),-16,+16)) {
//      if (x < GetX())
//        return(SetComDir(COMD_Left()));
//      if (x > GetX())
//        return(SetComDir(COMD_Right()));
//    }
//  }
//  // Andere Kommandos nicht auswerten
//}
//
//public func ControlUpdate(object caller, int comdir, bool dig, bool throw)
//{
//  // Remove horizontal Comdir component
//  if(!Horizontal())
//  {
//    if(ComDirLike(comdir, COMD_Right)) comdir = ComDirTransform(comdir, COMD_Left);
//    if(ComDirLike(comdir, COMD_Left)) comdir = ComDirTransform(comdir, COMD_Right);
//  }
//
//  // Remove vertical ComDir component
//  if(!Vertical())
//  {
//    if(ComDirLike(comdir, COMD_Up)) comdir = ComDirTransform(comdir, COMD_Down);
//    if(ComDirLike(comdir, COMD_Down)) comdir = ComDirTransform(comdir, COMD_Up);
//  }
//
//  if(comdir != GetComDir())
//    Sound("Click");
//
//  // Stop without input
//  if(comdir == COMD_Stop) return(Stop());
//
//  // Clear X/Y-Dir if necessary
//  if(!ComDirLike(comdir, COMD_Right) && !ComDirLike(comdir, COMD_Left))
//    SetXDir(0);
//  if(!ComDirLike(comdir, COMD_Up) && !ComDirLike(comdir, COMD_Down))
//    SetYDir(0);
//
//  // Ab geth.
//  SetComDir(comdir);
//  SetAction("Travel");
//  return(1);
//}
//
//public func ControlUp (object caller) {
//  if(!GetPlrCoreJumpAndRunControl(caller->GetController()))
//  {
//    Sound("Click");
//    if (!Vertical()) return(Stop());
//    SetComDir(COMD_Up());
//    SetXDir(0);
//    SetAction("Travel");
//    return(1);
//  }
//}
//
//public func ControlDownSingle (object caller) {
//  if(!GetPlrCoreJumpAndRunControl(caller->GetController()))
//  {
//    Sound("Click");
//    if (!Vertical()) return(Stop());
//    SetComDir(COMD_Down());
//    SetXDir(0);
//    SetAction("Travel");
//    return(1);
//  }
//}
//
//public func ControlLeft (object caller) {
//  if(!GetPlrCoreJumpAndRunControl(caller->GetController()))
//  {
//    Sound("Click");
//    if (!Horizontal()) return(Stop());
//    SetComDir(COMD_Left());
//    SetYDir(0);
//    SetAction("Travel");
//    return(1);
//  }
//  else
//  {
//    AddCommand(caller, "Call", this(), 0,0,0,0, "ContMoveLeft");
//    AddCommand(caller, "UnGrab", this());
//    return(1);
//  }
//}
//
//public func ControlRight (object caller) {
//  if(!GetPlrCoreJumpAndRunControl(caller->GetController()))
//  {
//    Sound("Click");
//    if (!Horizontal()) return(Stop());
//    SetComDir(COMD_Right());
//    SetYDir(0);
//    SetAction("Travel");
//    return(1);
//  }
//  else
//  {
//    AddCommand(caller, "Call", this(), 0,0,0,0, "ContMoveRight");
//    AddCommand(caller, "UnGrab", this());
//    return(1);
//  }
//}
//
//public func ContMoveLeft(object caller) { caller->SetComDir(COMD_Left); }
//public func ContMoveRight(object caller) { caller->SetComDir(COMD_Right); }
//
///* Kontakt */
//
//protected func ContactTop () {
//  if (!Vertical()) return(Stop());
//  Wait(COMD_Down());
//  return(1);
//}
//
//protected func ContactBottom () {
//  UnstuckClonks();
//
//  if (!Vertical()) return(Stop());
//  if (Horizontal() && Vertical()) return(Stop());
//  Wait(COMD_Up());
//  return(1);
//}
//
//protected func ContactLeft () {
//  if (Horizontal() && Vertical()) return(Stop());
//  if (!Horizontal()) return(Stop());
//  Wait(COMD_Right());
//  return(1);
//}
//
//protected func ContactRight () {
//  if (Horizontal() && Vertical()) return(Stop());
//  if (!Horizontal()) return(Stop());
//  Wait(COMD_Left());
//  return(1);
//}
//
///* Konfiguration */
//
//public func EnableHorizontal () {
//  if (0 == Mode) Mode = 2; else Mode = 1;
//  UpdateTransferZone();
//}
//
//public func DisableHorizontal () {
//  Mode = 0;
//  UpdateTransferZone();
//}
//
//public func EnableVertical () {
//  if (1 == Mode) Mode = 2; else Mode = 0;
//  UpdateTransferZone();
//}
//
//public func DisableVertical () {
//  Mode = 1;
//  UpdateTransferZone();
//}
//
//private func Horizontal () {
//  return(Mode != 0);
//}
//
//private func Vertical () {
//  return(Mode != 1);
//}
//
//public func SetYTop (int iNewTop) {
//  return(UpperY = iNewTop);
//}
//  
//private func YTop () {
//  return(UpperY);
//}
//
//public func SetYBottom (int iNewBottom) {
//  return(LowerY = iNewBottom);
//}
//  
//private func YBottom () {
//  return(LowerY);
//}
//
///* Kommandos */
//
//private func Stop () {
//  SetAction("Travel");
//  SetComDir(COMD_Stop());
//  SetSpeed();
//  return(1);
//}
//
//private func Wait (int iComDir) {
//  Stop();
//  // Aktion Wait ruft als EndCall ResumeTravel auf
//  SetAction("Wait"); 
//
//  WaitComDir = iComDir;
//  return(1);
//}
//
//private func ResumeTravel () { // EndCall von Aktion Wait
//  SetAction("Travel");
//  SetComDir(WaitComDir);
//  return(1);
//}
//
//private func Automatic () { // EndCall von Aktion Travel
//  // Keine Automatik, wenn ein Clonk mitf‰hrt
//  if (Passenger()) return(0); 
//  // Zu einem wartenden Clonk hinfahren
//  if (SpeedToVertical()) return(1);
//  // Eigenst‰ndige Bewegung beginnen
//  if (SelfTravel()) return(1);
//  return(0);
//}
//
//private func SelfTravel () {
//  // Nur wenn angehalten
//  if (GetComDir() != COMD_Stop()) return(0);
//  // Eigenst‰ndige Bewegung je nach zugelassener Richtung beginnen
//  if (1 == Mode) SetComDir(COMD_Left());
//  if (0 == Mode) SetComDir(COMD_Up());
//  if (2 == Mode) SetComDir(COMD_Down());
//  return(1);
//}
//
//private func SpeedToVertical () {
//  if (Mode != 0) return(0);
//  
//  var WaitClonk;
//  var upperdistance = AbsY(UpperY-20);
//  var lowerdistance = AbsY(LowerY+20);
//
//  for(WaitClonk in FindObjects(	Find_InRect(-50,upperdistance,100,-upperdistance+lowerdistance),
//								Find_OCF(OCF_CrewMember),
//								Find_NoContainer())) {
//
//	if (GetComDir(WaitClonk) != COMD_Stop()) continue;
//
//	SetAction("SpeedToY");
//	WaitY = Max(GetY(WaitClonk), UpperY-10);
//	
//	return(1);
//  }
//  
//  return(0);
//}
//
///* Aktion */
//
//private func CheckYTop () { // PhaseCall von Travel
//  if (GetY() < YTop()-10) {
//	SetPosition(GetX(), YTop()-10);
//	ContactTop();
//  }
//  return(1);
//}
//
//private func SpeedToY () { // StartCall von SpeedToY
//  SetYDir(BoundBy(WaitY - GetY(), -50, 50));
//  if (Inside(WaitY-GetY(),-4,+2)) Stop();
//  return(1);
//}
//
//private func SpeedToX () { // StartCall von SpeedToX
//  SetYDir(0);
//  SetXDir(BoundBy(WaitY-GetX(),-50,+50));
//  if (Inside(WaitY - GetX(),-10,+10)) Stop();
//  return(1);
//}
//
//private func UnstuckClonks () { // Sucht Clonks in der SolidMask und setzt sie auf die Platte
//  var pClonk;
//  while(pClonk = FindObject(0,-25,-2,50,20, 0, 0,0, NoContainer(), pClonk))
//    if(Stuck(pClonk))
//      SetPosition(GetX(pClonk), GetY(), pClonk);
//}
//
///* Status */
//
//private func Passenger () {
//  return( FindObject(0, -24,-13,48,16, OCF_CrewMember(),0,0,NoContainer()) );
//}
//
//public func IsLift() { return(1); }

/* Initialisierung */

protected func Initialize()
{
//  AddLightAmbience(100, this());
  //lLight = AddLightCone(1000,RGBa(255,255,220,20),this());
  //rLight = AddLightCone(1000,RGBa(255,255,220,20),this());

  //lLight->ChangeSizeXY(700, 500);
  //rLight->ChangeSizeXY(700, 500);
  //lLight->ChangeOffset(-11,-1);
  //rLight->ChangeOffset(+11,-1);
  //lLight->ChangeR(-130);
  //rLight->ChangeR(+130);

  AddEffect("LightEffects", this, 1, 5, this);
  SetAction("Travel");
  //UpdateTransferZone();
}

//public func FxLightEffectsTimer(target, number, time)
//{
//  // Funken! :D
//  CreateParticle("PSpark", RandomX(-8,8), 15, RandomX(-1,1), RandomX(2,5), 150+Random(70), RGB(50,50,25), this(), true);
//  // Lichter
///*  var r = LocalN("iR", rLight);
//  // Nach unten drehen
//  if(GetComDir() == COMD_Down())
//    {
//    if(r < 120)
//      r+=5;
//    else
//      return();
//    }
//  // Nach oben drehen
//  if(GetComDir() == COMD_Up())
//    {
//    if(r > 10)
//      r-=5;
//    else
//      return();
//    }
//  // mittig ausrichten
//  if(GetComDir() == COMD_Left() && GetComDir() == COMD_Right() && GetComDir() == COMD_Stop())
//    {
//    if(r > 90)
//      r-=5;
//    else
//      if(r < 90)
//        r+=5;
//      else
//        return();
//    }
//  rLight->ChangeR(+r);
//  lLight->ChangeR(-r);*/
//}
//
///* Transfer */
//
//protected func UpdateTransferZone () {
//  // Nur vertikale Schwebeplatten
//  if (Mode) return(SetTransferZone());
//  // Obergrenze ermitteln
//  var y1, y2;
//  while(GetY() + y1 > 5 && !GBackSolid(0, y1))
//    y1-=5;
//  // Untergrenze ermitteln
//  while(GetY() + y2 + 15 < LandscapeHeight()-5 && !GBackSolid(0, y2+15))
//    y2+=5;
//  // Transferzone setzen
//  SetTransferZone(-23,y1,48,y2-y1 +10);
//  // Ober- und Untergrenze speichern
//  if(!Limited) return;
//  UpperY = GetY() + y1;
//  LowerY = GetY() + y2;
//  return(1);
//}
//
//protected func ControlTransfer (object pObj, int iX, int iY) {
//  // Durchgang am Boden des Fahrstuhlschachts: nicht warten
//  if(Inside(GetY(pObj) - LowerY, -10, +10))
//    if(Inside(iY - LowerY, -10, +10))
//      return();
//
//  // Zielposition
//  // Maximale Obergrenze
//  iY = Max(iY, UpperY);
//
//  // Fahrstuhl mit Clonk an Zielposition: anhalten, erledigt
//  if(Inside(iY - GetY(), -5, +5))
//    if(Passenger())
//      return(0,Stop());
//
//  // Fahrstuhl noch nicht beim Clonk: warten
//  if(!Inside(GetY() - GetY(pObj), -20, +20))
//    return(1);
//
//  // Fahrstuhl nur in der Mitte anfassen
//  if(!Inside(GetX() - GetX(pObj), -15, +15))
//    return(AddCommand(pObj,"MoveTo",this(),0,0,0,15));
//
//  // Fahrstuhl anfassen
//  if(GetAction(pObj) ne "Push" || GetActionTarget(0, pObj) != this())
//    return(AddCommand(pObj,"Grab",this()));
//
//  // Fahrstuhl zur Zielposition steuern
//  if(iY < GetY())
//    return(SetComDir(COMD_Up()));  
//  return(SetComDir(COMD_Down()));
//}
//
///* Serialisierung */
//
//public func Serialize(array& extra)
//{
//	extra[GetLength(extra)] = Format("SetYTop(%d)", UpperY);
//	if (Mode == 1)
//		extra[GetLength(extra)] = "DisableVertical()";
//	if (Mode == 2)
//		extra[GetLength(extra)] = "EnableHorizontal()";
//}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// properties

local Name = "$Name$";
local Description = "$Description$";

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// actions

local ActMap = {
Travel = {
	Prototype = Action,
	Name = "Travel",
	Procedure = DFA_FLOAT,
	Length = 7,
	Delay = 5,
	FacetBase = 1,
	NextAction = "Travel",
	//EndCall = "Automatic",
	//PhaseCall = "CheckYTop",
},

Wait = {
	Prototype = Action,
	Name = "Wait",
	Procedure = DFA_FLOAT,
	Length = 1,
	Delay = 70,
	FacetBase = 1,
	NextAction = "Travel",
	//EndCall = "ResumeTravel",
},

SpeedToY = {
	Prototype = Action,
	Name = "SpeedToY",
	Procedure = DFA_NONE,
	Length = 1,
	Delay = 3,
	FacetBase = 1,
	NextAction = "SpeedToY",
	//StartCall = "SpeedToY",
},

SpeedToX = {
	Prototype = Action,
	Name = "SpeedToX",
	Procedure = DFA_NONE,
	Length = 1,
	Delay = 1,
	FacetBase = 1,
	NextAction = "SpeedToX",
	//StartCall = "SpeedToX",
},

};
