/**
	Controller Status Bar

	Displays the status of the currently selected crew member.

	@author Marky
*/

local gui_status_bar_menu;
local gui_status_bar_id;
local crew_bars;

static const GUI_Controller_StatusBar_MarginLeft = 0;
static const GUI_Controller_StatusBar_MarginTop = 800;


/* GUI creation */

// For custom HUD graphics overload the following function as deemed fit.

func AssembleStatusBar()
{
	return
	{
		Target = this,
		ID = 1,
		Style = GUI_Multiple | GUI_NoCrop | GUI_IgnoreMouse,
		ElementAmmoBar = AssembleCrewBar(0, "BarAmmo"),
		ElementHealthBar = AssembleCrewBar(1, "BarHealth"),
		ElementArmorBar = AssembleCrewBar(2, "BarArmor"),
		ElementShieldBar = AssembleCrewBar(3, "BarShield"),
	};
}

/* Creation / Destruction */

private func Construction()
{
	crew_bars = [];
	gui_status_bar_menu = AssembleStatusBar();
	gui_status_bar_id = GuiOpen(gui_status_bar_menu);
	
	return _inherited(...);
}

private func Destruction()
{
	GuiClose(gui_status_bar_id);
	gui_status_bar_id = nil;

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
private func AssembleCrewBar(int slot_nr, string icon_name)
{
	var tab_width = 150; //100;
	var tab_height = tab_width / 2; //50;
//	var offset_y = (-slot_nr) * 4 * tab_height / 3;
	var offset_y = (-slot_nr) * (tab_height + 65);

	var info_tab = {
		Target = this,
		Left = ToPercentString(GUI_Controller_StatusBar_MarginLeft),
		Right = ToPercentString(GUI_Controller_StatusBar_MarginLeft + tab_width),
		Top = ToPercentString(GUI_Controller_StatusBar_MarginTop + offset_y),
		Bottom = ToPercentString(GUI_Controller_StatusBar_MarginTop + offset_y + tab_height),
		Symbol = GUI_UA_InfoTab,
		GraphicsName = nil,
		ElementHex = {
			Target = this,
			Style = GUI_NoCrop,
			Priority = 2,
			Symbol = GUI_UA_InfoTab,
			GraphicsName = "Hex",
//			Left = "0%", Right = "100%", Top = "0%", Bottom = "100%",
			Left = ToPercentString(0), Right = ToPercentString(1000), Top = ToPercentString(0), Bottom = ToPercentString(1000),
		},
		ElementIcon = {
			Target = this,
			Style = GUI_NoCrop,
			Priority = 3,
			Symbol = GUI_UA_InfoTab,
			GraphicsName = icon_name,
//			Left = "0%", Right = "100%", Top = "0%", Bottom = "100%",
			Left = ToPercentString(0), Right = ToPercentString(1000), Top = ToPercentString(0), Bottom = ToPercentString(1000),
		},
		Element001 = AssembleDigit(0),
		Element010 = AssembleDigit(1),
		Element100 = AssembleDigit(2),
	};

	return info_tab;
}

func AssembleDigit(int dimension)
{
	var width = 150;
	var height = 520;
	var diff_x = 170;
	var position_x = 750 - dimension * diff_x;
	var position_y = 500 - height / 2;
	return {
		Target = this,
		Style = GUI_NoCrop,
		Symbol = GUI_UA_Number,
		GraphicsName = nil,
		Priority = 4 + dimension,
		Left = ToPercentString(position_x), Right = ToPercentString(position_x + width),
		Top = ToPercentString(position_y), Bottom = ToPercentString(position_y + height), 
		Digit = {
			Target = this,
			Style = GUI_NoCrop,
			Symbol = GUI_UA_Number,
			GraphicsName = "Number0",
		},
	};
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

	crew_bars[bar].shown = GuiUpdate({ Player = GetOwner(), Top = top, Bottom = bottom }, gui_status_bar_id, crew_bars[bar].ID, this);
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

	GuiUpdate({ fill = { Player = plr, Right = ToPercentString(value) }, text = { Text = bar_text } }, gui_status_bar_id, crew_bars[bar].ID, this);
}

// Hides the bar that was saved in crew_bars[bar]
private func HideCrewBar(int bar)
{
	if (!crew_bars[bar]) return;
	if (!crew_bars[bar].shown) return;

	GuiUpdate({ Player = NO_OWNER }, gui_status_bar_id, crew_bars[bar].ID, this);
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

public func SetNumbers(proplist info_tab, int value)
{
	var value100 = 0, value010 = 0, value001 = 0;

	value100 = (value / 100) % 10;
	value010 = (value / 10) % 10;
	value001 =  value % 10;

	info_tab.Element100.Digit.GraphicsName = Format("Number%d", value100);
	info_tab.Element010.Digit.GraphicsName = Format("Number%d", value010);
	info_tab.Element001.Digit.GraphicsName = Format("Number%d", value001);

	GuiUpdate(gui_status_bar_menu, gui_status_bar_id);
}



