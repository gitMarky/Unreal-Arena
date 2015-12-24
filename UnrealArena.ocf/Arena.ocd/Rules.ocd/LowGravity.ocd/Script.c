/*-- Low Gravity --*/

#include Library_ConfigurableRule

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
