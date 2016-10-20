#include Library_ModularAppearance

func Initialize()
{
	this.ActMap = Clonk.ActMap;
	SetAction("Jump");
}

func CopyProps(object target)
{
	for (var name in target->GetProperties())
	{
		this[name] = target[name];
	}
}