/*-- Neues Script --*/

#strict 2

public func Initialize()
{
	MakeBackground();

	SetAction("Exist");
}

private func MakeBackground()
{
	SetCategory(GetCategory() | C4D_Parallax | C4D_Background);

	Local() = Local(1) = 65;

}

protected func UpdateTransferZone()
  {
  if (IsNewgfx()) if (~GetCategory() & C4D_Parallax) MakeBackground();
  }
