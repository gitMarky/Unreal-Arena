#include Clonk

func SetSkin(int new_skin)
{
	// Remember skin
	skin = new_skin;
	
	//Steampunk
	skin_name = "";
	gender = 0;
	
	RemoveBackpack(); // remove the backpack

	//refreshes animation (whatever that means?)
	// Go back to original action afterwards and hope
	// that noone calls SetSkin during more complex activities
	var prev_action = GetAction();
	SetAction("Jump");
	SetAction(prev_action);
	SetOwner(GetOwner()); // somehow the color did not get updated after setting the color

	return skin;
}

