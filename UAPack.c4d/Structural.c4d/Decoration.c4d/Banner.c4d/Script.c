/*-- Banner --*/

#strict 2

private func Initialize()
{
	SetAction("Flag");
}

private func SetRed()
{
	SetClrModulation(RGBa(180,0,0));
}

private func SetBlue()
{
	SetClrModulation(RGBa(0,0,180));
}
