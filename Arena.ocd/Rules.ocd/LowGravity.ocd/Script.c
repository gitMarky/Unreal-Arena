/*-- LowGravity --*/
#include Library_ConfigurableRule


global func MOD_LowGravity(){ return ObjectCount( Rule_LowGravity ); }

static g_iGravity;

public func Initialize()
{
	if(!g_iGravity) g_iGravity = GetGravity();

	SetGravity(g_iGravity/2);
	SetPosition();
}

public func Destruction()
{
	SetGravity( g_iGravity );
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// properties

local Name = "$Name$";
local Description = "$Description$";
