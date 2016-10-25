
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// properties

local Collectible = false;
local blood_type;
local ContactCalls = true;

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// functionality

public func ContactBottom()
{
	RemoveObject();
}

public func Launch(string type)
{
	if (MOD_NoBlood()) return RemoveObject();

	blood_type = type;
	ScheduleCall(this, this.Splatter, 1, 1);
}

public func Splatter()
{
	if (MOD_NoBlood()) return RemoveObject();
	
	EffectBloodSpray(1, 0, 0, 0);
	//CreateParticle("Blood",0,0,0,0,10+Random(30),RGBaRandom(BloodFXColor(blood_type)[0], BloodFXColor(blood_type)[1] ));

	ScheduleCall(this, this.Splatter, 1, 1);
}
