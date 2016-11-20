/**
	Controller Status Bar

	Displays the status of the currently selected crew member.

	@author Marky
*/

local crew_gui_menu;
local crew_gui_id;
local crew_bars;


/* GUI creation */

// For custom HUD graphics overload the following function as deemed fit.

func AssembleCrewBar()
{
	return
	{
		Target = this,
		ID = 1,
		Style = GUI_Multiple | GUI_NoCrop | GUI_IgnoreMouse,
	};
}

/* Creation / Destruction */

private func Construction()
{
	crew_bars = [];
	crew_gui_menu = AssembleCrewBar();
	crew_gui_id = GuiOpen(crew_gui_menu);
	
	return _inherited(...);
}

private func Destruction()
{
	GuiClose(crew_gui_id);
	crew_gui_id = nil;

	_inherited(...);
}

/* Callbacks */

public func OnCrewRecruitment(object clonk, int plr)
{
	UpdateCrewDisplay();

	return _inherited(clonk, plr, ...);
}

public func OnCrewDeRecruitment(object clonk, int plr)
{
	UpdateCrewDisplay();

	return _inherited(clonk, plr, ...);
}

public func OnCrewDeath(object clonk, int killer)
{
	UpdateCrewDisplay();

	return _inherited(clonk, killer, ...);
}

public func OnCrewDestruction(object clonk)
{
	UpdateCrewDisplay();

	return _inherited(clonk, ...);
}

public func OnCrewDisabled(object clonk)
{
	UpdateCrewDisplay();

	return _inherited(clonk, ...);
}

public func OnCrewEnabled(object clonk)
{
	UpdateCrewDisplay();

	return _inherited(clonk, ...);
}

public func OnCrewSelection(object clonk, bool unselect)
{
	UpdateCrewDisplay();

	return _inherited(clonk, unselect, ...);
}


public func OnCrewHealthChange(object clonk, int change, int cause, int caused_by)
{
	// TODO: set values

	return _inherited(clonk, change, cause, caused_by, ...);
}



// Update everything
private func UpdateCrewDisplay()
{
	var cursor = GetCursor(GetOwner());
}

/* Bars (health, breath, ...) */

// Adds a new bar to the portrait.
// The bar is not shown until ShowCrewBar() is called. Bars will appear in order of creation.
private func AddCrewBar(int foreground, int background)
{
	if (!crew_gui_id) return;
	if (!foreground) return;
	if (!background) background = RGB(40, 40, 40);

	var y_begin = GUI_Controller_CrewBar_CursorMargin + GUI_Controller_CrewBar_CursorSize;
	var y_end = y_begin + GUI_Controller_CrewBar_BarSize;

	var new_bar = 
	{
		ID = 100 + GetLength(crew_bars),
		shown = false
	};
	PushBack(crew_bars, new_bar);

	// Will display the bar at the left screen border

	var crew_gui_bar_name = Format("bar%d", new_bar.ID);
	crew_gui_menu.cursor[crew_gui_bar_name] =
	{
		Target = this,
		Player = NO_OWNER,
		Style = GUI_NoCrop,
		ID = new_bar.ID,
		Left = "0%",
		Right = Format("0%%%s", ToEmString(GUI_Controller_CrewBar_CursorSize)),
		Top = ToEmString(y_begin),
		Bottom = ToEmString(y_end),
		BackgroundColor = background,
		Priority = 3,
		fill =
		{
			Target = this,
			Left = "0%",
			Margin = ["0em", "0.1em"],
			BackgroundColor = foreground,
			Priority = 4
		},
		text =
		{
			Target = this,
			Top = "-0.5em",
			Bottom = "0.5em",
			Style = GUI_TextHCenter | GUI_TextVCenter,
			Text = "",
			Priority = 5
		}
	};

	GuiUpdate(crew_gui_menu, crew_gui_id);
	
	// Prevent further calls to GuiUpdate on the main panel from overwriting values in the bars with their defaults
	crew_gui_menu.cursor[crew_gui_bar_name] = nil;
	
	UpdateCrewDisplay();

	return GetLength(crew_bars)-1;
}

// Shows the bar that was saved in crew_bars[bar]
private func ShowCrewBar(int bar)
{
	if (!crew_bars[bar]) return;
	if (crew_bars[bar].shown) return;
	if (GetOwner() == NO_OWNER) return;

	// Bars at left side of the screen

	var top = GUI_Controller_CrewBar_CursorMargin + GUI_Controller_CrewBar_CursorSize;
	var i = 0;
	while (i < bar)
	{
		if (crew_bars[i] && crew_bars[i].shown)
			top += GUI_Controller_CrewBar_BarSize + GUI_Controller_CrewBar_BarMargin;
		i++;
	}
	var bottom = ToEmString(top + GUI_Controller_CrewBar_BarSize);
	top = ToEmString(top);

	crew_bars[bar].shown = GuiUpdate({ Player = GetOwner(), Top = top, Bottom = bottom }, crew_gui_id, crew_bars[bar].ID, this);
}

// Sets the fill status of the bar. value is between 0 and 1000
// Shows text_val before the bar if given
private func SetCrewBarValue(int bar, int value, int text_val)
{
	if (!crew_bars[bar]) return;
	value = BoundBy(value, 0, 1000);
	var plr = GetOwner();
	var bar_text = "";
	if (text_val) bar_text = Format("<c dddd00>%d</c>", text_val);
	// Displaying the fill with Top = 100% creates an unwanted scrollbar
	//if (value == 0) plr = NO_OWNER;

	GuiUpdate({ fill = { Player = plr, Right = ToPercentString(value) }, text = { Text = bar_text } }, crew_gui_id, crew_bars[bar].ID, this);
}

// Hides the bar that was saved in crew_bars[bar]
private func HideCrewBar(int bar)
{
	if (!crew_bars[bar]) return;
	if (!crew_bars[bar].shown) return;

	GuiUpdate({ Player = NO_OWNER }, crew_gui_id, crew_bars[bar].ID, this);
	crew_bars[bar].shown = false;

	// Update position of all following bars
	for (var i = bar; i < GetLength(crew_bars); i++)
		if (crew_bars[i].shown)
		{
			crew_bars[i].shown = false;
			ShowCrewBar(i);
		}
}

/* Crew indices */



