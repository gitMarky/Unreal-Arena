/**
	Controller Status Bar

	Displays the status of the currently selected crew member.

	@author Marky
*/

local gui_status_bar_menu;
local gui_status_bar_id;
local gui_status_bars;

static const GUI_Controller_StatusBar_MarginLeft = 0;
static const GUI_Controller_StatusBar_MarginTop = 800;


/* GUI creation */

// For custom HUD graphics overload the following function as deemed fit.

func AssembleStatusBarMenu()
{
	return
	{
		Target = this,
		ID = 1,
		Style = GUI_Multiple | GUI_NoCrop | GUI_IgnoreMouse,
		ElementAmmoBar = AssembleStatusBar(0, "BarAmmo"),
		ElementHealthBar = AssembleStatusBar(1, "BarHealth"),
		ElementArmorBar = AssembleStatusBar(2, "BarArmor"),
		ElementShieldBar = AssembleStatusBar(3, "BarShield"),
	};
}

/* Creation / Destruction */

private func Construction()
{
	gui_status_bars = [];
	gui_status_bar_menu = AssembleStatusBarMenu();
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
	UpdateStatusBarDisplay();

	return _inherited(clonk, plr, ...);
}

public func OnCrewDeRecruitment(object clonk, int plr)
{
	UpdateStatusBarDisplay();

	return _inherited(clonk, plr, ...);
}

public func OnCrewDeath(object clonk, int killer)
{
	UpdateStatusBarDisplay();

	return _inherited(clonk, killer, ...);
}

public func OnCrewDestruction(object clonk)
{
	UpdateStatusBarDisplay();

	return _inherited(clonk, ...);
}

public func OnCrewDisabled(object clonk)
{
	UpdateStatusBarDisplay();

	return _inherited(clonk, ...);
}

public func OnCrewEnabled(object clonk)
{
	UpdateStatusBarDisplay();

	return _inherited(clonk, ...);
}

public func OnCrewSelection(object clonk, bool unselect)
{
	UpdateStatusBarDisplay();

	return _inherited(clonk, unselect, ...);
}


public func OnCrewHealthChange(object clonk, int change, int cause, int caused_by)
{
	// TODO: set values

	return _inherited(clonk, change, cause, caused_by, ...);
}



// Update everything
private func UpdateStatusBarDisplay()
{
	var cursor = GetCursor(GetOwner());
	
	if (!cursor)
	{
		HideStatusBar(gui_status_bar_menu.ElementAmmoBar);
		HideStatusBar(gui_status_bar_menu.ElementArmorBar);
		HideStatusBar(gui_status_bar_menu.ElementShieldBar);
		HideStatusBar(gui_status_bar_menu.ElementHealthBar);
	}
	else
	{
		ShowStatusBar(gui_status_bar_menu.ElementAmmoBar);
		ShowStatusBar(gui_status_bar_menu.ElementArmorBar);
		ShowStatusBar(gui_status_bar_menu.ElementShieldBar);
		ShowStatusBar(gui_status_bar_menu.ElementHealthBar);

		StatusBarSetValue(gui_status_bar_menu.ElementHealthBar, cursor->GetEnergy());
		StatusBarSetValue(gui_status_bar_menu.ElementArmorBar, cursor->~GetUTArmor());
		StatusBarSetValue(gui_status_bar_menu.ElementShieldBar, cursor->GetUTShield());
	}
}

/* Bars (health, breath, ...) */

// Adds a new bar to the portrait.
// The bar is not shown until ShowCrewBar() is called. Bars will appear in order of creation.
private func AssembleStatusBar(int slot_nr, string icon_name)
{
	var tab_width = 150;
	var tab_height = tab_width / 2;
	var offset_y = (-slot_nr) * (tab_height + 65);
	
//	var tab_position = {
//		Left = ToPercentString(GUI_Controller_StatusBar_MarginLeft),
//		Right = ToPercentString(GUI_Controller_StatusBar_MarginLeft + tab_width),
//		Top = ToPercentString(GUI_Controller_StatusBar_MarginTop + offset_y),
//		Bottom = ToPercentString(GUI_Controller_StatusBar_MarginTop + offset_y + tab_height),
//	};

	var tab_layout = {
		Grid = {
			Prototype = GUI_BoxLayout,
			Align = {X = GUI_AlignLeft, Y = GUI_AlignBottom},
			Margin = {Bottom = GUI_UA_InventoryBar_Height_Pt},
			Rows = 4,
		},
		Cell = {
			Prototype = GUI_BoxLayout,
			Margin = {Bottom = 10},
			Width = tab_width,
			Height = tab_height,
		},
	};
	
	var tab_position = GuiCalculateGridElementPosition(tab_layout, slot_nr, 0);

	var info_tab = {
		Target = this,
		Symbol = GUI_UA_InfoTab,
		GraphicsName = nil,
		ElementHex = {
			Target = this,
			Style = GUI_NoCrop,
			Priority = 2,
			Symbol = GUI_UA_InfoTab,
			GraphicsName = "Hex",
			Left = ToPercentString(0), Right = ToPercentString(1000), Top = ToPercentString(0), Bottom = ToPercentString(1000),
		},
		ElementIcon = {
			Target = this,
			Style = GUI_NoCrop,
			Priority = 3,
			Symbol = GUI_UA_InfoTab,
			GraphicsName = icon_name,
			Left = ToPercentString(0), Right = ToPercentString(1000), Top = ToPercentString(0), Bottom = ToPercentString(1000),
		},
		Element001 = AssembleStatusBarDigit(0),
		Element010 = AssembleStatusBarDigit(1),
		Element100 = AssembleStatusBarDigit(2),
	};
	
	AddProperties(info_tab, tab_position);

	return info_tab;
}

func AssembleStatusBarDigit(int dimension)
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


private func ShowStatusBar(proplist info_tab)
{
	if (GetOwner() == NO_OWNER) return;
	if (info_tab.Player == GetOwner()) return;

	info_tab.Player = GetOwner();
	GuiUpdate(gui_status_bar_menu, gui_status_bar_id);
}


private func HideStatusBar(proplist info_tab)
{
	if (info_tab.Player == NO_OWNER) return;

	info_tab.Player = NO_OWNER;
	GuiUpdate(gui_status_bar_menu, gui_status_bar_id);
}


/* Crew indices */

public func StatusBarSetValue(proplist info_tab, int value)
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

