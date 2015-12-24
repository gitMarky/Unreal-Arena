/*-- Announcer --*/

#include Library_ConfigurableRule

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// properties

local Name = "$Name$";
local Description = "$Description$";


//static aAnnouncer;
//
//protected func Initialize()
//{
//	local aLastMsg;
//	aLastMsg = [];
//	aAnnouncer = [];
//	aAnnouncer[0] = [];
//
//	AddEffect("AnnouncerQueue",this,50,1,this);
//}
//
//public func DoAnnouncerQueue()
//{
//	var f = FrameCounter();
//	if(GetType(aAnnouncer) != C4V_Array) return;
//	for( var i = 0; i<GetLength(aAnnouncer); i++)
//	{
//		var list = aAnnouncer[i];
//		var last = aLastMsg[i];
//
//		if(GetType(list) != C4V_Array) continue;
//		if(!GetLength(list)) continue;
//		if(f-last < 80) continue;
//
//			aLastMsg[i]=f; // FrameCounter updaten
//			eval(list[0]); // Sound abspielen, wurde vorher gespeichert
//			DeleteArrayItem(0,aAnnouncer[i]); // Sound aus der Schlange werfen
//	}
//}
//
//
//global func UA_Announcer( string szSound, pObj, bool bInstant)
//{
//	if(!MOD_Announcer()) return;
//	if(GetType(pObj) == C4V_Int) if(pObj != 0) return;
//	if(pObj)
//	{
//		if(bInstant)
//		{
//			Sound(szSound,true);
//			return;
//		}
//		else
//		{
//			if(GetEffect("NoAnnouncer", pObj)) return;
//			UA_NoAnnouncer(pObj);
//		}
//	}
//
//	var iPlr = 0;
//	if(pObj) iPlr = GetOwner(pObj)+1;
//
//	var command = Format("Sound(\"%s\", true, 0, 0, %d)",szSound,iPlr);
//
//	// zur Sicherheit
//	if(!aAnnouncer[iPlr] || GetType(aAnnouncer[iPlr] != C4V_Array)) aAnnouncer[iPlr] = [];
//	PushBack(command,aAnnouncer[iPlr]);
//}
//
//global func UA_NoAnnouncer( object pTarget) // Hält 20 Frame lang, also Clonk schnell töten, dann kommt keine Nachricht
//{
//	AddEffect("NoAnnouncer", pTarget, 1, 1, 0, R_M6);
//}
//
//public func FxNoAnnouncerStop(dummy, dummy2, int iReason)
//{
//	if(iReason == 4) return -1;
//}
//
//public func FxAnnouncerQueueTimer()
//{
//	DoAnnouncerQueue();
//}
