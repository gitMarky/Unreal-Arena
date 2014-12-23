#strict 2
#appendto FLSH

public func GearEffect(object pClonk,int rgba)
{
	if(!rgba) rgba = RGBa(255,255,220,90);
	if(light) RemoveObject(light);

	user = pClonk;
	light = AddLightCone(1000,rgba,user);
	light->ChangeSizeXY(1900,6000);
	light->ChangeOffset(0,0, true);
	SetAction("On");
	Sound("Click");
}

// damit die Dinger nicht am Boden rumliegen
public func GearUnbuckle(object pClonk)
{
  GearEffectReset(pClonk);
  RemoveObject();
}
